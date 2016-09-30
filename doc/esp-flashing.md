# Putting esp-link on the ESP

The ESP-01 is slightly fiddly, but not technically complicated, to flash.  Get
a Nano on a breadboard with a bit of room and an ESP-01 in an adatper.  You
need a few wires to connect the adatper to the Nano; either four male-female to
have it dangling, or four male-male to do it via the breadboard.  You also
need:

 * a USB/serial (aka FTDI) cable
 * five wires, each with at least one male end
 * a copy of [esptool.py][esptool]
 * a copy of the esp-link firmware (I used [version 2.2.3][esplink] because it
   was the latest stable)

 [esptool]: https://github.com/themadinventor/esptool
 [esplink]: https://github.com/jeelabs/esp-link/releases/tag/v2.2.3

Wire the breadboard [as pictured](fritzing/ShonkESP-flashing.png) but don't
plug the adatper in yet.  The flying black wire, a male-male, stays unconnected
at the far end for the moment.

You may need three hands for the next bit.  With the flying end of the black
wire, firmly touch pin GPIO-0; maintain contact while plugging the adatper in.
You can then break contact and let the flying black wire go somewhere out of
the way.

Unpack the esp-link firmware zip and change to that directory.  Use esptool.py
as per the instructions in the esp-link release notes; probably something like:

    ~/path/to/esptool/esptool.py \
        --port /dev/ttyUSB0 \
        write_flash -fs 4m -ff 40m \
        0x00000 boot_v1.5.bin \
        0x01000 user1.bin \
        0x7E000 blank.bin

With any luck, you should see ESP\_789ABC or similar appear as a new access
point.  Connect to it, and visit its [admin page](192.168.4.1).  You'll want
to:

 * change the serial baud under "µC Console" to 38400
 * change the hostname to something more interesting
 * disable esp-link itself logging debug info over the serial port; "Debug Log
   -> UART debug log -> off"
 * reset the ESP (IIRC this is under Debug Log; this _may_ avoid hangs later if
   switching to client mode)

Don't change any other settings.  The typed settings save to flash
automatically when you hit Enter.  The "Info" panel claims you can flash the
Nano via the ESP, but our adatper doesn't break out enough pins.  REST and MQTT
won't work; they require supporting libraries on the Nano that we don't have
space for just now.

The way esp-link is intended to work is to be a WiFi client so you can do RESTy
MQTTish things on the real Internet.  It starts as an isolated AP so you can
find it and tell it credentials for your home network and then manually switch
to another mode.

Mine did go through a period of getting stuck when switching to client mode
(symptom: admin page won't load, blue light on ESP solidly on); if so, unplug
it and repeat the flashing procedure (and even then, you may have to settle
for AP mode only).

# Annoyance

It'd be really nice to be able to do this with just Shonkbot parts, i.e. no
extra USB/serial cable, and since the Nano effectively contains a USB/serial
converter it should be possible.  However, the Nano's CH340 doesn't seem to be
able to talk serial to anything but the AVR; once you add an ESP listening on
the same lines, neither AVR nor ESP seem to hear anything from the CH340.
Perhaps the CH340's drive capacity is too low and/or the ESP draws too much.
Or too little.  Maybe a question for the Circuit Whisperer.
