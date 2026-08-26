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

    int *cells = (int *) malloc(w*h * sizeof(int));
    memset(cells, 0, w*h*sizeof(int));
    InitWindow(w * RATIO_W, h * RATIO_H, "fontbuilder");

    printf("Press P to print the Sprite as a C String.\n");
    printf("Press C or Mouse 3 to Clear the Screen as a C String.\n");
    while (!WindowShouldClose()) {
        BeginDrawing();

        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {
                if (cells[y * w + x]) {
                    DrawRectangle(x * RATIO_W, y * RATIO_H, RATIO_W, RATIO_H, RAYWHITE);
                } else {
                    DrawRectangle(x * RATIO_W, y * RATIO_H, RATIO_W, RATIO_H, BLACK);
                }
            }       
        }       

        for (int x = 0; x < w; ++x)
            DrawRectangle(x * RATIO_W, 0, line_thickness, h * RATIO_H, GRAY);
        for (int y = 0; y < h; ++y)
            DrawRectangle(0, y * RATIO_H, w * RATIO_W, line_thickness, GRAY);

        EndDrawing();

        int mx = GetMouseX();
        int my = GetMouseY();

        int mouse_i = (my/RATIO_H) * w + (mx/RATIO_W);

        if (IsMouseButtonDown(0)) {
            cells[((int)(my/RATIO_H)) * w + ((int)(mx/RATIO_W))] = 1;
        } else if (IsMouseButtonDown(1)) {
            cells[((int)(my/RATIO_H)) * w + ((int)(mx/RATIO_W))] = 0;
        } else if (IsMouseButtonDown(2) || IsKeyPressed(KEY_C)) {
            memset(cells, 0, w*h*sizeof(int));
        }

        if (IsKeyPressed(KEY_P)) {
            printf("NGL_GLYPH(");
            for (int y = 0; y < h; ++y) {
                printf("0b");
                for (int x = w-1; x >= 0; --x) {
                    printf("%d", cells[x + y * w]);
                }
                if (y != h-1) printf(",");
            }
            printf("),\n");
        }
    }

    CloseWindow();
}
