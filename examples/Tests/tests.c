#define DOOM_IMPLEMENTATION
#include "PureDOOM.h"
#include <memory.h>

// Very simple unit test. We run it for 175 frames,
// then we compare the frame buffer with our test frame buffer to see if they match.

#define FRAMEBUFFER_SIZE (320 * 200 * 4)
//#define GEN_FRAME_TEST_DATA

void doom_exit_override(int code)
{
    printf("ERROR: Doom exit()\n");
    exit(80001);
}

int main(int argc, char** argv)
{
    FILE* f;

    doom_set_exit(doom_exit_override);
    doom_init(argc, argv, 0);
    printf("Doom initialized\n");

    int duration = 35 * 5;
    while (duration--) doom_force_update();
    const unsigned char* fb = doom_get_framebuffer(4);
    printf("Frames rendered\n");

#if defined(GEN_FRAME_TEST_DATA)
    f = fopen("test_framebuffer.raw", "wb");
    fwrite(fb, 1, FRAMEBUFFER_SIZE, f);
    fclose(f);
#endif

    f = fopen("test_framebuffer.raw", "rb");
    if (!f)
    {
        printf("ERROR: Failed to open file: test_framebuffer.raw\n");
        return 80002;
    }

    void* test_fb = malloc(FRAMEBUFFER_SIZE);
    if (!test_fb)
    {
        printf("ERROR: Out of memory\n");
        return 80003;
    }

    fread(test_fb, 1, FRAMEBUFFER_SIZE, f);
    fclose(f);

    if (memcmp(fb, test_fb, FRAMEBUFFER_SIZE) != 0)
    {
        printf("ERROR: Frames not matching\n");
        return 80004;
    }
    
    printf("SUCCESS: Frames matching\n");
    return 0;
}
