/* cc -o pong examples/30-pong.c */
#define NGL_IMPLEMENTATION
#include "../ngl.h"

typedef struct {
    vec2_t pos;
    vec2_t vel;
    u32    w, h;
} ball_t;

typedef struct {
    vec2_t    pos;
    ngl_float vy;
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

int main(void) {
new_game: (void)0;
    ngl_t ngl = ngl_new(.font.hpad = 2);
    u32 w = ngl.screen.w, h = ngl.screen.h;

    ball_t ball = {
        .pos = vec2(w / 2.0, h / 2.0),

        .vel = vec2(60, 40),

        .w = w / 30.0,
        .h = w / 30.0 / 1.67,
    };
   
    paddle_t paddle = {
        .pos = vec2(2, (f64)h / 2),
        .vy = 50,

        .w = 3,
        .h = h/5,
        .col = RED,
    };

    clear_screen();

    bool running = true;
    bool lost = false;
    while (running) {
        get_keyboard_state(&ngl);

        if (is_key_down(&ngl, KEY_Q)) running = false;
        if (is_key_down(&ngl, KEY_R)) {
            ngl_destroy(&ngl);
            goto new_game;
        }

        if (lost) {
            const char *loss_text = "you lost :(";
            const char *quit_text = "(q)uit";
            const char *restart_text = "(r)estart";

            fill_bg(&ngl, ' ', (color_t){0});
            draw_text(
                &ngl,
                w / 2 - (strlen(loss_text) / 2) * (ngl.font.w + ngl.font.hpad), h / 2 - 5,
                'l', (color_t){255,40,40},
                loss_text);
            draw_text(
                &ngl,
                w / 2 - (strlen(quit_text) / 2) * (ngl.font.w + ngl.font.hpad), h / 2 + 4,
                'Q', (color_t){200,60,60},
                quit_text);
            draw_text(
                &ngl,
                w / 2 - (strlen(restart_text) / 2) * (ngl.font.w + ngl.font.hpad), h / 2 + 11,
                'R', (color_t){200,60,60},
                restart_text);
            print_screen(&ngl);
            continue;
        }
        get_dt(&ngl);

        vec2_t new_pos = vec2_add(vec2_scale(ball.vel, ngl.dt), ball.pos);

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

        new_pos = vec2_add(vec2_scale(ball.vel, ngl.dt), ball.pos);

        ball.pos = new_pos;

        if (is_key_down(&ngl, KEY_W)) {
            if (paddle.pos.y - paddle.vy*ngl.dt > 1) paddle.pos.y -= paddle.vy * ngl.dt;
        }

        if (is_key_down(&ngl, KEY_S)) {
            if (paddle.pos.y + paddle.vy*ngl.dt + paddle.h < h) paddle.pos.y += paddle.vy * ngl.dt;
        }

        fill_bg(&ngl, ' ', (color_t){0,0,0});

        draw_rect(&ngl, paddle.pos.x, paddle.pos.y, paddle.w, paddle.h, '$', paddle.col);
        draw_rect(&ngl, ball.pos.x, ball.pos.y, ball.w, ball.h, '@', GREEN);

        draw_screen_borders(&ngl, 0, (color_t){255, 255, 255});

        print_screen(&ngl);
        delay(1000.0 / 60.0 - ngl.dt);
    }

    clear_screen();
   
    ngl_destroy(&ngl);
}


