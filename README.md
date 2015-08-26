# Quickshonk

A slight variation on the [normal shonkbot](https://github.com/jarkman/shonkbot)
which is:

 * Somewhat faster
 * Somewhat less accurate, so...
 * Probably not as well suited to drawing things
 * Hopefully just as cheap
 * Certainly less well documented
 * Not properly integrated with shonkbot (code written for one won't work on
   the other)

The purpose was:

 * see if a faster one can stay within the budget (it broadly does, I think,
   and ultrasonic is an indulgence really)
 * give interested people variations to build after shonkbot

AccelStepper is not used.

[Ultrasonic-HC-SR04](https://github.com/JRodrigoTech/Ultrasonic-HC-SR04) is
needed.

# Parts/changes

 * No stepper motors, no stepper motor drivers, (currently) no LEDs
 * An SN754410 or L293D ([£0.32](http://www.ebay.co.uk/itm/141663342419) to [£1.80](http://www.hobbytronics.co.uk/h-bridge-driver-sn754410) depending on how many/how fast)
 * 2 x [TT gearhead motor](http://www.ebay.co.uk/itm/391083333774) (~£2)
 * An [HC-SR04 ultrasonic rangefinder](http://www.ebay.co.uk/itm/301559167417) (~£1)

# Wiring

Those gearhead motors don't come with wires, so some kind of soldering,
friction/clamping or wrapping of wires around the tabs is needed.

See [the Fritzing diagram](doc/quickshonk_bb.pdf) for general wiring.  There's
no step-by-step like shonkbot has, sorry.

# Behaviour

Quickshonk currently just wanders about avoiding bumping into walls, and tries
a couple of manoeuvres to escape if he thinks he's stuck.  If he decides none
of them are working, he gives up, stops and sulks/cries.

# Improvements

 * Right now quickshonk often stops and thinks quite hard, probably owing to a
   bug in the code that averages many readings when they seem to be varying
   wildly.  He has particular problems with approaching surfaces at 45&deg; to
   his direction of motion.  Both might be improved upon by:
    * using [a better ultrasonic library](https://code.google.com/p/arduino-new-ping/)
    * a capacitor close to the module to suppress noise
    * switching to IR for proximity (which might also let quickshonk swarm
      with shonkbot)
 * Couldn't be too hard to adapt quickshonk's higher-level calls to match
   shonkbot's.
 * On the only unit built, the left/right motor speeds vary considerably (maybe
   hot glue got in one gearhead?) and existing calibration for that is either
   broken or insufficient.  This leaves quickshonk with a strong pull to the
   left.
