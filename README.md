# Shonkbot

A supercheap Arduino-based 2-wheel robot for robot workshop purposes.
A collaboration by various Bristol Hackspace members.

See
 http://jarkman.co.uk/catalog/robots/shonkbot.htm

for images and video and

 http://bit.ly/1GEuDTD

for Digimakers detailed instructions

# Software

Use the usual Arduino IDE: 
 http://www.arduino.cc/en/Main/Software

If you have a Mac, you need drivers for the Arduino clones (CH340G)
 http://kiguino.moos.io/2014/12/31/how-to-use-arduino-nano-mini-pro-with-CH340G-on-mac-osx-yosemite.html
 -> http://kiguino.moos.io/downloads/CH341SER_MAC.ZIP

You need this library for the stepper motors:
 http://www.airspayce.com/mikem/arduino/AccelStepper/
 http://www.airspayce.com/mikem/arduino/AccelStepper/AccelStepper-1.47.zip

# Install

 * git clone https://github.com/jarkman/shonkbot
 * open shonkbot/shonkbot_detector/shonkbot_detector.ino in the Arduino IDE
 * download http://www.airspayce.com/mikem/arduino/AccelStepper/AccelStepper-1.47.zip and import the library into the IDE: http://www.arduino.cc/en/Guide/Libraries
 * select "arduino nano" for your board in the IDE
 * plug the Arduino into the computer using a mini USB cable
 * locate the serial port. On a Mac you'll need to (download drivers)[http://kiguino.moos.io/2014/12/31/how-to-use-arduino-nano-mini-pro-with-CH340G-on-mac-osx-yosemite.html]. Windows and LInux should just work.
 * upload the code to the board using the IDE
