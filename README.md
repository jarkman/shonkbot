# Shonkbot

A supercheap Arduino-based 2-wheel robot for robot workshop purposes.
A collaboration by various Bristol Hackspace members.

See 

* [Images and video](http://jarkman.co.uk/catalog/robots/shonkbot.htm)
* [Detailed contstruction instructions](http://bit.ly/1N9sv5e) (written for Bristol Hackspace Skillshare)
* [Previous instructions](http://bit.ly/1GEuDTD) (written for Digimakers)

# Software

Use the usual Arduino IDE: 
 http://www.arduino.cc/en/Main/Software

If you have a Mac, you need drivers for the Arduino clones (CH340G)
 http://kiguino.moos.io/2014/12/31/how-to-use-arduino-nano-mini-pro-with-CH340G-on-mac-osx-yosemite.html
 -> http://kiguino.moos.io/downloads/CH341SER_MAC.ZIP

You need this library for the stepper motors:
 http://www.airspayce.com/mikem/arduino/AccelStepper/
 http://www.airspayce.com/mikem/arduino/AccelStepper/AccelStepper-1.47.zip

A common problem is that the micro USB cable you use to upload the code to the Arduino MUST be a data cable not just a power cable.

# Install

 * clone the [Shonkbot code](https://github.com/jarkman/shonkbot) from github
 * open shonkbot/shonkbot_detector/shonkbot_detector.ino in the Arduino IDE
 * download the AccelStepper library ([zip](http://www.airspayce.com/mikem/arduino/AccelStepper/AccelStepper-1.47.zip)) and [import the library into the IDE](http://www.arduino.cc/en/Guide/Libraries)
 * select "arduino nano" for your board in the IDE
 * plug the Arduino into the computer using a mini USB cable
 * locate the serial port. If you're using the Arduino clones we gave you, on a Mac you'll need to [download drivers](http://kiguino.moos.io/2014/12/31/how-to-use-arduino-nano-mini-pro-with-CH340G-on-mac-osx-yosemite.html). Windows and Linux _may_ just work; On some versions of Windows you may have to explicitly allow the system to install the drivers.
 * open shonkbot_detector/shonkbot_detector.ino in the IDE
 * upload changes to the code to the board using the IDE
