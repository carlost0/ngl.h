> [!NOTE]
> This is a continuation to [aschii](https://github.com/carlost0/aschii).

# ngl.h

Not a Graphics Library is a stb-style single Header Graphics Library for your Terminal written with ISO C99.
ngl.h only has around 1000 lines of actual code, 450 semicolons and 100 open curly braces.

---

# TODO
- Add windows support (maybe).

# Usage
Download ngl.h into your Project's directory: `wget https://github.com/carlost0/ngl.h/releases/download/v1.0.1/ngl.h`,
then include ngl.h in your Program.

See the example directory for a quick Guide on how to use ngl.h.

The source code of ngl.h aims to be very understandable, and extensively commented so even an English speaking Baby that knows C could understand it.

# Example
Here's a basic Program that with `ngl.h` as in examples/10-bouncing-ball.c

``` C 
/* cc examples/10-bouncing-ball.c -o bouncing-ball */
#define NGL_NO_FONTS
#define NGL_NO_MATH
#define NGL_IMPLEMENTATION
#include "../ngl.h"

static const u32 FPS = 60;

int main(void) {
    /* Nearly all ngl.h functions return an error Code. */
    error_t err = 0;

    /* ngl_api_t holds all the Context ngl needs to work (Screen buffers,
     * screen size, font, input device etc.).
     * We could also configure things like the screen size or font padding,
     * example:
     *  ngl = ngl_new(.font.hpad = 3, .font.vpad = 2, .screen.w = 32, .screen.h = 32);
     * all Options are specified in ngl.h.
     * 
     * Since we use the default options, we put 0 as the parameter.
     */
    ngl_t ngl = ngl_new(0);

    if (ngl.screen.status != ERR_SUCCESS) {
        fprintf(stderr, "ERROR: %s", error_to_string(ngl.screen.status));
        return -1;
    }

    if (ngl.input.status != ERR_SUCCESS) {
        fprintf(stderr, "ERROR: %s", error_to_string(ngl.input.status));
        return -1;
    }

    u32 ball_x = ngl.screen.w / 2;
    u32 ball_y = ngl.screen.h / 2;
   
    u32 vx = 2;
    u32 vy = 1;

    /* clear_screen prints the ANSI Escape Codes to set the Cursor's Position to (0,0) and clears everything after it. */
    clear_screen();

    bool running = true;
    while (running) {
        get_keyboard_state(&ngl);
        if (is_key_down(&ngl, KEY_Q)) running = false;


        u32 nx = ball_x + vx;
        u32 ny = ball_y + vy;

        if (nx >= ngl.screen.w) {
            vx *= -1;
            nx = ball_x + vx;
        }
        if (ny >= ngl.screen.h) {
            vy *= -1;
            ny = ball_y + vy;
        }

        ball_x = nx;
        ball_y = ny;

        /* It isn't necesary to check for Errors every Frame, but it's probably a good Habit. */
        err |= fill_bg(&ngl, '#', (color_t){0,0,0});
        err |= draw_rect(&ngl, ball_x, ball_y, 1, 1, '@', GREEN);
        err |= draw_screen_borders(&ngl, 0, WHITE);

        err |= print_screen(&ngl);
        if (err) {
            ngl_destroy(&ngl);
            return -1;
        }

        /* Since the Program is so small, we can delay by 1000/60 ms to get to 60 FPS */
        delay(1000/FPS);
    }

    clear_screen();
   
    ngl_destroy(&ngl);
    return 0;
}

```
