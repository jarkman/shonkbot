# Shonkbot's control console

There are two modes of operation.

 * **queuemode**: follow (or resume following) queued movements and turn on
   collision avoidance.  This is the default after a reset/power loss.
 * **interactivemode**: stop following any queued movements, and turn off
   collision avoidance.  Shonky will still squeak about obstacles, just not
   avoid them.

The simplest commands move Shonky directly.  It's best to be in
**interactivemode** when sending these.

 * **n iforward**: immediately go forward *n* millimetres; e.g. "100 iforward"
   to go 10cm.  Negative distances make Shonky reverse that far.
 * **n iturn**: immediately turn clockwise *n* degrees; e.g. "45 iturn".
   Negative angles make Shonky turn anticlockwise by that much.
   You can't turn more than 359 degrees in one go.

Once you've sent a command Shonky will move in the background, still accepting
new commands on the console.  Giving it a new command will override and forget
about the old one.  If you want to wait for it to complete, use **iwait**
before the next command.

 * **iwait**: wait until the most recent immediate command has finished before
   accepting a new one.  Example: "50 iforward iwait 45 iturn" (can be sent all
   on one line).
 * **delay**: freeze for the given number of milliseconds.  Example:
   "2000 delay" does nothing for two seconds.

Shonky can also beep.

 * __*dur* *freq* ibeep__: emit a beep of *freq* Hertz lasting *dur*
   milliseconds.

## Queue-related commands

These build up a queue of up to 40 movements which gets run whenever you switch
to queue mode.  You can add commands while earlier ones are running, but that
gets confusing, so it's easiest to add movements while in interactive mode.

Shonky starts with a simple random pattern already queued up, and loops around
following it endlessly unless it sees an obstacle.

 * **clearq**: Wipe out any existing queued commands.
 * **listq**: Show the command queue.
 * **n qforward**: like **iforward** but queues the motion rather than doing
   it immediately.  Unlike **iforward**, queued forward motion can't go
   backwards.
 * **n qturn**: like **iturn** but queues the turn for later.
 * **Beeps cannot be queued.**
 * **loopq**: A variable that lets you switch between following the queued
   movements once or following them endlessly.  By default, looping is on.
   To switch it off, send "0 loopq !".  To switch it on, send "1 loopq !".
   To examine it, send "loopq @".
 * **randomq**: Populate the queue with a random "curvahedron" like Shonky
   follows when it first starts up.

To wipe Shonky's default queue, then queue up a triangle, then follow it:

    interactivemode
    clearq
    50 qforward
    120 qturn
    50 qforward
    120 qturn
    50 qforward
    120 qturn
    queuemode

## Compound commands

You can send multiple commands on one line; Shonky doesn't care whether you
separate commands with a space or by pressing enter.

You can also create new commands in terms of a list of existing ones
(although Shonky will forget them if you reset it).  Create a new one with:

 * A colon (:).
 * The name of the new command.  The name can't contain spaces.
 * The list of existing commands.  You can split this across lines or put it
   all on one line.  If you split it, until you end the list Shonky will print
   a pipe (|) at the start of each new line to signal that it expects the list
   to end eventually.
 * A semicolon (;) to mark the end of the list.

So to define a "catwalk" command that walks along a few centimetres, does
a spin, waits a bit, and heads back, send:

    : catwalk
      50 iforward iwait
      359 iturn iwait
      1000 delay
      180 iturn iwait
      50 iforward iwait
    ;

Now if you send "catwalk", Shonky will strut up and down.  You could define
an alphabet this way and spell out words.

## Getting clever

Shonkbot's console is slightly less rudimentary than it appears.  Underneath,
it's actually a tiny programming language called Forth.  Some commands that
might be handy for more complex tasks:

 * **getrange**: get the (rough!) distance to a detected obstacle in cm.
   0 means no obstacle has been detected.  Maybe Shonky could stay still and
   work out whether the thing it's seeing is another bot.
 * **getx**, **gety**, **getheading**: Shonky does dead reckoning and
   estimates its location and heading relative to where it started/where it
   was pointing at power-on.

