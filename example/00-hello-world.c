#define NGL_IMPLEMENTATION
#include "../ngl.h"

int main() {
    u16 w, h;
    get_term_size(&h, &w);

    screen_t screen = {w, h-1, {0}, {0}};
    input_ctx_t input_ctx = {0};
    ngl_font_t font = {.hpad = 2};

    load_glyphs(&font, NULL);
    init_screen(&screen);
    init_input(&input_ctx);

    clear_screen();

    int running = 1;
    while (running) {
        get_keyboard_state(&input_ctx);
        if (is_key_down(input_ctx, KEY_Q)) running = 0;

        clear_bg(&screen, '#', (color_t){0,0,0});
        draw_text(&screen, font, 2, 2, 'l', (color_t){255,255,255}, "Hello, World!");
        draw_screen_borders(&screen, 0, (color_t){255,255,255});
        print_screen(&screen);
    }

    destroy_input(&input_ctx);
    destroy_screen(&screen);

    return 0;
}
