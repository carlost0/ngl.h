/* cc -o bouncing-ball 10-bouncing-ball.c  */
#define NGL_NO_FONTS
#define NGL_NO_MATH
#define NGL_IMPLEMENTATION
#include "../ngl.h"

static const u32 FPS = 60;

int main() {
    error_t err = 0;
    u32 w, h;;

    /* Nearly all ngl.h functions return an error Code. */
    /* The error Codes are specified in ngl.h and probably self-explanatory. */
    err |= get_term_size(&w, &h);
    if (err) return 1;

    /* Specify the Width and Height of the Screen. */
    /* We use rows-1 because only rows would cause Scroling. */
    /* This Function allocates Heap memory for the Front and Back Buffers and returns the Screen Struct. */
    /* A Buffer consists of two 1d arrays, one for the Characters (4 Byte i32), and one for the Colors (3 * 1 Bytes u8 for the Red, Green and Blue channels). */
    screen_t screen = screen_new(w, h-1);
    if (screen.status != ERR_SUCCESS) return 1;

    /* The input_ctx_t type stores all the necesary Stuff to get User Input from /dev/input using the 'poll' syscall. */
    /* This Function turns terminal Echo of, and opens the Keyboard's File Descriptor, for that the User must be in the 'input' group */
    input_ctx_t input_ctx = input_new();
    if (input_ctx.status != ERR_SUCCESS) return 1;


    u32 ball_x = screen.w / 2;
    u32 ball_y = screen.h / 2;
   
    u32 vx = 2;
    u32 vy = 1;

    /* clear_screen prints the ANSI Escape Codes to set the Cursor's Position to (0,0) and clears everything after it. */
    clear_screen();

    bool running = true;
    while (running) {
        get_keyboard_state(&input_ctx);
        if (is_key_down(input_ctx, KEY_Q)) running = false;


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
