// 45678901234567890123456789012345678901234567890123456789012345678901234567890
//       1         2         3         4         5         6         7         8
#include <stdlib.h> // qsort

// Pin definitions
const unsigned LEFT_FWD = 4;
const unsigned LEFT_REV = 5;
const unsigned LEFT_EN = 6;
const unsigned RIGHT_FWD = 7;
const unsigned RIGHT_REV = 8;
const unsigned RIGHT_EN = 9;
const unsigned PIEZO_PIN = 10;

// Connected to the HC-SR04's Trig pin.
const unsigned RANGER_INITIATE = A0;
// Connected to the HC-SR04's Echo pin.
const unsigned RANGER_RESULT = A1;

const unsigned UNUSED_ANALOGUE = A4;

// Universal constants

// Unit is microseconds.  All sound travels at 343.42m/s in ideal air; the
// pulse must make a round-trip to something no more than about 5m away
// (according to the datasheet-like things it does have) so travels 10m.
// Allow 10% extra range, so 11m.  Timeout therefore should be about
// 11/343.42 = 0.032031s.
// One of the Chinglish-y docs says "we suggest to use over 60ms measurement
// cycle, in order to prevent trigger signal to the echo signal".  Huh?
const unsigned RANGER_TIMEOUT = 32031;
// Speed of sound in air is also used to convert range time to distance.
// Unit: metres per second
const float SOUND_SPEED_IN_AIR = 343.42;

// Vital statistics of the bot

const unsigned WHEEL_SEPARATION = 130; // mm
const unsigned BOT_CIRCUMFERENCE = PI * WHEEL_SEPARATION;
// Haven't needed these yet.
//const unsigned WHEEL_DIAMETER = 48; // mm
//const unsigned WHEEL_CIRCUMFERENCE = PI * WHEEL_DIAMETER;
// Recorded just by roughly counting wheel rotations under no load, at 1.4V
// (again, no-load voltage, not even free wheels).
// mm/s, = 48 * Pi * ~1.3333 rotations per second
//const unsigned FULL_SPEED = 201; 
// Recorded in a pivoting circle (one stopped, one moving wheel) over 7
// mid-movement rotations taking 29s.  130 * 2 * 3.14159 * 7 = 5717.69380mm
// 5717.69380 / 29 = 197.16185
const unsigned FULL_SPEED = 197;
// Takes ~this long before the wheels actually start turning, probably owing
// to gear slop or motor inductance/capacitance or something.
const unsigned WHEEL_LATENCY = 72; // ms
// A spin that should go 360deg needs a fudge factor of about this much when
// turning clockwise.  Not clear why, but seems one motor is faster than the
// other by a little.  Proper fix here is to run bot along a straight line
// and tweak PWM on the faster motor to slow it until bot runs straight.
const float SPIN_FUDGE_CLOCKWISE = 1.19;

enum wheel_action {
  BACKWARDS = -1,
  NOTHING = 0,
  FORWARDS = 1
};

void engage(wheel_action left, wheel_action right, unsigned duration);
void pivot(int degrees, wheel_action turn);
void spivot(int degrees, wheel_action turn);

// PWM under ~100 won't overcome gear slop.
// PWM under ~150 won't overcome gear slop/bot's own weight.

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(LEFT_FWD, OUTPUT);
  pinMode(LEFT_REV, OUTPUT);
  pinMode(LEFT_EN, OUTPUT);
  pinMode(RIGHT_FWD, OUTPUT);
  pinMode(RIGHT_REV, OUTPUT);
  pinMode(RIGHT_EN, OUTPUT);
  
  pinMode(RANGER_INITIATE, OUTPUT);
  pinMode(RANGER_RESULT, INPUT);
  
  Serial.begin(19200);
  Serial.print("BOT_CIRCUMFERENCE is ");
  Serial.println(BOT_CIRCUMFERENCE);
  
  randomSeed(analogRead(UNUSED_ANALOGUE) + single_horizon());
}

unsigned prev_horizon = 0;

void loop() {
  // For a behaviour where the bot generally explores the environment and
  // tries to avoid being stuck, and with our only measure of success being
  // distance to things, we repeatedly:
  //  - move forwards if it looks like we can; otherwise
  //  - reverse then turn, and if that makes no difference to horizon...
  //  - pivot backwards one way, and if that makes no difference to horizon...
  //  - pivot backwards the other way, and if that makes no difference...
  //  - stop and cry
  // The aims for fun continuous roaming are:
  //  - avoid getting stuck in the first place (maybe including orthogonal
  //    sensors)
  //  - have techniques for getting unstuck (maybe including orthogonal
  //    actuators)
  // More sensors only help if they tell you something different, useful and
  // reliable --- new dimensions --- which you can use to avoid getting stuck.
  // Encoders only really help for accurate motion; you can still wheelspin
  // and have no idea.  Mouse optics only really help you know you definitely
  // are stuck (and give you something fun to look at over remote monitoring).
  int range = slow_horizon();
  Serial.print("Initial distance to horizon: ");
  Serial.print(range);
  Serial.println("mm");
  
  // Did our last advance really move us forward, or are we just spinning
  // wheels on the spot?
  if ((prev_horizon - range) > 20) {
    // Yes; worth pushing a bit further.
    prev_horizon = range;
    
    if (range > 150) {
      advance(100);
    } else if (range > 100) {
      advance(50);
    }
  } else {
    // Either we've run out of road or we're stuck.  Evasive manoeuvres.
    reverse_beep();
    retreat(70);
    spin(120 * randomSign());
    
    range = slow_horizon();
    Serial.print("Post-retreat-and-turn distance to horizon: ");
    Serial.print(range);
    Serial.println("mm");
    if (range < 100) {
      reverse_beep();
      int sign = randomSign();
      pivot(135 * sign, BACKWARDS);
      
      range = slow_horizon();
      Serial.print("Post-backwards-pivot distance to horizon: ");
      Serial.print(range);
      Serial.println("mm");
      if (range < 100) {
        reverse_beep();
        // Whatever way we didn't try last time.
        sign *= -1;
        pivot(135 * sign, BACKWARDS);
        
        range = slow_horizon();
        Serial.print("Post-backwards-pivot2 distance to horizon: ");
        Serial.print(range);
        Serial.println("mm");
        if (range < 100) {
          stop_and_cry();
        }
      }
    }
  }
}

// Should connect up the piezo and use that really.
void stop_and_cry() {
  while (true) {
    digitalWrite(LED_BUILTIN, HIGH);
    for (unsigned hz = 20; hz <= 10000; hz *= 1.3) {
      tone(10, hz);
      delay(25);
      noTone(10);
    }
    digitalWrite(LED_BUILTIN, LOW);
    for (unsigned hz = 10000; hz >= 20; hz /= 1.3) {
      tone(10, hz);
      delay(25);
      noTone(10);
    }
  }
}

void reverse_beep() {
  tone(PIEZO_PIN, 1800, 700);
/*  tone(PIEZO_PIN, 1800);


  delay(700);
  noTone(PIEZO_PIN);
  delay(700);
  */
}

// All of these return distance to horizon in mm, or -1 if range cannot be
// reliably determined.

int single_horizon() {
  digitalWrite(RANGER_INITIATE, HIGH);
  delayMicroseconds(10);
  digitalWrite(RANGER_INITIATE, LOW);
  // Maybe the receiver has its own timeout and already brings RANGER_RESULT
  // low after waiting that long for a pulse; or maybe it just gets stuck
  // until the next RANGER_INITIATE pulse.  Also seems possible we could fail
  // to initiate a pulse properly, or miss RANGER_RESULT going high.
  unsigned duration = pulseIn(RANGER_RESULT, HIGH, RANGER_TIMEOUT);
  if (duration == 0) {
    return -1;
  }
  return duration * (1000.0 * SOUND_SPEED_IN_AIR / 1000000) / 2;
}

// Take a cheap-but-not-stupid horizon reading.  If any of a small number of
// readings are failures or the max is more than a fudge factor greater than
// the min, fail; otherwise return the average.
int cheap_horizon() {
  const unsigned SAMPLES = 3;
  const float FUDGE_FACTOR = 1.2;
  int cur_range;
  int max_range = 0;
  int min_range = 7500;
  unsigned int sum_range = 0;
  
  for (unsigned i=0; i<SAMPLES; i++) {
    cur_range = single_horizon();
    if (cur_range == -1) {
      goto fail;
    }
    sum_range += cur_range;
    if (cur_range > max_range) {
      max_range = cur_range;
    }
    if (cur_range < min_range) {
      min_range = cur_range;
    }
  }
  if (max_range > FUDGE_FACTOR * min_range) {
    goto fail;
  }
  return (sum_range / (float)SAMPLES);
  
fail:
  return -1;
}

// If a cheap reading fails, take tens more successful ones and average those.
// Will still give up and fail after enough failed single readings.
int slow_horizon() {
  int range = cheap_horizon();
  if (range != -1) {
    return range;
  }
  
  unsigned collected = 0;
  
  // Median code assumes this is odd.
  const unsigned SAMPLES = 19;
  int ranges[SAMPLES];
  // Have to Signed/unsigned is 
  for (unsigned i=0; i < SAMPLES;     ) {
    ranges[i] = single_horizon();
    collected++;
    if (ranges[i] != -1) {
      i++;
    } else if (collected >= 20 && i <= 5) { // arbitrary and magic numbers
      return -1;
    }
  }
  qsort(ranges, SAMPLES, sizeof(int), intcmp);
  return ranges[SAMPLES/2];
}


void spin(int degrees) {
  spivot(degrees, NOTHING);
}

// You can pivot clockwise by 45deg either by:
//  - turn left wheel forwards, keep right wheel still
// or
//  - turn right wheel backwards, keep left wheel still
void pivot(int degrees, wheel_action turn) {
  spivot(degrees, turn);
}

void advance(unsigned mm) {
  unsigned duration = 1000.0 * (float)mm / (float)FULL_SPEED;
  
  engage(FORWARDS, FORWARDS, duration);
}

void retreat(unsigned mm) {
  unsigned duration = 1000.0 * (float)mm / (float)FULL_SPEED;
  engage(BACKWARDS, BACKWARDS, duration);
}

// 'turn' is a bit overridden, but broadly describes the net *movement* of the
// bot; a spin does NOTHING to the bot's position, only orientation; a pivot
// can move forwards or backwards (a bit, while also changing orientation).
void spivot(int degrees, wheel_action turn) {
  if (abs(degrees) >= 360) {
    degrees %= 360;
  }
  
  // Minimise movements that are just inaccuracies in WHEEL_LATENCY.
  if (degrees == 0) {
    return;
  }
  float distance = (degrees / 360.0) * BOT_CIRCUMFERENCE;
 
  if (degrees > 0) {
    distance *= SPIN_FUDGE_CLOCKWISE;
  }/* else {
    distance *= 1.0 / SPIN_FUDGE_CLOCKWISE;
  }*/

  unsigned duration = abs(1000.0 * distance / (float)FULL_SPEED);
  
  if (turn) {
    duration *= 2;
    Serial.print("duration for pivot of ");
  } else {
    Serial.print("duration for spin of ");
  }
  Serial.print(degrees);
  Serial.print("deg is ");
  Serial.println(duration);
  
  if (degrees > 0) {
    engage((turn >= NOTHING ? FORWARDS : NOTHING),
           (turn <= NOTHING ? BACKWARDS : NOTHING), duration);
  } else {
    engage((turn <= NOTHING ? BACKWARDS : NOTHING),
           (turn >= NOTHING ? FORWARDS : NOTHING), duration);
  }
}

void engage(wheel_action left, wheel_action right, unsigned duration) {
  if (left) {
    digitalWrite(LEFT_FWD, (left == FORWARDS ? HIGH : LOW));
    digitalWrite(LEFT_REV, (left == FORWARDS ? LOW : HIGH));
  }
  if (right) {
    digitalWrite(RIGHT_FWD, (right == FORWARDS ? HIGH : LOW));
    digitalWrite(RIGHT_REV, (right == FORWARDS ? LOW : HIGH));
  }
  
  if (left) {
    digitalWrite(LEFT_EN, HIGH);
  }
  if (right) {
    digitalWrite(RIGHT_EN, HIGH);
  }

  delay(WHEEL_LATENCY + duration);
  
  digitalWrite(RIGHT_EN, LOW);
  digitalWrite(LEFT_EN, LOW);
}

int intcmp(const void *aa, const void *bb)
{
  const int *a = (const int *)aa, *b = (const int *)bb;
  return (*a < *b) ? -1 : (*a > *b);
}

// Return a randomly chosen 1 or -1.
int randomSign() {
  return random(2) * 2 - 1;
}
