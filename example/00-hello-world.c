/* cc examples/00-hello-world.c -o hello */
#define NGL_IMPLEMENTATION
#define NGL_NO_MATH
#include "../ngl.h"

static const u32 FPS = 30;

int main(void) {
    ngl_api_t ngl = ngl_new(.font.hpad = 2);

    const char *username = getenv("USER");

    clear_screen();

    int running = 1;
    while (running) {
        get_keyboard_state(&ngl);
        if (is_key_down(&ngl, KEY_Q)) running = 0;

        fill_bg(&ngl, '#', (color_t){0,0,0});
        draw_text_fmt(&ngl, 2, 2, 'l', (color_t){255,255,255}, "Hello, %s!", username);
        draw_screen_borders(&ngl, 0, (color_t){255,255,255});
        print_screen(&ngl);

        delay(1000/FPS);
    }

    ngl_destroy(&ngl);
    return 0;
}
