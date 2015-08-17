#include "IRDetector.h"


IRDetector::IRDetector( int8_t _ledPin, int8_t _phototransistorPin, int8_t _piezoPin, int _frequency )
{
  ledPin = _ledPin;
  ptPin = _phototransistorPin;
  piezoPin = _piezoPin;
  
  frequency = _frequency; // Practical maximum frequency is probably 500hz
  
  period = 1000000 / frequency; // in microseconds
  phaseDuration = period/4; // in microsecods
  numSamples = 20.0; // rolling average
  signal = 0.0;
  
  for( int p = 0; p < 4; p ++ )
    readings[p] = 0.0;
    
  totalReadings = 0;
  
}

void IRDetector::setup()
{
  if( ledPin > -1 )
    pinMode(ledPin, OUTPUT);
    
    nextSampleTime = micros();
}

// Experiment suggests we come through here at about 1mS intervals
void IRDetector::loop()
{
  
  
  long now = micros();
  if( now > nextSampleTime )
  {
    int timeInPeriod = now % period;
    int phase = timeInPeriod / phaseDuration;
    nextSampleTime = now - (now % phaseDuration) + phaseDuration; // make sure we keep the sampling times in phase
    
    if( ledPin > -1 )      // if we have a LED
      digitalWrite( ledPin, phase < 2 ); // turn it on for the first two phases and off for the next two
    
    if( ptPin > -1 )
    {
      int r = analogRead(ptPin); // 0 to 1024
      
      readings[phase] = (readings[phase] * (float) (numSamples -1) + (float) r ) / numSamples;
      
      totalReadings++;
      
      analyse();
      
      if( piezoPin > -1 )
      {
        int range = getRangeInCm();
        //Serial.print( " range " );
        //Serial.println( range);
        
        if( range != 0 )
        {
           toneFrequency = ((MAX_RANGE - range) + 20) * 100;
           
           if( piezoPin >= 0 )
              tone(piezoPin, toneFrequency);
        }
        else
        {
          if( toneFrequency != 0 )
            if( piezoPin >= 0 )
              noTone(piezoPin);
            
          toneFrequency = 0;
        }
      }
    }
      

  }
}

void IRDetector::analyse()
{
  // we have four phase signals, we would expect to have two high readings (whem we are receiving signal) and two low ones (when we are not)
  int min = 0;
  int max = 0;
  float sum = 0;
  
  for( int p = 1; p < 4; p ++ )
  {
    if( readings[p] < readings[min] )
      min = p;
      
    if( readings[p] > readings[max] )
      max = p;
      
     sum += readings[p];
  }
  
  // So our signal is really the difference between our max and our min signals

  signal = (readings[max] - readings[min]) ; 
  minReading = readings[min]; 
  maxReading = readings[max]; 
  
  /*
    Serial.print ("IRDetector::analyse - signal ");
    Serial.print( signal );
    Serial.print( " range " );
    Serial.print( getRangeInCm());
    Serial.print( " min "  );
    Serial.print( minReading );
    Serial.print( " max "  );
    Serial.println( maxReading );
  */
  
 
}

// 0 to 1024, higher means closer
// 5 or so is the normal far-away noise value
// very roughly:
// 50 means something is 10cm away
// 100 means 5cm
// 800 means 1cm

int IRDetector::getSignal() 
{
  if( totalReadings < 100L ) // ignore our state until we've done a full set of readings
    return 0;
    
  return (int) signal;
}

// In reflection mode, with ledPin and ptPin set, returns approx range to nearest object in cm or 0 if there's nothing

int IRDetector::getRangeInCm() 
{
  int range = (int) (500.0/(getSignal()+1)); // +1 to avoid divide-by-zero errors
  if( range < MAX_RANGE )
    return range+1;
  else
    return 0;
}

int IRDetector::getAmbientBrightness() // 0 to 1023, higher means brighter. Above 975 or so we can't detect our LED at all.
{
  return (int) (1023.0 - maxReading); // maxreading represents the phase when we didn't detect our LED, so it must represent the ambient brightness.
}
