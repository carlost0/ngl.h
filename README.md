> [!NOTE]
> This is a continuation to [aschii](https://github.com/carlost0/aschii).

# ngl.h
Not a Graphics Library is a stb-style single Header Graphics Library for your Terminal written with ISO C99.

# Usage
Download ngl.h into your Project's directory: `wget https://raw.githubusercontent.com/carlost0/ngl.h/refs/heads/main/ngl.h`,
then include ngl.h in your Program.

See the example directory for a quick Guide on how to use ngl.h.

The source code of ngl.h aims to be very understandable, and is extensively commented so even an English speaking Baby that knows C could understand it.

# Example
Here's a basic Program that with `ngl.h` as in examples/00-bouncing-ball.c

``` C 
#define NGL_INPUT
#define NGL_INPUT_IMPLEMENTATION
#define NGL_IMPLEMENTATION
#include "../ngl.h"

static const u32 FPS = 60;
int main() {
    error_t err = 0;
    u16 rows = 0;
    u16 cols = 0;

    /* Nearly all ngl.h functions return an error Code. */
    /* The error Codes are specified in ngl.h and probably self-explanatory. */
    err |= get_term_size(&rows, &cols);
    if (err) return 1;

    /* Specify the Width and Height of the Screen. */
    /* We use rows-1 because only rows would cause Scroling. */
    screen_t screen = { cols, rows-1, };

    /* The input_ctx_t type stores all the necesary Stuff to get User Input from STDIN on a seperate Thread without blocking Input. */
    input_ctx_t input_ctx = {0};
    i32 input = 0;

    /* This Function allocates Heap memory for the Front and Back Buffers. */
    /* A Buffer consists of two 1d arrays, one for the Characters (4 Byte i32), and one for the Colors (3 * 1 Bytes u8 for the Red, Green and Blue channels). */
    err |= init_screen(&screen);
    if (err) return 1;

    /* init_input creates a Mutex for the Input and starts a new Nhread where we will be reading from STDIN. */
    err = init_input(&input_ctx);
    if (err) return 1;

    u32 ball_x = screen.w / 2;
    u32 ball_y = screen.h / 2;
   
    u32 vx = 2;
    u32 vy = 1;

    /* clear_screen prints the ANSI Escape Codes to set the Cursor's Position to (0,0) and clears everything after it. */
    clear_screen();

    while (input != 'q') {
        input = get_input(&input_ctx);

        u32 nx = ball_x + vx;
        u32 ny = ball_y + vy;

        if (nx >= screen.w) {
            vx *= -1;
            nx = ball_x + vx;
        }
        if (ny >= screen.h) {
            vy *= -1;
            ny = ball_y + vy;
        }

        ball_x = nx;
        ball_y = ny;

        /* It isn't necesary to check for Errors every Frame, but it's probably a good Habit. */
        err |= clear_bg(&screen, '#', (color_t){0,0,0});
        err |= draw_rect(&screen, ball_x, ball_y, 1, 1, '@', (color_t){0,255,0});
        err |= draw_screen_borders(&screen, 0, (color_t){255, 255, 255});

        err |= print_screen(&screen);
        if (err) return 1;

        /* Since the Program is so small, we can delay by 1000/60 ms to get to 60 FPS */
        delay(1000/FPS);
    }

    clear_screen();
   
    /* Here we Join the input thread, destroy the Mutex and free all the Screen Buffers. */
    destroy_input(&input_ctx);
    destroy_screen(&screen);
    return 0;
}

```
