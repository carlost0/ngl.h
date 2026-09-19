#!/usr/bin/env bash

FILE=$1
touch $FILE


echo "/* cc $FILE -o main */
#define NGL_IMPLEMENTATION
#include \"ngl.h\"

int main(void) {
    ngl_api_t ngl = ngl_new(0);

    bool running = true;
    clear_screen();

    while (running) {
        /* Input handling. */
        get_keyboard_state(&api);

        if (is_key_down(ngl, KEY_Q)) running = false;

        /* Game Logic here. */
        
        fill_bg(&ngl, ' ', ngl_color(0,0,0));
        /* Drawing goes here. */

        print_screen(&ngl);
    }

    ngl_destroy(&ngl);
}
" > $FILE
