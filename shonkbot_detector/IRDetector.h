#ifndef IRDetector_h
#define IRDetector_h



/*

IRDetector

Generates and detects flashing IR at a specified frequency

Usages
--------

Collision detection: 

IRDetector( 13, A0, 12, 100 );  
With  
 - IR LED on pin 13
 - phototransistor from pin A0 to ground, and a 10k pullup
 - piezo sounder on pin 12
Will detect obstacles by detecting the reflection of the LED, using a 100Hz square wave.
Piezo sounder indicates range when object is detected.
Use getRangeInCm() to get the range.


Beacon for swarming:

IRDetector( 13, -1, -1, 75 );  
With  
 - IR LED on pin 13
Will run the LED with a 75Hz square wave.


Detector for swarming:

IRDetector( -1, A0, 12, 75 );  
With  
 - phototransistor from pin A0 to ground, and a 10k pullup
 - piezo sounder on pin 12
Will detect 75Hz square wave. Pizo sounder will indicate signal level.

Use getSignal() to read the received signal level.


You can use one phototransistor (and even one piezo) with multiple IRDetectors, so you can use the same
phototransistor for both collision detection and swarming if you want.


*/



#include <stdlib.h>
#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#include <wiring.h>
#endif

#define MAX_RANGE 30


class IRDetector
{
  int8_t ledPin;
  int8_t ptPin;
  int8_t piezoPin;
  
  int frequency; // not over 250!
  long period;
  long nextSampleTime;
  long phaseDuration;
  float readings[4]; // one for each of 4 phases
  long totalReadings;
  float minReading;
  float maxReading;
  float numSamples;
  float signal;
   int toneFrequency;
  
  public:
  IRDetector( int8_t _ledPin, int8_t _phototransistorPin, int8_t _piezoPin, int _frequency );
  void setup();
  void loop();
  int getSignal();
  int getRangeInCm();         // range to nearest object or 0 if there's nothing detectable
  int getAmbientBrightness(); // 0 to 1023, higher means brighter
  
  private:
  void analyse();
  
};

#endif 
