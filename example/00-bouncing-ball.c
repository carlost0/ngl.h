#define NGL_INPUT
#define NGL_INPUT_IMPLEMENTATION
#define NGL_IMPLEMENTATION
#include "../ngl.h"

int main() {
    error_t err = 0;
    u16 rows = 0;
    u16 cols = 0;

    err |= get_term_size(&rows, &cols);
    if (err) return 1;

    screen_t screen = { cols, rows-1, {0}, {0} };

    input_ctx_t input_ctx = {0};
    i32 input = 0;

    err |= init_screen(&screen);
    if (err) return 1;

    err = ngl_init_input(&input_ctx);
    if (err) return 1;

    u32 ball_x = screen.w / 2;
    u32 ball_y = screen.h / 2;
   
    u32 vx = 2;
    u32 vy = 1;

    clear_screen();

    while (input != 'q') {
        input = ngl_get_input(&input_ctx);

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

        err |= clear_bg(&screen, '#', (color_t){0,0,0});
        err |= draw_rect(&screen, ball_x, ball_y, 1, 1, '@', (color_t){0,255,0});
        err |= draw_screen_borders(&screen, 0, (color_t){255, 255, 255});

        err |= print_screen(&screen);
        if (err) return 1;
        delay(1000/60);
    }

   
    ngl_destroy_input(&input_ctx);
    clear_screen();
    destroy_screen(&screen);
    return 0;
}
