# ainsuForth-gen-exp-m4
ainsuForth - generalized - all targets including SAMD51 M4 - experimental

+Metro M4 Express     - SAMD51J19A
+ItsyBitsy M4 Express - SAMD51G19A

previously ainsuForth-gen-exp

22 August 2018:

Development for a bare-metal Forth in

   https://github.com/wa1tnr/ainsuMtxd21.git

The most recent branch incorporating what is achieved is found

   On branch KM_converser_aa-

of that repository.

There is also a UART-based version of ainsuForth-gen-exp-m4,
though it hasn't been published.  All that was required was
a global search and replace for 'Serial.' with 'Serial1.' to
get that to work quite well (using CP2104 USART-USB bridge
chip; reference implementation was for Adafruit PiUART - a
nice way to leverage this, quickly, with just two wires (plus
ground, for three wires, total).

12 June 2018: Nothing said below this line is reliable, as it is
a copy-over from an earlier project and hasn't been looked at,
recently.  Ignore (or interpret from past events).

August 2018 update:  Haven't removed what's below, nor reviewed it.

It's so much easier to just be diligent with 'git diff' and
to use 'gitk' to step through the changes (though that is the
developer's point of view; for the end-user, hard to say).

 - - - - - - - -

Suggest: diff against ainsuForth-gen-exp -- which was the best
         SAMD21 M0 effort, to date.


Older notes follow.  No effort is made to update them, or remove
where no longer applicable.

August 2nd:

More experimental -- looking at adding SPI flash write support,
file capture from ASCII upload (the 'download' word) and perhaps
splitting out file i/o to its own proper source files.


EXPERIMENTAL -- the below, plus a limited use of the SPI flash
to author forth source code on the device.  Currently, this
amounts to reading a full program (a multi-line forth source
program) from /forth/job.fs on the SPI flash volume.


Platform: Circuit Playground Express (an M0 board w/SPI 2MB flashROM) - 02 Aug 2017

A Forth for SAMD21G18A based on YAFFA-ARM Forth - 2012 by Stuart Wood.



See compatibility.h for (some) configuration options across
target board devices (subset of boards.txt found under

  .arduino15/packages/adafruit/hardware/samd/1.0.18/boards.txt )


----------------------------------------------------------------

Current platform is the

    Adafruit Metro M4 Express Beta -- 13 May 2018.

was:

    Adafruit Feather M0 Express -- 15 January 2018.

All other platform declarations are incorrect, where they differ
from this one.

----------------------------------------------------------------

OLD:
Current platform is the
    Adafruit CPX (Circuit Playground Express) - PRODUCT ID: 3333
which is ATSAMD21 Cortex M0+ based.
----------------------------------------------------------------

Platform will change frequently -- seven different targets are
tested on a (somewhat) rotating basis:

  * Adafruit Metro M4 Express Beta (SAMD51)
  * Arduino M0 Pro (SAMD21)
  * Adafruit Feather M0 Express
  * Adafruit Metro M0 Express
  * Adafruit Circuit Playground Express
  * Adafruit Gemma M0
  * Adafruit Trinket M0

Sun 13 May 00:34:29 UTC 2018

28f54e6bc7

old:

# YAFFA-ARM
YAFFA for Arduino ARM Processors 

See:
 [ https://github.com/sdwood68/YAFFA-ARM ]

