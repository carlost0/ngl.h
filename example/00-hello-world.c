/* cc examples/00-hello-world.c -o hello */
#define NGL_IMPLEMENTATION
#define NGL_NO_MATH
#include "../ngl.h"

#include <stdlib.h>

static const u32 FPS = 30;

int main() {
    u32 w, h;
    get_term_size(&w, &h);
    h--;

    screen_t screen = screen_new(w, h);
    input_ctx_t input_ctx = input_new();
    ngl_font_t font = {.hpad = 2};
    load_glyphs(&font, NULL);

    const char *username = getenv("USER");


    clear_screen();

    int running = 1;
    while (running) {
        get_keyboard_state(&input_ctx);
        if (is_key_down(input_ctx, KEY_Q)) running = 0;

        clear_bg(&screen, '#', (color_t){0,0,0});
        draw_text_fmt(&screen, font, 2, 2, 'l', (color_t){255,255,255}, "Hello, %s!", username);
        draw_screen_borders(&screen, 0, (color_t){255,255,255});
        print_screen(&screen);

        delay(1000/FPS);
    }

    destroy_input(&input_ctx);
    destroy_screen(&screen);

    return 0;
}
