![](images/PureDOOM.png)

# Pure DOOM
Header only, no dependencies DOOM source port. Designed to run on any devices.

Primarily of interest to people that want to "run DOOM on their microwave".

## LICENSE
See end of file for license information.

## Main Features:
- Single header
- Pure-C, No includes dependencies: no stdlib, stdio, etc.
- Supports 32 bits and 64 bits

## Other Features:
- Menu option to disable mouse move Forward/Backward
- Crosshair option
- Always-Run option

## TODOs
- Custom resolution
- Remove exit and have update return 0 on quit. Add doom_get_exit_code()
- Implement Sockets and Multiplayer

## Cool TODOs to have
- Rebindable keys
- Unlocked FPS
- Release mouse in menus and use it for clicking
- Wipe screen freezes menu
- Use floats instead of fixed_t
- French and german
- Full color mode (Don't use COLORMAPS, use full 24 bits RGB)

## Usage
Call `doom_init()`, then call `doom_update()` every frame, or as often as you can. This will run DOOM, without any video, inputs, sounds or music.

```c
#define DOOM_IMPLEMENTATION 
#include "PureDOOM.h"

int main(int argc, char** argv)
{
    doom_init(argc, argv, 0);
    while (true)
    {
        doom_update();
    }
}
```

## Enable features
Most standard headers are available on most platforms. Define these preprocessors to toggle these features.

- `DOOM_IMPLEMENT_PRINT`. Allows printf, requires `<stdio.h>`
- `DOOM_IMPLEMENT_MALLOC`. Allows malloc/free, requires `<stdlib.h>`
- `DOOM_IMPLEMENT_FILE_IO`. Allows FILE, requires `<stdio.h>`
- `DOOM_IMPLEMENT_GETTIME`. Requires `<sys/time.h>` or `<winsock.h>`
- `DOOM_IMPLEMENT_EXIT`. Allows exit(), requires `<stdlib.h>`
- `DOOM_IMPLEMENT_GETENV`. Requires `<stdlib.h>`

If your microwave doesn't have these headers, you can override their default implementations:

```c
void doom_set_print(doom_print_fn print_fn);
void doom_set_malloc(doom_malloc_fn malloc_fn, doom_free_fn free_fn);
void doom_set_file_io(doom_open_fn open_fn,
                      doom_close_fn close_fn,
                      doom_read_fn read_fn,
                      doom_write_fn write_fn,
                      doom_seek_fn seek_fn,
                      doom_tell_fn tell_fn,
                      doom_eof_fn eof_fn);
void doom_set_gettime(doom_gettime_fn gettime_fn);
void doom_set_exit(doom_exit_fn exit_fn);
void doom_set_getenv(doom_getenv_fn getenv_fn);
```

## Video
Every frame, after having called `doom_update()`, you can get the screen pixels with `doom_get_framebuffer` and display it as you please.

```c
while (true)
{
    doom_update();
    uint8_t* framebuffer = doom_get_framebuffer(4 /* RGBA */);
    // ... Display framebuffer
}
```

## Inputs
When you receive input events from your microwave touch pad, simply call one of the DOOM input events:

```c
void doom_key_down(doom_key_t key);
void doom_key_up(doom_key_t key);
void doom_button_down(doom_button_t button);
void doom_button_up(doom_button_t button);
void doom_mouse_move(int delta_x, int delta_y);
```

## Sounds
Create a sound thread that outputs at 11025hz (`DOOM_SAMPLERATE`), 512 samples, 16 bits, stereo. Then in your sound callback, call `doom_get_sound_buffer` to update and get the current DOOM's sound output. Make sure to add synchronization primitives around this and `doom_update` if your sound loop is in a thread.

Here is a quick example using SDL audio callback:
```c
void sdl_audio_callback(void* userdata, Uint8* stream, int len)
{
    SDL_LockAudio();
    int16_t* buffer = doom_get_sound_buffer(len);
    SDL_UnlockAudio();

    memcpy(stream, buffer, len);
}
```

You can use different bitrate, but make sure to resample because DOOM will always be 11025hz, 512 samples, 16 bits, 2 channels. For a total for 2048 bytes per buffer.

## Music
Set a timer in your application that runs at 140hz. In the timer's callback, tick DOOM's music as long as there are MIDI messages to send.

Here is an example using Window's MultiMedia to play MIDI events, using an SDL timer.
```c
Uint32 tick_music(Uint32 interval, void *param)
{
    uint32_t midi_msg;

    SDL_LockAudio();

    while (midi_msg = doom_tick_midi())
        midiOutShortMsg(midi_out_handle, midi_msg);

    SDL_UnlockAudio();

    return 1000 / DOOM_MIDI_RATE /* 140 */;
}
```

## Change default settings
Default input setup in DOOM's source is not modern. It uses Arrows to move and `','` / `'.'` to strafe. You can call `doom_set_default_int` and `doom_set_default_str` to change them:

```c
// Change default bindings to modern mapping
doom_set_default_int("key_up",          DOOM_KEY_W);
doom_set_default_int("key_down",        DOOM_KEY_S);
doom_set_default_int("key_strafeleft",  DOOM_KEY_A);
doom_set_default_int("key_straferight", DOOM_KEY_D);
doom_set_default_int("key_use",         DOOM_KEY_E);
doom_set_default_int("mouse_move",      0); // Mouse will not move forward
```

Refer to the defaults in `m_misc.cpp` for the complete list.

## Working SDL Example
See the file `src/sdl_example.c` for a complete SDL example.

# DOOM LICENSE
```
      LIMITED USE SOFTWARE LICENSE AGREEMENT

        This Limited Use Software License Agreement (the "Agreement")
is a legal agreement between you, the end-user, and Id Software, Inc.
("ID").  By downloading or purchasing the software material, which
includes source code (the "Source Code"), artwork data, music and
software tools (collectively, the "Software"), you are agreeing to
be bound by the terms of this Agreement.  If you do not agree to the
terms of this Agreement, promptly destroy the Software you may have
downloaded or copied.

ID SOFTWARE LICENSE

1.      Grant of License.  ID grants to you the right to use the
Software.  You have no ownership or proprietary rights in or to the 
Software, or the Trademark. For purposes of this section, "use" means 
loading the Software into RAM, as well as installation on a hard disk
or other storage device. The Software, together with any archive copy
thereof, shall be destroyed when no longer used in accordance with 
this Agreement, or when the right to use the Software is terminated.  
You agree that the Software will not be shipped, transferred or 
exported into any country in violation of the U.S. Export 
Administration Act (or any other law governing such matters) and that 
you will not utilize, in any other manner, the Software in violation 
of any applicable law.

2.      Permitted Uses.  For educational purposes only, you, the
end-user, may use portions of the Source Code, such as particular
routines, to develop your own software, but may not duplicate the
Source Code, except as noted in paragraph 4.  The limited right
referenced in the preceding sentence is hereinafter referred to as
"Educational Use."  By so exercising the Educational Use right you
shall not obtain any ownership, copyright, proprietary or other
interest in or to the Source Code, or any portion of the Source
Code.  You may dispose of your own software in your sole discretion.
With the exception of the Educational Use right, you may not
otherwise use the Software, or an portion of the Software, which
includes the Source Code, for commercial gain.

3.      Prohibited Uses:  Under no circumstances shall you, the
end-user, be permitted, allowed or authorized to commercially exploit
the Software. Neither you nor anyone at your direction shall do any
of the following acts with regard to the Software, or any portion
thereof:

        Rent;

        Sell;

        Lease;

        Offer on a pay-per-play basis;

        Distribute for money or any other consideration; or

        In any other manner and through any medium whatsoever
commercially exploit or use for any commercial purpose.

Notwithstanding the foregoing prohibitions, you may commercially
exploit the software you develop by exercising the Educational Use 
right, referenced in paragraph 2. hereinabove.

4.      Copyright.  The Software and all copyrights related thereto 
(including all characters and other images generated by the Software
or depicted in the Software) are owned by ID and is protected by
United States  copyright laws and international treaty provisions.  
Id shall retain exclusive ownership and copyright in and to the
Software and all portions of the Software and you shall have no 
ownership or other proprietary interest in such materials. You must
treat the Software like any other copyrighted material. You may not
otherwise reproduce, copy or disclose to others, in whole or in any
part, the Software.  You may not copy the written materials
accompanying the Software.  You agree to use your best efforts to
see that any user of the Software licensed hereunder complies with
this Agreement.

5.      NO WARRANTIES.  ID DISCLAIMS ALL WARRANTIES, BOTH EXPRESS
IMPLIED, INCLUDING BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE WITH RESPECT
TO THE SOFTWARE.  THIS LIMITED WARRANTY GIVES YOU SPECIFIC LEGAL
RIGHTS.  YOU MAY HAVE OTHER RIGHTS WHICH VARY FROM JURISDICTION TO
JURISDICTION.  ID DOES NOT WARRANT THAT THE OPERATION OF THE SOFTWARE
WILL BE UNINTERRUPTED, ERROR FREE OR MEET YOUR SPECIFIC REQUIREMENTS.
THE WARRANTY SET FORTH ABOVE IS IN LIEU OF ALL OTHER EXPRESS
WARRANTIES WHETHER ORAL OR WRITTEN.  THE AGENTS, EMPLOYEES, 
DISTRIBUTORS, AND DEALERS OF ID ARE NOT AUTHORIZED TO MAKE 
MODIFICATIONS TO THIS WARRANTY, OR ADDITIONAL WARRANTIES ON BEHALF
OF ID. 

        Exclusive Remedies.  The Software is being offered to you
free of any charge.  You agree that you have no remedy against ID, its
affiliates, contractors, suppliers, and agents for loss or damage 
caused by any defect or failure in the Software regardless of the form
of action, whether in contract, tort, includinegligence, strict
liability or otherwise, with regard to the Software.  This Agreement
shall be construed in accordance with and governed by the laws of the
State of Texas.  Copyright and other proprietary matters will be
governed by United States laws and international treaties.  IN ANY 
CASE, ID SHALL NOT BE LIABLE FOR LOSS OF DATA, LOSS OF PROFITS, LOST
SAVINGS, SPECIAL, INCIDENTAL, CONSEQUENTIAL, INDIRECT OR OTHER
SIMILAR DAMAGES ARISING FROM BREACH OF WARRANTY, BREACH OF CONTRACT,
NEGLIGENCE, OR OTHER LEGAL THEORY EVEN IF ID OR ITS AGENT HAS BEEN
ADVISED OF THE POSSIBILITY OF SUCH DAMAGES, OR FOR ANY CLAIM BY ANY
OTHER PARTY. Some jurisdictions do not allow the exclusion or
limitation of incidental or consequential damages, so the above
limitation or exclusion may not apply to you.
```
