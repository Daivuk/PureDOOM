#define DOOM_IMPLEMENTATION
#include "PureDOOM.h"


// Very simple unit test. We run it for 175 frames,
// then we compare the frame buffer with our test frame buffer to see if they match.

#define FRAMEBUFFER_SIZE (320 * 200 * 4)

void doom_exit_override(int code)
{
    exit(80001);
}

int main(int argc, char** argv)
{
    doom_set_exit(doom_exit_override);
    doom_init(argc, argv, 0);
    printf("Doom initialized\n");

    int duration = 35 * 5;
    while (duration--)
    {
        doom_update();
    }
    const unsigned char* fb = doom_get_framebuffer(4);
    printf("Frames rendered\n");

    FILE* f = fopen("test_framebuffer.raw", "rb");
    if (!f) return 80002;

    void* test_fb = malloc(FRAMEBUFFER_SIZE);
    if (!test_fb) return 80003;

    fread(test_fb, 1, FRAMEBUFFER_SIZE, f);
    fclose(f);

    if (memcmp(fb, test_fb, FRAMEBUFFER_SIZE) != 0) return 80004;
    return 0;
}
