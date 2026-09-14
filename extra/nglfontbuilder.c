/* cc nglfontbuilder.c -o fontbuilder -lraylib -lX11 -lm */
#include <raylib.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define RATIO_W 120
#define RATIO_H (RATIO_W * 1.5)

int main() {
    int w = 5, h = 5;
    int line_thickness = 3;

    char *cells = (char*)malloc(w*h * sizeof(char));
    memset(cells, '0', w*h * sizeof(char));
    InitWindow(w * RATIO_W, h * RATIO_H, "fontbuilder");

    printf("Press P to print the Sprite as a C String.\n");
    printf("Press C or Mouse 3 to Clear the Screen.\n");

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(BLACK);

        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {
                Color color = cells[y * w + x] == '1'
                            ? RAYWHITE
                            : BLACK;

                DrawRectangle(
                    x * RATIO_W,
                    y * RATIO_H,
                    RATIO_W,
                    RATIO_H,
                    color
                );
            }
        }

        for (int x = 0; x <= w; ++x) {
            DrawRectangle(
                x * RATIO_W,
                0,
                line_thickness,
                h * RATIO_H,
                GRAY
            );
        }

        for (int y = 0; y <= h; ++y) {
            DrawRectangle(
                0,
                y * RATIO_H,
                w * RATIO_W,
                line_thickness,
                GRAY
            );
        }

        EndDrawing();

        int mx = GetMouseX();
        int my = GetMouseY();

        int cell_x = mx / RATIO_W;
        int cell_y = my / RATIO_H;

        if (cell_x >= 0 && cell_x < w &&
            cell_y >= 0 && cell_y < h) {

            int index = cell_y * w + cell_x;

            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                cells[index] = '1';
            } else if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
                cells[index] = '0';
            }
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE) ||
            IsKeyPressed(KEY_C)) {
            memset(cells, '0', w * h);
        }

        if (IsKeyPressed(KEY_P)) {
        printf("NGL_GLYPH(");

        for (int y = 0; y < h; ++y) {
            unsigned int value = 0;

            // Read each row from right to left
            for (int x = w - 1; x >= 0; --x) {
                value <<= 1;

                if (cells[y * w + x] == '1') {
                    value |= 1;
                }
            }

            printf("0x%02x", value);

            if (y != h - 1) {
                printf(",");
            }
        }

            printf("),\n");
        }

    }


    free(cells);
    CloseWindow();
}
