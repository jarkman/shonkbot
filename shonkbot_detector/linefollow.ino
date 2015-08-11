
#define LF_PHOTO_RIGHT_PIN A0
#define LF_PHOTO_LEFT_PIN A1
#define LF_LED_PIN 13

void setupLineFollow()
{
  pinMode(LF_LED_PIN, OUTPUT);
  digitalWrite(LF_LED_PIN, HIGH);
  tone(PIEZO_PIN, 4000, 300);
  delay(50);
  tone(PIEZO_PIN, 2000, 300);
  delay(50);
  tone(PIEZO_PIN, 4000, 300);
}

void loopLineFollow()
{
  if (twoWheel.arrived()) {
    twoWheel.go(10);
    int left = analogRead(LF_PHOTO_LEFT_PIN);
    int right = analogRead(LF_PHOTO_RIGHT_PIN);
    if (left - right > 50) {
      twoWheel.turn(10);
    } else if (right - left > 50) {
      twoWheel.turn(-10);
    }
  }
}
