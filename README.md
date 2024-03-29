# Shonkbot

A supercheap Arduino-based 2-wheel robot for robot workshop purposes.
A collaboration by various Bristol Hackspace members. Designed by Richard Sewell.

See 

* [Images and video](http://jarkman.co.uk/catalog/robots/shonkbot.htm)
* [Detailed construction instructions](https://docs.google.com/document/d/1QHNT2ikNUSk4J_2aCeMx6DPxwK5BRjbKGgpWV1k8Bqs/edit) (written for Bristol Mini Maker Faire)
* [Bill of materials (October 2016)](https://docs.google.com/spreadsheets/d/1L7IJjhZtqhlaf8FUNChBJWbDVZbxt01uz1hc8_6pe8Y/edit#gid=0)
* [Guide to running a Shonkbot workshop](https://docs.google.com/document/d/1BIUs8wwT11izIy_k6pxFZcgozXeECxZGXGSwyxmB_EE)

# Software

Use the usual Arduino IDE: 
 http://www.arduino.cc/en/Main/Software

If you have a Mac, you need drivers for the Arduino clones (CH340G)
 http://kiguino.moos.io/2014/12/31/how-to-use-arduino-nano-mini-pro-with-CH340G-on-mac-osx-yosemite.html
 -> http://kiguino.moos.io/downloads/CH341SER_MAC.ZIP

You need this _particular_ library for the stepper motors:
 http://www.airspayce.com/mikem/arduino/AccelStepper/
 http://www.airspayce.com/mikem/arduino/AccelStepper/AccelStepper-1.47.zip

A common problem is that the micro USB cable you use to upload the code to the Arduino MUST be a data cable not just a power cable. If you do the steps below and don't find a serial port, that may be the issue.

# Install

 * clone the [Shonkbot code](https://github.com/jarkman/shonkbot) from github
 * open shonkbot/shonkbot_detector/shonkbot_detector.ino in the Arduino IDE
 * download the [AccelStepper library](http://www.airspayce.com/mikem/arduino/AccelStepper/index.html) ([zip](http://www.airspayce.com/mikem/arduino/AccelStepper/AccelStepper-1.47.zip)) and [import the library into the IDE](http://www.arduino.cc/en/Guide/Libraries)
 * select "arduino nano" for your board in the IDE
 * you may need to select Processor: APMega328P **(old bootloader) 
 * plug the Arduino into the computer using a mini USB _data_ cable
 * locate the serial port. If you're using the Arduino clones we gave you, on a Mac you'll need to [download drivers](http://kiguino.moos.io/2014/12/31/how-to-use-arduino-nano-mini-pro-with-CH340G-on-mac-osx-yosemite.html). Windows and Linux _may_ just work; On some versions of Windows you may have to explicitly allow the system to install the drivers.
 * open shonkbot_detector/shonkbot_detector.ino in the IDE
 * upload changes to the code to the board using the IDE
