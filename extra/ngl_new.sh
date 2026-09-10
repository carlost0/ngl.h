#!/usr/bin/env bash

FILE=$1
touch $FILE


echo "
#define NGL_IMPLEMENTATION
#include \"ngl.h\"

int main() {
    u32 w, h;
    get_term_size(&w, &h);

    screen_t screen = screen_new(w, --h);
    input_ctx_t input = input_new();
    font_t font = {0};

    load_glyphs(&font, NULL);

    bool running = true;
    clear_screen();

    while (running) {
        /* Input handling. */
        get_keyboard_state(&input);

        if (is_key_down(input, KEY_Q)) running = false;

        /* Game Logic here. */
        
        clear_bg(&screen, ' ', ngl_color(0,0,0));
        /* Drawing goes here. */

        print_screen(&screen);
    }

    destroy_screen(&screen);
    destroy_input(&input);
}
" > $FILE
