#define DOOM_IMPLEMENTATION
#include "PureDOOM.h"


// Very simple unit test. We run it for 175 frames,
// then we compare the frame buffer with our test frame buffer to see if they match.

#define FRAMEBUFFER_SIZE (320 * 200 * 4)

int main(int argc, char** argv)
{
    doom_init(argc, argv, 0);

    int duration = 35 * 5;
    while (duration--)
    {
        doom_update();
    }
    const unsigned char* fb = doom_get_framebuffer(4);

    FILE* f = fopen("test_framebuffer.raw", "rb");
    if (!f) return 1;

    void* test_fb = malloc(FRAMEBUFFER_SIZE);
    if (!test_fb) return 2;

    fread(test_fb, 1, FRAMEBUFFER_SIZE, f);
    fclose(f);

    return memcmp(fb, test_fb, FRAMEBUFFER_SIZE);
}
