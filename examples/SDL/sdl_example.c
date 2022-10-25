#if defined(WIN32)
#pragma comment(lib, "winmm.lib")
#include <Windows.h>
#include <mmeapi.h>
#elif defined(__APPLE__)
#include <AudioToolbox/AudioToolbox.h>
#include <CoreFoundation/CoreFoundation.h>
#endif
#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#if 1 // Use the PureDOOM.h single header
#define DOOM_IMPLEMENTATION
#include "../../PureDOOM.h"
#else
#include "DOOM/DOOM.h"
#endif


// Palette experiments
//#define PICO8 1
//#define CGA 1
//#define GAME_BOY 1


// Resolution DOOM renders at
#define WIDTH 320
#define HEIGHT 200
#define SCALE 4

// Resolution of the SDL window
#define FULL_WIDTH (WIDTH * SCALE)
#define FULL_HEIGHT (int)(HEIGHT * 1.2 * SCALE) // 1.2x higher than DOOM's height. Original game was designed stretched


doom_key_t sdl_scancode_to_doom_key(SDL_Scancode scancode)
{
    switch (scancode)
    {
        case SDL_SCANCODE_TAB: return DOOM_KEY_TAB;
        case SDL_SCANCODE_RETURN: return DOOM_KEY_ENTER;
        case SDL_SCANCODE_ESCAPE: return DOOM_KEY_ESCAPE;
        case SDL_SCANCODE_SPACE: return DOOM_KEY_SPACE;
        case SDL_SCANCODE_APOSTROPHE: return DOOM_KEY_APOSTROPHE;
        case SDL_SCANCODE_KP_MULTIPLY: return DOOM_KEY_MULTIPLY;
        case SDL_SCANCODE_COMMA: return DOOM_KEY_COMMA;
        case SDL_SCANCODE_MINUS: return DOOM_KEY_MINUS;
        case SDL_SCANCODE_PERIOD: return DOOM_KEY_PERIOD;
        case SDL_SCANCODE_SLASH: return DOOM_KEY_SLASH;
        case SDL_SCANCODE_0: return DOOM_KEY_0;
        case SDL_SCANCODE_1: return DOOM_KEY_1;
        case SDL_SCANCODE_2: return DOOM_KEY_2;
        case SDL_SCANCODE_3: return DOOM_KEY_3;
        case SDL_SCANCODE_4: return DOOM_KEY_4;
        case SDL_SCANCODE_5: return DOOM_KEY_5;
        case SDL_SCANCODE_6: return DOOM_KEY_6;
        case SDL_SCANCODE_7: return DOOM_KEY_7;
        case SDL_SCANCODE_8: return DOOM_KEY_8;
        case SDL_SCANCODE_9: return DOOM_KEY_9;
        case SDL_SCANCODE_SEMICOLON: return DOOM_KEY_SEMICOLON;
        case SDL_SCANCODE_EQUALS: return DOOM_KEY_EQUALS;
        case SDL_SCANCODE_LEFTBRACKET: return DOOM_KEY_LEFT_BRACKET;
        case SDL_SCANCODE_RIGHTBRACKET: return DOOM_KEY_RIGHT_BRACKET;
        case SDL_SCANCODE_A: return DOOM_KEY_A;
        case SDL_SCANCODE_B: return DOOM_KEY_B;
        case SDL_SCANCODE_C: return DOOM_KEY_C;
        case SDL_SCANCODE_D: return DOOM_KEY_D;
        case SDL_SCANCODE_E: return DOOM_KEY_E;
        case SDL_SCANCODE_F: return DOOM_KEY_F;
        case SDL_SCANCODE_G: return DOOM_KEY_G;
        case SDL_SCANCODE_H: return DOOM_KEY_H;
        case SDL_SCANCODE_I: return DOOM_KEY_I;
        case SDL_SCANCODE_J: return DOOM_KEY_J;
        case SDL_SCANCODE_K: return DOOM_KEY_K;
        case SDL_SCANCODE_L: return DOOM_KEY_L;
        case SDL_SCANCODE_M: return DOOM_KEY_M;
        case SDL_SCANCODE_N: return DOOM_KEY_N;
        case SDL_SCANCODE_O: return DOOM_KEY_O;
        case SDL_SCANCODE_P: return DOOM_KEY_P;
        case SDL_SCANCODE_Q: return DOOM_KEY_Q;
        case SDL_SCANCODE_R: return DOOM_KEY_R;
        case SDL_SCANCODE_S: return DOOM_KEY_S;
        case SDL_SCANCODE_T: return DOOM_KEY_T;
        case SDL_SCANCODE_U: return DOOM_KEY_U;
        case SDL_SCANCODE_V: return DOOM_KEY_V;
        case SDL_SCANCODE_W: return DOOM_KEY_W;
        case SDL_SCANCODE_X: return DOOM_KEY_X;
        case SDL_SCANCODE_Y: return DOOM_KEY_Y;
        case SDL_SCANCODE_Z: return DOOM_KEY_Z;
        case SDL_SCANCODE_BACKSPACE: return DOOM_KEY_BACKSPACE;
        case SDL_SCANCODE_LCTRL:
        case SDL_SCANCODE_RCTRL: return DOOM_KEY_CTRL;
        case SDL_SCANCODE_LEFT: return DOOM_KEY_LEFT_ARROW;
        case SDL_SCANCODE_UP: return DOOM_KEY_UP_ARROW;
        case SDL_SCANCODE_RIGHT: return DOOM_KEY_RIGHT_ARROW;
        case SDL_SCANCODE_DOWN: return DOOM_KEY_DOWN_ARROW;
        case SDL_SCANCODE_LSHIFT:
        case SDL_SCANCODE_RSHIFT: return DOOM_KEY_SHIFT;
        case SDL_SCANCODE_LALT:
        case SDL_SCANCODE_RALT: return DOOM_KEY_ALT;
        case SDL_SCANCODE_F1: return DOOM_KEY_F1;
        case SDL_SCANCODE_F2: return DOOM_KEY_F2;
        case SDL_SCANCODE_F3: return DOOM_KEY_F3;
        case SDL_SCANCODE_F4: return DOOM_KEY_F4;
        case SDL_SCANCODE_F5: return DOOM_KEY_F5;
        case SDL_SCANCODE_F6: return DOOM_KEY_F6;
        case SDL_SCANCODE_F7: return DOOM_KEY_F7;
        case SDL_SCANCODE_F8: return DOOM_KEY_F8;
        case SDL_SCANCODE_F9: return DOOM_KEY_F9;
        case SDL_SCANCODE_F10: return DOOM_KEY_F10;
        case SDL_SCANCODE_F11: return DOOM_KEY_F11;
        case SDL_SCANCODE_F12: return DOOM_KEY_F12;
        case SDL_SCANCODE_PAUSE: return DOOM_KEY_PAUSE;
        default: return DOOM_KEY_UNKNOWN;
    }
    return DOOM_KEY_UNKNOWN;
}


doom_button_t sdl_button_to_doom_button(Uint8 sdl_button)
{
    switch (sdl_button)
    {
        case SDL_BUTTON_LEFT: return DOOM_LEFT_BUTTON;
        case SDL_BUTTON_RIGHT: return DOOM_RIGHT_BUTTON;
        case SDL_BUTTON_MIDDLE: return DOOM_MIDDLE_BUTTON;
    }
    return (doom_button_t)0;
}


void audio_callback(void* userdata, Uint8* stream, int len)
{
    SDL_LockAudio();
    int16_t* buffer = doom_get_sound_buffer();
    SDL_UnlockAudio();

    memcpy(stream, buffer, len);
}


#if defined(WIN32)
static HMIDIOUT midi_out_handle = 0;
void send_midi_msg(uint32_t midi_msg)
{
    if (midi_out_handle)
    {
        midiOutShortMsg(midi_out_handle, midi_msg);
    }
}
#elif defined(__APPLE__)
AudioUnit audio_unit = 0;
void send_midi_msg(uint32_t midi_msg)
{
    if (audio_unit)
    {
        MusicDeviceMIDIEvent(audio_unit, 
            (midi_msg) & 0xFF,
            (midi_msg >> 8) & 0xFF,
            (midi_msg >> 16) & 0xFF,
            0);
    }
}
#else
void send_midi_msg(uint32_t midi_msg) {}
#endif


SDL_TimerID midi_timer = 0;
Uint32 tick_midi(Uint32 interval, void *param)
{
    uint32_t midi_msg;

    SDL_LockAudio();
    while ((midi_msg = doom_tick_midi()) != 0) send_midi_msg(midi_msg);
    SDL_UnlockAudio();

#if defined(__APPLE__)
    return 1000 / DOOM_MIDI_RATE - 1; // Weirdly, on Apple music is too slow
#else
    return 1000 / DOOM_MIDI_RATE;
#endif
}


int main(int argc, char** argv)
{
    // Init SDL
    SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_TIMER);
    SDL_Window* window = SDL_CreateWindow("Pure DOOM - SDL Example", 
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                                          FULL_WIDTH, FULL_HEIGHT, 
                                          SDL_WINDOW_RESIZABLE);
    
    // SDL Renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
    SDL_Texture* render_target = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);

    // SDL Audio thread
    SDL_AudioSpec audio_spec;
    memset(&audio_spec, 0, sizeof(audio_spec));
    audio_spec.freq = DOOM_SAMPLERATE;
    audio_spec.format = AUDIO_S16;
    audio_spec.channels = 2;
    audio_spec.samples = 512;
    audio_spec.callback = audio_callback;

    if (SDL_OpenAudio(&audio_spec, NULL) < 0)
    {
        printf("Failed to SDL_OpenAudio\n");
        return 1;
    }

    // Capture mouse
    SDL_SetRelativeMouseMode(SDL_TRUE);

    //-----------------------------------------------------------------------
    // Setup DOOM
    //-----------------------------------------------------------------------

    // Change default bindings to modern
    doom_set_default_int("key_up", DOOM_KEY_W);
    doom_set_default_int("key_down", DOOM_KEY_S);
    doom_set_default_int("key_strafeleft", DOOM_KEY_A);
    doom_set_default_int("key_straferight", DOOM_KEY_D);
    doom_set_default_int("key_use", DOOM_KEY_E);
    doom_set_default_int("mouse_move", 0); // Mouse will not move forward

    // Setup resolution
    doom_set_resolution(WIDTH, HEIGHT);

    // Setup MIDI for songs
#if defined(WIN32)
    if (midiOutGetNumDevs() != 0)
        midiOutOpen(&midi_out_handle, 0, 0, 0, 0);
#elif defined(__APPLE__)
    // I don't really understand this part, but it works.
	AUGraph graph;
	AUNode outputNode, mixerNode, dlsNode;
	NewAUGraph(&graph);
	AudioComponentDescription output = {'auou','ahal','appl',0,0};
	AUGraphAddNode(graph, &output, &outputNode);
	AUGraphOpen(graph);
	AUGraphInitialize(graph);
	AUGraphStart(graph);
	AudioComponentDescription dls = {'aumu','dls ','appl',0,0};
	AUGraphAddNode(graph, &dls, &dlsNode);
	AUGraphNodeInfo(graph, dlsNode, NULL, &audio_unit);
	AudioComponentDescription mixer = {'aumx','smxr','appl',0,0};
	AUGraphAddNode(graph, &mixer, &mixerNode);
	AUGraphConnectNodeInput(graph,mixerNode,0,outputNode,0);
	AUGraphConnectNodeInput(graph,dlsNode,0,mixerNode,0);
	AUGraphUpdate(graph,NULL);
#endif
    midi_timer = SDL_AddTimer(0, tick_midi, 0);

    // Initialize doom
    doom_init(argc, argv, DOOM_FLAG_MENU_DARKEN_BG);

    //-----------------------------------------------------------------------
    
    // Main loop
    SDL_PauseAudio(0); 
    int done = 0;
    int active_mouse = 1; // Dev allow us to take mouse out of window
    while (!done)
    {
        SDL_Event e;
        Sint32 mouse_motion_x = 0;
        Sint32 mouse_motion_y = 0;
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
                case SDL_QUIT:
                    done = 1;
                    break;

                case SDL_KEYDOWN:
                    if (e.key.keysym.scancode == SDL_SCANCODE_END)
                    {
                        active_mouse = !active_mouse;
                        SDL_SetRelativeMouseMode(active_mouse ? SDL_TRUE : SDL_FALSE);
                    }

                    if (!e.key.repeat)
                        doom_key_down(sdl_scancode_to_doom_key(e.key.keysym.scancode));
                    break;

                case SDL_KEYUP:
                    if (!e.key.repeat)
                        doom_key_up(sdl_scancode_to_doom_key(e.key.keysym.scancode));
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    if (active_mouse) doom_button_down(sdl_button_to_doom_button(e.button.button));
                    break;

                case SDL_MOUSEBUTTONUP:
                    if (active_mouse) doom_button_up(sdl_button_to_doom_button(e.button.button));
                    break;

                case SDL_MOUSEMOTION:
                    if (active_mouse)
                    {
                        mouse_motion_x += e.motion.xrel;
                        mouse_motion_y += e.motion.yrel;
                    }
                    break;
            }
            if (done) break;
        }
        if (done) break;

        if (mouse_motion_x || mouse_motion_y)
            doom_mouse_move(mouse_motion_x * 4, mouse_motion_y * 4);
        
        SDL_LockAudio();
        doom_update();
        SDL_UnlockAudio();

        // Blit DOOM's framebuffer onto our SDL texture
#if GAME_BOY
        void* dst;
        const unsigned char* src = doom_get_framebuffer(3);
        int src_pitch = WIDTH * 3;
        int dst_pitch;
        const int palette_size = 4;
        const int pico8_palette[4 * 3] = {
            0x0f, 0x3, 0x80f,
            0x30, 0x62, 0x30,
            0x8b, 0xac, 0x0f,
            0x9b, 0xbc, 0x0f
        };
        if (!SDL_LockTexture(render_target, NULL, &dst, &dst_pitch))
        {
            unsigned char* dst8 = (unsigned char*)dst;
            for (int y = 0; y < HEIGHT; ++y)
            {
                for (int x = 0; x < WIDTH; ++x)
                {
                    int dstk = y * dst_pitch + x * 4;
                    int srck = y * src_pitch + x * 3;

                    int r = src[srck + 0];
                    int g = src[srck + 1];
                    int b = src[srck + 2];

                    int best = 0;
                    int best_score = (r * r + g * g + b * b) * 2;
                    for (int p = 0; p < palette_size * 3; p += 3)
                    {
                        const int* pico8_color = pico8_palette + p;
                        if (pico8_color[0] == r &&
                            pico8_color[1] == g &&
                            pico8_color[2] == b)
                        {
                            // Perfect match
                            best = p;
                            break;
                        }
                        int ri = pico8_color[0] - r;
                        int gi = pico8_color[1] - g;
                        int bi = pico8_color[2] - b;
                        int score = ri * ri + gi * gi + bi * bi;
                        if (score < best_score)
                        {
                            best_score = score;
                            best = p;
                        }
                    }

                    dst8[dstk + 0] = pico8_palette[best + 0];
                    dst8[dstk + 1] = pico8_palette[best + 1];
                    dst8[dstk + 2] = pico8_palette[best + 2];
                    dst8[dstk + 3] = 255;
                }
            }
            SDL_UnlockTexture(render_target);
        }
#elif PICO8 || CGA
        void* dst;
        const unsigned char* src = doom_get_framebuffer(3);
        int src_pitch = WIDTH * 3;
        int dst_pitch;
#if CGA
        const int palette_size = 16;
        const int pico8_palette[16 * 3] = {
            0, 0, 0,
            0, 0, 0xAA,
            0, 0xAA, 0,
            0, 0xAA, 0xAA,
            0xAA, 0, 0,
            0xAA, 0, 0xAA,
            0xAA, 0x55, 0,
            0xAA, 0xAA, 0xAA,
            0x55, 0x55, 0x55,
            0x55, 0x55, 0xFF,
            0x55, 0xFF, 0x55,
            0x55, 0xFF, 0xFF,
            0xFF, 0x55, 0x55,
            0xFF, 0x55, 0xFF,
            0xFF, 0xFF, 0x55,
            0xFF, 0xFF, 0xFF
        };
#elif PICO8
        const int palette_size = 16;
        const int pico8_palette[16 * 3] = {
            0, 0, 0,
            29, 43, 83,
            126, 37, 83,
            0, 135, 81,
            171, 82, 54,
            95, 87, 79,
            194, 195, 199,
            255, 241, 232,
            255, 0, 77,
            255, 163, 0,
            255, 236, 39,
            0, 228, 54,
            41, 173, 255,
            131, 118, 156,
            255, 119, 168,
            255, 204, 170
        };
#endif
        if (!SDL_LockTexture(render_target, NULL, &dst, &dst_pitch))
        {
            unsigned char* dst8 = (unsigned char*)dst;
            for (int y = 0; y < HEIGHT; ++y)
            {
                for (int x = 0; x < WIDTH; ++x)
                {
                    int dstk = y * dst_pitch + x * 4;
                    int srck = y * src_pitch + x * 3;

                    int r = src[srck + 0];
                    int g = src[srck + 1];
                    int b = src[srck + 2];

                    int best = 0;
                    int best_score = (r * r + g * g + b * b) * 2;
                    for (int p = 0; p < palette_size * 3; p += 3)
                    {
                        const int* pico8_color = pico8_palette + p;
                        if (pico8_color[0] == r &&
                            pico8_color[1] == g &&
                            pico8_color[2] == b)
                        {
                            // Perfect match
                            best = p;
                            break;
                        }
                        int ri = pico8_color[0] - r;
                        int gi = pico8_color[1] - g;
                        int bi = pico8_color[2] - b;
                        int score = ri * ri + gi * gi + bi * bi;
                        if (score < best_score)
                        {
                            best_score = score;
                            best = p;
                        }
                    }

                    dst8[dstk + 0] = pico8_palette[best + 0];
                    dst8[dstk + 1] = pico8_palette[best + 1];
                    dst8[dstk + 2] = pico8_palette[best + 2];
                    dst8[dstk + 3] = 255;
                }
            }
            SDL_UnlockTexture(render_target);
        }
#else
        void* dst;
        const unsigned char* src = doom_get_framebuffer(4);
        int src_pitch = WIDTH * 4;
        int dst_pitch;
        if (!SDL_LockTexture(render_target, NULL, &dst, &dst_pitch))
        {
            for (int y = 0; y < HEIGHT; ++y)
            {
                memcpy(dst, src, src_pitch);
                dst = (unsigned char*)dst + dst_pitch;
                src += src_pitch;
            }
            SDL_UnlockTexture(render_target);
        }
#endif

        // Stretch our texture on the screen
        SDL_Rect src_rect = {0, 0, WIDTH, HEIGHT };
        SDL_Rect dst_rect = {0, 0, FULL_WIDTH, FULL_HEIGHT};
        SDL_RenderCopy(renderer, render_target, &src_rect, &dst_rect);

        // Swap
        SDL_RenderPresent(renderer);
    }
    
    // Shutdown
    if (midi_timer) SDL_RemoveTimer(midi_timer);
#if defined(WIN32)
    if (midi_out_handle) midiOutClose(midi_out_handle);
#endif
    SDL_SetRelativeMouseMode(SDL_FALSE);
    SDL_CloseAudio();
    SDL_DestroyTexture(render_target);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
