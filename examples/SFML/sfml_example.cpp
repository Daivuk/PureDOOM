#if defined(WIN32)
#pragma comment(lib, "winmm.lib")
#include <Windows.h>
#include <mmeapi.h>
#elif defined(__APPLE__)
#include <AudioToolbox/AudioToolbox.h>
#include <CoreFoundation/CoreFoundation.h>
#endif
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <chrono>
#include <mutex>
#include <thread>

#if 0 // Use the PureDOOM.h single header
#define DOOM_IMPLEMENTATION
#include "../../PureDOOM.h"
#else
#include "DOOM/DOOM.h"
#endif


// Resolution DOOM renders at
#define WIDTH 320
#define HEIGHT 200
#define SCALE 4

// Resolution of the SDL window
#define FULL_WIDTH (WIDTH * SCALE)
#define FULL_HEIGHT (int)(HEIGHT * 1.2 * SCALE) // 1.2x higher than DOOM's height. Original game was designed stretched


std::mutex audio_mutex;


class AudioStream : public sf::SoundStream
{
public:
    AudioStream()
    {
        initialize(2, DOOM_SAMPLERATE);
    }

    bool onGetData(Chunk& data) override
    {
        std::unique_lock<std::mutex> lock(audio_mutex);

        data.sampleCount = 512 * 2;
        data.samples = doom_get_sound_buffer();

        return true;
    }

    void onSeek(sf::Time timeOffset) override {} // Ignore this
};


sf::RenderWindow* window = nullptr;
sf::Texture* texture = nullptr;
sf::Sprite* sprite = nullptr;
AudioStream* stream = nullptr;
bool quit_from_update = false;
std::thread* music_thread = nullptr;
bool music_playing = true;


doom_key_t sfml_scancode_to_doom_key(sf::Keyboard::Key code)
{
    switch (code)
    {
        case sf::Keyboard::Key::Tab: return DOOM_KEY_TAB;
        case sf::Keyboard::Key::Enter: return DOOM_KEY_ENTER;
        case sf::Keyboard::Key::Escape: return DOOM_KEY_ESCAPE;
        case sf::Keyboard::Key::Space: return DOOM_KEY_SPACE;
        case sf::Keyboard::Key::Quote: return DOOM_KEY_APOSTROPHE;
        case sf::Keyboard::Key::Multiply: return DOOM_KEY_MULTIPLY;
        case sf::Keyboard::Key::Comma: return DOOM_KEY_COMMA;
        case sf::Keyboard::Key::Hyphen: return DOOM_KEY_MINUS;
        case sf::Keyboard::Key::Period: return DOOM_KEY_PERIOD;
        case sf::Keyboard::Key::Slash: return DOOM_KEY_SLASH;
        case sf::Keyboard::Key::Num0: return DOOM_KEY_0;
        case sf::Keyboard::Key::Num1: return DOOM_KEY_1;
        case sf::Keyboard::Key::Num2: return DOOM_KEY_2;
        case sf::Keyboard::Key::Num3: return DOOM_KEY_3;
        case sf::Keyboard::Key::Num4: return DOOM_KEY_4;
        case sf::Keyboard::Key::Num5: return DOOM_KEY_5;
        case sf::Keyboard::Key::Num6: return DOOM_KEY_6;
        case sf::Keyboard::Key::Num7: return DOOM_KEY_7;
        case sf::Keyboard::Key::Num8: return DOOM_KEY_8;
        case sf::Keyboard::Key::Num9: return DOOM_KEY_9;
        case sf::Keyboard::Key::Semicolon: return DOOM_KEY_SEMICOLON;
        case sf::Keyboard::Key::Equal: return DOOM_KEY_EQUALS;
        case sf::Keyboard::Key::LBracket: return DOOM_KEY_LEFT_BRACKET;
        case sf::Keyboard::Key::RBracket: return DOOM_KEY_RIGHT_BRACKET;
        case sf::Keyboard::Key::A: return DOOM_KEY_A;
        case sf::Keyboard::Key::B: return DOOM_KEY_B;
        case sf::Keyboard::Key::C: return DOOM_KEY_C;
        case sf::Keyboard::Key::D: return DOOM_KEY_D;
        case sf::Keyboard::Key::E: return DOOM_KEY_E;
        case sf::Keyboard::Key::F: return DOOM_KEY_F;
        case sf::Keyboard::Key::G: return DOOM_KEY_G;
        case sf::Keyboard::Key::H: return DOOM_KEY_H;
        case sf::Keyboard::Key::I: return DOOM_KEY_I;
        case sf::Keyboard::Key::J: return DOOM_KEY_J;
        case sf::Keyboard::Key::K: return DOOM_KEY_K;
        case sf::Keyboard::Key::L: return DOOM_KEY_L;
        case sf::Keyboard::Key::M: return DOOM_KEY_M;
        case sf::Keyboard::Key::N: return DOOM_KEY_N;
        case sf::Keyboard::Key::O: return DOOM_KEY_O;
        case sf::Keyboard::Key::P: return DOOM_KEY_P;
        case sf::Keyboard::Key::Q: return DOOM_KEY_Q;
        case sf::Keyboard::Key::R: return DOOM_KEY_R;
        case sf::Keyboard::Key::S: return DOOM_KEY_S;
        case sf::Keyboard::Key::T: return DOOM_KEY_T;
        case sf::Keyboard::Key::U: return DOOM_KEY_U;
        case sf::Keyboard::Key::V: return DOOM_KEY_V;
        case sf::Keyboard::Key::W: return DOOM_KEY_W;
        case sf::Keyboard::Key::X: return DOOM_KEY_X;
        case sf::Keyboard::Key::Y: return DOOM_KEY_Y;
        case sf::Keyboard::Key::Z: return DOOM_KEY_Z;
        case sf::Keyboard::Key::Backspace: return DOOM_KEY_BACKSPACE;
        case sf::Keyboard::Key::LControl:
        case sf::Keyboard::Key::RControl: return DOOM_KEY_CTRL;
        case sf::Keyboard::Key::Left: return DOOM_KEY_LEFT_ARROW;
        case sf::Keyboard::Key::Up: return DOOM_KEY_UP_ARROW;
        case sf::Keyboard::Key::Right: return DOOM_KEY_RIGHT_ARROW;
        case sf::Keyboard::Key::Down: return DOOM_KEY_DOWN_ARROW;
        case sf::Keyboard::Key::LShift:
        case sf::Keyboard::Key::RShift: return DOOM_KEY_SHIFT;
        case sf::Keyboard::Key::LAlt:
        case sf::Keyboard::Key::RAlt: return DOOM_KEY_ALT;
        case sf::Keyboard::Key::F1: return DOOM_KEY_F1;
        case sf::Keyboard::Key::F2: return DOOM_KEY_F2;
        case sf::Keyboard::Key::F3: return DOOM_KEY_F3;
        case sf::Keyboard::Key::F4: return DOOM_KEY_F4;
        case sf::Keyboard::Key::F5: return DOOM_KEY_F5;
        case sf::Keyboard::Key::F6: return DOOM_KEY_F6;
        case sf::Keyboard::Key::F7: return DOOM_KEY_F7;
        case sf::Keyboard::Key::F8: return DOOM_KEY_F8;
        case sf::Keyboard::Key::F9: return DOOM_KEY_F9;
        case sf::Keyboard::Key::F10: return DOOM_KEY_F10;
        case sf::Keyboard::Key::F11: return DOOM_KEY_F11;
        case sf::Keyboard::Key::F12: return DOOM_KEY_F12;
        case sf::Keyboard::Key::Pause: return DOOM_KEY_PAUSE;
        default: return DOOM_KEY_UNKNOWN;
    }
    return DOOM_KEY_UNKNOWN;
}


doom_button_t sfml_button_to_doom_button(sf::Mouse::Button sfml_button)
{
    switch (sfml_button)
    {
        case sf::Mouse::Button::Left: return DOOM_LEFT_BUTTON;
        case sf::Mouse::Button::Right: return DOOM_RIGHT_BUTTON;
        case sf::Mouse::Button::Middle: return DOOM_MIDDLE_BUTTON;
    }
    return (doom_button_t)0;
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


std::chrono::high_resolution_clock::time_point now()
{
    return std::chrono::high_resolution_clock::now();
}


void music_loop()
{
    uint32_t midi_msg;
    auto prev = now();

    while (true)
    {
        {
            std::unique_lock<std::mutex> lock(audio_mutex);
            if (!music_playing)
                break;

            while ((midi_msg = doom_tick_midi()) != 0) send_midi_msg(midi_msg);
        }

        // This is CPU expensive. But SFML doesn't have SDL's equivalent for SDL_AddTimer. Got lazy and didn't implement the equivalent. Just an example..
        while (now() - prev < std::chrono::microseconds(1000000) / DOOM_MIDI_RATE);
        prev = now();
    }
}


void quit(int code)
{
    // Stop music and audio
    music_playing = false;
    if (quit_from_update) audio_mutex.unlock();
    music_thread->join();
    delete music_thread;
    stream->stop();
    delete stream;
#if defined(WIN32)
    if (midi_out_handle) midiOutClose(midi_out_handle);
#endif

    // Restore mouse
    window->setMouseCursorGrabbed(false);
    window->setMouseCursorVisible(true);

    // Destroy main SFML objects
    delete sprite;
    delete texture;
    delete window;

    exit(code);
}


int main(int argc, char** argv)
{
    // Init SFML
    window = new sf::RenderWindow(sf::VideoMode({FULL_WIDTH, FULL_HEIGHT}), "Pure DOOM - SFML Example");
    window->setKeyRepeatEnabled(false); // Don't repeat KeyPresses

    texture = new sf::Texture();
    if (!texture->create({WIDTH, HEIGHT}))
    {
        printf("Error: Failed to create sf::Texture\n");
        exit(1);
    }

    sprite = new sf::Sprite(*texture);
    sprite->setScale({(float)FULL_WIDTH / (float)WIDTH, (float)FULL_HEIGHT / (float)HEIGHT}); // Stretch to full screen

    // Audio stream
    stream = new AudioStream();
    stream->play();

    // Capture mouse
    window->setMouseCursorGrabbed(true);
    window->setMouseCursorVisible(false);

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

    // When doom quit, make sure to release our stuff
    doom_set_exit(quit);

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
    music_thread = new std::thread(music_loop);

    // Initialize doom
    doom_init(argc, argv, DOOM_FLAG_MENU_DARKEN_BG);

    //-----------------------------------------------------------------------
    
    // Main loop
    bool active_mouse = true; // Dev allow us to take mouse out of window
    sf::Mouse::setPosition({FULL_WIDTH / 2, FULL_HEIGHT / 2}, *window);
    while (window->isOpen())
    {
        sf::Event e;
        while (window->pollEvent(e))
        {
            switch (e.type)
            {
                case sf::Event::Closed:
                    window->close();
                    break;

                case sf::Event::KeyPressed:
                    if (e.key.code == sf::Keyboard::Key::End)
                    {
                        active_mouse = !active_mouse;
                        window->setMouseCursorGrabbed(active_mouse);
                        window->setMouseCursorVisible(!active_mouse);
                    }

                    doom_key_down(sfml_scancode_to_doom_key(e.key.code));
                    break;

                case sf::Event::KeyReleased:
                    doom_key_up(sfml_scancode_to_doom_key(e.key.code));
                    break;

                case sf::Event::MouseButtonPressed:
                    if (active_mouse) doom_button_down(sfml_button_to_doom_button(e.mouseButton.button));
                    break;

                case sf::Event::MouseButtonReleased:
                    if (active_mouse) doom_button_up(sfml_button_to_doom_button(e.mouseButton.button));
                    break;
            }
            if (!window->isOpen()) break;
        }
        if (!window->isOpen()) break;

        // Mouse motion. SFML gives weird results. Mouse sometimes jumps.
        if (active_mouse)
        {
            auto mousePos = sf::Mouse::getPosition(*window);
            int mouse_motion_x = mousePos.x - FULL_WIDTH / 2;
            int mouse_motion_y = mousePos.y - FULL_HEIGHT / 2;
            sf::Mouse::setPosition({FULL_WIDTH / 2, FULL_HEIGHT / 2}, *window);
            if (mouse_motion_x || mouse_motion_y)
                doom_mouse_move(mouse_motion_x * 2, mouse_motion_y * 2);
        }
        
        // Update doom.
        audio_mutex.lock();
        quit_from_update = true; // Hack so we don't crash with the audio lock
        doom_update();
        quit_from_update = false;
        audio_mutex.unlock();

        // Blit DOOM's framebuffer onto our SFML texture
        texture->update(doom_get_framebuffer(4));

        // Draw our texture on the screen
        window->draw(*sprite);

        // Swap
        window->display();
    }
    
    // Shutdown (Not reached if quit from doom)
    quit(0);

    return 0;
}
