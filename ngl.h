/* 
 * Copyright (C) 2026 Carlos G. S.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Contact: Instagram DMs @cgs.dev
*/

/* 
 * ngl.h: not a graphics library is a stb-style single-header
 * Graphics Library for the Console written in C.
 *
 * usage:
 *  Include ngl.h in your file and define the NGL_IMPLEMENTATION Macro in your main File.
 * 
 * more Macros:
 *  NGL_UNSTRIP_PREFIX: 
 *  Prepends "ngl_" to all functions, this is a Workaround as we don't 
 *  have Namespaces in C
 *
 *  NGL_INPUT:
 *  Allow ngl to get Input via STDIN, view README.md for the usage
 *
 *  NGL_INPUT_IMPLEMENTATION:
 *  Like NGL_INPUT_IMPLEMENTATION but for input
 *
 * example:
 * // cc -o test test.c
 * // The following Code will display a green Rectangle in the Console until you press 'q'
 *
 * #define NGL_INPUT
 * #define NGL_INPUT_IMPLEMENTATION
 * #define NGL_IMPLEMENTATION
 * #include "ngl.h"
 *
 * int main() {
 *     screen_t screen = {.w = 30, .h = 20};
 *     input_ctx_t input_ctx = {0};
 *
 *     init_screen(&screen);
 *     char input = 0;
 *     init_input(input_ctx, input);
 *
 *     u32 rect_w = 10;
 *     u32 rect_h = 6;
 *     u32 rect_x = 4;
 *     u32 rect_y = 2;
 *    
 *     clear_screen();
 *     while (input != 'q') {
 *         get_input(input_ctx, input);
 *         clear_bg(&screen, '#', (color_t){0,0,0});
 *         draw_rect(&screen, rect_x, rect_y, rect_w, rect_h, '#', (color_t){0,255,0});
 *         draw_screen_borders(&screen, 0, (color_t){255, 255, 255});
 *         print_screen(&screen);
 *         delay(1000/60);
 *     }
 *     clear_screen();
 *    
 *     end_input(input_ctx);
 *     free_screen(&screen);
 * }
 *
*/

#ifndef _NGL_H
#define _NGL_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include <string.h>

#include <time.h>

/* more helpful types */
typedef int8_t    i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;

typedef uint8_t   u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef double   f64;
typedef float    f32;

/* Most ngl Functions return this Type. */
/* ngl also gives you the Freedom to write Error Messages yourself. */
enum ngl_error_e {
    ERR_SUCCESS,

    ERR_INVALID_SIZE,
    ERR_INVALID_PTR,

    ERR_FAILED_MALLOC,
    ERR_FAILED_FILE_OPEN,

    ERR_FAILED_THREAD_CREATION,
    ERR_FAILED_MUTEX_CREATION,
    ERR_FAILED_THREAD_DESTRUCTION,
    ERR_FAILED_MUTEX_DESTRUCTION,
};

/* Basic RGB Color Struct. */
struct ngl_color_s {
    u8 r;
    u8 g;
    u8 b;
};

/* These are two Heap allocated 1d arrays where we will store the "Pixels" to. */
struct ngl_buf_s {
    struct ngl_color_s  *colors;
    char            *chars;
};

/* All the drawing will be done to the "next" Buffer this Struct. */
struct ngl_screen_s {
    u32 w, h;
    struct ngl_buf_s current;
    struct ngl_buf_s next;
};

typedef enum   ngl_error_e  ngl_error_t;
typedef struct ngl_color_s  ngl_color_t;
typedef struct ngl_screen_s ngl_screen_t;
typedef struct ngl_buf_s    ngl_buf_t;

/* Convert a 2d Coordinate into a 1d Index. */
#define ngl_idx(x, y, w) ((y) * (w) + (x))

void ngl_delay(u32 ms);
void ngl_clear_screen(void);

ngl_error_t ngl_get_term_size(u16 *rows, u16 *cols);

ngl_error_t ngl_init_screen(ngl_screen_t *screen);
ngl_error_t ngl_destroy_screen(ngl_screen_t *screen);
ngl_error_t ngl_print_screen(ngl_screen_t *screen);

ngl_error_t ngl_clear_bg(ngl_screen_t *screen, char c, ngl_color_t color);

ngl_error_t ngl_draw_screen_borders(ngl_screen_t *screen, char c, ngl_color_t color); 
ngl_error_t ngl_draw_rect(ngl_screen_t *screen, u32 x, u32 y, u32 w, u32 h, char c, ngl_color_t color);


#ifdef NGL_IMPLEMENTATION

#include <sys/select.h>

void ngl_delay(u32 ms) {
    struct timeval tv;
    tv.tv_sec = ms / 1000;
    tv.tv_usec = (ms % 1000) * 1000L;
    select(0, NULL, NULL, NULL, &tv);
}

#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

/* Function originaly written by Glenn Chappell & Ian Chai 14 Apr 1993 */
ngl_error_t ngl_get_term_size(u16 *rows, u16 *cols) {
  struct winsize ws;
  int fd,result;

  if ((fd = open("/dev/tty",O_WRONLY))<0) return ERR_FAILED_FILE_OPEN;

  result = ioctl(fd,TIOCGWINSZ,&ws);
  close(fd);

  if (result < 0) return ERR_FAILED_FILE_OPEN;

  *cols = ws.ws_col;
  *rows = ws.ws_row;
  return ERR_SUCCESS;
}

ngl_error_t ngl_init_screen(ngl_screen_t *screen) {
    u32 n = screen->w * screen->h;

    /* Allocate front Buffer. */
    screen->current.colors = (ngl_color_t*)malloc(n * sizeof(ngl_color_t));
    if (!screen->current.colors) return ERR_FAILED_MALLOC;

    screen->current.chars = (char*)malloc(n * sizeof(char));
    if (!screen->current.chars) return ERR_FAILED_MALLOC;

    /* Allocate back Buffer. */
    screen->next.colors = (ngl_color_t*)malloc(n * sizeof(ngl_color_t));
    if (!screen->next.colors) return ERR_FAILED_MALLOC;

    screen->next.chars = (char*)malloc(n * sizeof(char));
    if (!screen->next.chars) return ERR_FAILED_MALLOC;

    /* Zero-initialize both Buffers. */
    memset(screen->current.colors, ' ', n * sizeof(ngl_color_t));
    memset(screen->current.chars,  ' ', n * sizeof(char));

    memset(screen->next.colors, ' ', n * sizeof(ngl_color_t));
    memset(screen->next.chars,  ' ', n * sizeof(char));
    return ERR_SUCCESS;
}

ngl_error_t ngl_destroy_screen(ngl_screen_t *screen) {
    if (!screen || !(screen->next.chars && screen->next.chars)) return ERR_INVALID_PTR;

    /* Free front Buffer. */
    if (!screen->current.colors) return ERR_INVALID_PTR;
    free(screen->current.colors);

    if (!screen->current.chars) return ERR_INVALID_PTR;
    free(screen->current.chars);

    /* Free back Buffer. */
    if (!screen->next.colors) return ERR_INVALID_PTR;
    free(screen->next.colors);

    if (!screen->next.chars) return ERR_INVALID_PTR;
    free(screen->next.chars);
     
    screen = NULL;

    return ERR_SUCCESS;
}


ngl_error_t ngl_clear_bg(ngl_screen_t *screen, char c, ngl_color_t color) {
    if (!screen || !(screen->next.chars && screen->next.chars)) return ERR_INVALID_PTR;

    /* Set the whole screen to the specified Chars and Colors. */
    for (u32 i = 0; i < screen->h * screen->w; ++i) {
        screen->next.colors[i] = color;
        screen->next.chars[i] = c;
    }

    return ERR_SUCCESS;
}

void ngl_clear_screen(void) {
    /* ANSI Code to return the Cursor to the Home row and clear everything after the Cursor */
    printf("\x1b[H\x1b[J0");
    fflush(stdout);
}

ngl_error_t ngl_print_screen(ngl_screen_t *screen) {
    if (!screen || !(screen->next.chars && screen->next.chars)) return ERR_INVALID_PTR;

    /* The length of "\x1b[38;2;255;255;255mC" */
    u32 worst_case_pixel = 23;
    u32 cap = worst_case_pixel * screen->w * screen->h;

    /* We draw into a temporary Buffer and write it all at once to STDOUT to reduce CPU Usage */
    char *buf = (char *)malloc(cap * sizeof(char));
    if (!buf) return ERR_FAILED_MALLOC;
    u32 pos = 0;

    ngl_color_t last_col = {0,0,0};

    pos += snprintf(buf+pos, cap-pos, "\x1b[H");

    u32 x, y;
    for (y = 0; y < screen->h; ++y) {
        for (x = 0; x < screen->w; ++x) {
            /* current index, character and color */
            u32 i = ngl_idx(x, y, screen->w);
            char cchar          = screen->current.chars[i];
            ngl_color_t ccol = screen->current.colors[i];

            /* Since ANSI Escape Codes don't reset the color automatically,
             * we only have to change the Color if it differs from the previous color.
             */
            if (!(ccol.r == last_col.r && ccol.g == last_col.g && ccol.b == last_col.b)) {
                pos += snprintf(buf+pos, cap-pos, "\x1b[38;2;%u;%u;%um", ccol.r, ccol.g, ccol.b);
                last_col = ccol;
            }
            
            buf[pos++] = cchar;
        }
        buf[pos++] = '\n';
    }

    fwrite(buf, sizeof(char), pos, stdout);
    fflush(stdout);
    free(buf);

    /* Swap buffers. */
    ngl_buf_t tmp = screen->current;
    screen->current = screen->next;
    screen->next = tmp;

    return ERR_SUCCESS;
}

ngl_error_t ngl_draw_screen_borders(ngl_screen_t *screen, char c, ngl_color_t color) {
    if (!screen || !(screen->next.chars && screen->next.chars)) return ERR_INVALID_PTR;

    u32 w = screen->w;
    u32 h = screen->h;

    char cchar = c;

    if (!c) cchar = '-';

    /* Draw top and bottom lines at the same Time */
    u32 x;
    for (x = 0; x < w; ++x) {
        u32 i = ngl_idx(x, 0, w);
        u32 j = ngl_idx(x, h-1, w);

        screen->next.chars[i] = cchar;
        screen->next.chars[j] = cchar;
        screen->next.colors[i] = color;
        screen->next.colors[j] = color;
    }

    if (!c) cchar = '|';

    /* Draw right and left lines at the same Time */
    u32 y;
    for (y = 1; y < h; ++y) {
        u32 i = ngl_idx(0, y, w);
        u32 j = ngl_idx(w-1, y, w);

        screen->next.chars[i] = cchar;
        screen->next.chars[j] = cchar;

        screen->next.colors[i] = color;
        screen->next.colors[j] = color;
    }

    /* Place the Corners. */

    if (!c) cchar = '+';
    /* Top left Corner. */
    screen->next.chars[0]  = cchar;
    screen->next.colors[0] = color;

    /* Top right Corner. */
    screen->next.chars[w-1]  = cchar;
    screen->next.colors[w-1] = color;

    /* Bottom left Corner. */
    screen->next.chars[ngl_idx(0, h-1, w)] = cchar;
    screen->next.colors[ngl_idx(0, h-1, w)] = color;

    /* Bottom right Corner. */
    screen->next.chars[ngl_idx(w-1, h-1, w)] = cchar;
    screen->next.colors[ngl_idx(w-1, h-1, w)] = color;
    return ERR_SUCCESS;
}

ngl_error_t ngl_draw_rect(ngl_screen_t *screen, u32 x, u32 y, u32 w, u32 h, char c, ngl_color_t color) {
    if (!screen) return ERR_INVALID_PTR;

    /* Bounds check */
    if (x + w > screen->w || y + h > screen->h) return ERR_INVALID_SIZE;

    u32 cx, cy;
    for (cy = y; cy < y + h; ++cy) {
        u32 i = ngl_idx(x, cy, screen->w);
        memset(&screen->next.chars[i], c, w);
        for (cx = 0; cx < w; ++cx) screen->next.colors[i+cx] = color;
    }

    return ERR_SUCCESS;
}

#endif /* NGL_IMPLEMENTATION */

#endif /* _NGL_H */

#ifndef _NGL_INPUT_GUARD
#define _NGL_INPUT_GUARD

#ifdef NGL_INPUT

#include <termios.h>
#include <unistd.h>
#include <pthread.h>

typedef struct {
    i32 input;
    /* these vars shouldn't be edited by external functions */
    pthread_t thread;
    pthread_mutex_t mutex;
    struct termios oldt;
    int running;
} ngl_input_ctx_t;

ngl_error_t ngl_init_input(ngl_input_ctx_t *ctx);
i32 ngl_get_input(ngl_input_ctx_t *ctx);
ngl_error_t ngl_destroy_input(ngl_input_ctx_t *ctx);
#ifdef NGL_INPUT_IMPLEMENTATION

void _ngl_disable_canonical_input(struct termios *oldt);
void _ngl_enable_canonical_input(struct termios *oldt);
void *_ngl_get_keyboard_input(void *arg);

ngl_error_t ngl_init_input(ngl_input_ctx_t *ctx) {
    if (!ctx) return ERR_INVALID_PTR; 

    ctx->input = 0;
    ctx->running = 1;

    if (pthread_mutex_init(&ctx->mutex, NULL) != 0)
        return ERR_FAILED_MUTEX_CREATION;

    _ngl_disable_canonical_input(&ctx->oldt);

    if (pthread_create(&ctx->thread, NULL, _ngl_get_keyboard_input, ctx) != 0) {
        pthread_mutex_destroy(&ctx->mutex);
        _ngl_enable_canonical_input(&ctx->oldt);
        return ERR_FAILED_THREAD_CREATION;
    }

    return ERR_SUCCESS;
}

i32 ngl_get_input(ngl_input_ctx_t *ctx) {
    pthread_mutex_lock(&ctx->mutex);
    i32 input = ctx->input;
    ctx->input = 0;
    pthread_mutex_unlock(&ctx->mutex);
    return input;
}

ngl_error_t ngl_destroy_input(ngl_input_ctx_t *ctx) {
    pthread_mutex_lock(&ctx->mutex);
    ctx->running = 0;

    pthread_mutex_unlock(&ctx->mutex);

    if (pthread_join(ctx->thread, NULL)) return ERR_FAILED_THREAD_DESTRUCTION;
    if (pthread_mutex_destroy(&ctx->mutex)) return ERR_FAILED_THREAD_DESTRUCTION;

    fflush(stdout);
    _ngl_enable_canonical_input(&ctx->oldt);
    return ERR_SUCCESS;
}


void _ngl_disable_canonical_input(struct termios *oldt) {
    struct termios newt;

    /* Get the current terminal Settings. */
    tcgetattr(STDIN_FILENO, oldt);
    newt = *oldt;

    /* Disable canonical mode and echo .*/
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

void _ngl_enable_canonical_input(struct termios *oldt) {
    /* Restore old Settings. */
    tcsetattr(STDIN_FILENO, TCSANOW, oldt);
}

void *_ngl_get_keyboard_input(void *arg) {
    ngl_input_ctx_t *ctx = arg;
    while (ctx->running) {
        char c;
        size_t n = read(STDIN_FILENO, &c, 1);
        if (n == 1) {
            pthread_mutex_lock(&ctx->mutex);
            ctx->input = c;
            pthread_mutex_unlock(&ctx->mutex);
        }
    }
    return NULL;
}

#endif /* NGL_INPUT_IMPLEMENTATION */
#endif /* NGL_INPUT*/
#endif /* _NGL_INPUT_GUARD */

#ifndef _NGL_PREFIX_GUARD
#define _NGL_PREFIX_GUARD
#ifndef NGL_UNSTRIP_PREFIX

#define idx                 ngl_idx

#define delay               ngl_delay
#define clear_screen        ngl_clear_screen

#define get_term_size       ngl_get_term_size

#define init_screen         ngl_init_screen
#define destroy_screen      ngl_destroy_screen

#define print_screen        ngl_print_screen
#define clear_bg            ngl_clear_bg

#define draw_screen_borders ngl_draw_screen_borders
#define draw_rect           ngl_draw_rect

typedef ngl_error_t  error_t;
typedef ngl_screen_t screen_t;
typedef ngl_color_t  color_t;

#ifdef NGL_INPUT
#define init_input    ngl_init_input
#define get_input     ngl_get_input
#define destroy_input ngl_destroy_input
typedef ngl_input_ctx_t input_ctx_t;
#endif /* NGL_INPUT */

#endif /* NGL_UNSTRIP_PREFIX */
#endif /* _NGL_PREFIX_GUARD */
