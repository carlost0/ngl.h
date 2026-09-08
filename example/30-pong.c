/* cc -o pong examples/30-pong.c */
#define NGL_IMPLEMENTATION
#include "../ngl.h"

typedef struct {
    vec2_t pos;
    vec2_t vel;
    u32    w, h;
} ball_t;

typedef struct {
    vec2_t  pos;
    u32     w, h;
    color_t col;
} paddle_t;

int check_collision(i32 x1, i32 y1, i32 w1, i32 h1,
        i32 x2, i32 y2, i32 w2, i32 h2) {
    if (x1 + w1 < x2) return 0;
    if (x1 > x2 + w2) return 0;
    if (y1 + h1 < y2) return 0;
    if (y1 > y2 + h2) return 0;

    return 1;
}

int main() {
    u32 w, h;
new_game:
    get_term_size(&w, &h);
    h--;

    screen_t screen = screen_new(w, h);
    input_ctx_t input_ctx = input_new();
    font_t font = {.hpad = 2};
    load_glyphs(&font, NULL);

    ball_t ball = {
        .pos = vec2((f64)w / 2, (f64)h / 2),

        .vel = vec2(1, 0.67),

        .w = w/30,
        .h = (f64)w/30/1.67,
    };
   
    paddle_t paddle = {
        .pos = vec2(2, (f64)h / 2),

        .w = 3,
        .h = h/5,
        .col = {255, 0, 0},
    };

    clear_screen();

    bool running = true;
    bool lost = false;
    while (running) {
        get_keyboard_state(&input_ctx);

        if (is_key_down(input_ctx, KEY_Q)) running = false;
        if (is_key_down(input_ctx, KEY_R)) {
            destroy_input(&input_ctx);
            destroy_screen(&screen);
            goto new_game;
        }

        if (lost) {
            const char *loss_text = "you lost :(";
            const char *quit_text = "(q)uit";
            const char *restart_text = "(r)estart";

            clear_bg(&screen, ' ', (color_t){0});
            draw_text(
                &screen,
                font,
                w / 2 - (strlen(loss_text) / 2) * (font.w + font.hpad), h/2 - 5,
                'l', (color_t){255,40,40},
                loss_text);
            draw_text(
                &screen,
                font,
                w / 2 - (strlen(quit_text) / 2) * (font.w + font.hpad), h / 2 + 4,
                'Q', (color_t){200,60,60},
                quit_text);
            draw_text(
                &screen,
                font,
                w / 2 - (strlen(restart_text) / 2) * (font.w + font.hpad), h / 2 + 11,
                'R', (color_t){200,60,60},
                restart_text);
            print_screen(&screen);
            continue;
        }
        vec2_t new_pos = vec2_add(ball.vel, ball.pos);

        if ( (u32)new_pos.x <= 0) {
            lost = true;
        } else if ((u32)new_pos.x + ball.w >= w) {
            ball.vel.x *= -1;
        }

        if ((u32)new_pos.y + ball.h >= h || (u32)new_pos.y <= 0) {
            ball.vel.y *= -1;
        }

        if (check_collision(paddle.pos.x, paddle.pos.y, paddle.w, paddle.h,
                            new_pos.x,    new_pos.y,    ball.w,   ball.h)) {
            ball.vel.x *= -1;
        }

        new_pos = vec2_add(ball.vel, ball.pos);

        ball.pos = new_pos;

        if (is_key_down(input_ctx, KEY_W)) {
            if (paddle.pos.y - 1 > 0) paddle.pos.y--;
        }

        if (is_key_down(input_ctx, KEY_S)) {
            if (paddle.pos.y + 1 + paddle.h < h) paddle.pos.y++;
        }

        clear_bg(&screen, ' ', (color_t){0,0,0});

        draw_rect(&screen, paddle.pos.x, paddle.pos.y, paddle.w, paddle.h, '$', paddle.col);
        draw_rect(&screen, (u32)ball.pos.x, (u32)ball.pos.y, ball.w, ball.h, '@', (color_t){0,255,0});

        draw_screen_borders(&screen, 0, (color_t){255, 255, 255});

        print_screen(&screen);
        delay(1000/60);
    }

    clear_screen();
   
    destroy_input(&input_ctx);
    destroy_screen(&screen);
}


