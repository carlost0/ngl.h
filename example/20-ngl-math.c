/* cc -o math examples/20-ngl-math.c */
#define NGL_IMPLEMENTATION
#define NGL_NO_INPUT
#define NGL_NO_FONTS
#include "../ngl.h"

#include <stdio.h>

void print_vector(const char *prefix, vec2_t vec) {
    printf("%s(%f|%f)\n", prefix, vec.x, vec.y);
}

int main() {
    vec2_t base_vec1       = vec2(2, 1);
    vec2_t base_vec2       = vec2(1, -4);
    f64    scalar          = 3.4;
    f64    angle           = 20;
    f64    angle_r         = deg_to_rad(angle);
    vec2_t added_vec       = vec2_add(base_vec1, base_vec2);
    vec2_t subbed_vec      = vec2_sub(base_vec1, base_vec2);
    vec2_t mul_vec         = vec2_mul(base_vec1, base_vec2);
    f64    dotted_vec      = vec2_dot(base_vec1, base_vec2);
    f64    crossed_vec     = vec2_cross(base_vec1, base_vec2);
    vec2_t dived_vec       = vec2_div(base_vec1, base_vec2);
    vec2_t sqrt_vec        = vec2_sqrt(base_vec1);
    vec2_t scaled_vec      = vec2_scale(base_vec1, scalar);
    f64    len_vec         = vec2_len(base_vec1);
    vec2_t normalized_vec  = vec2_normalize(base_vec1);
    vec2_t rot90cw_vec     = vec2_rot90cw(base_vec1);
    vec2_t rot90ccw_vec    = vec2_rot90ccw(base_vec1);
    vec2_t rot_vec         = vec2_rot(base_vec1, angle_r);

    print_vector("v1:         | ", base_vec1);
    printf("---------------------------------\n");
    print_vector("v2:         | ", base_vec2);
    printf("---------------------------------\n");
    printf("scalar      | %f\n", scalar);
    printf("---------------------------------\n");
    printf("angle       | %f\n", angle);
    printf("---------------------------------\n");
    printf("radians     | %f\n", angle_r);
    printf("\n\n");
    print_vector("v1 + v2:    | ", added_vec);
    printf("---------------------------------\n");
    print_vector("v1 - v2:    | ", subbed_vec);
    printf("---------------------------------\n");
    print_vector("v1 * v2:    | ", mul_vec);
    printf("---------------------------------\n");
    print_vector("v1 / v2:    | ", dived_vec);
    printf("---------------------------------\n");
    print_vector("sqrt(v1):   | ", sqrt_vec);
    printf("---------------------------------\n");
    print_vector("v1 * scalar:| ", scaled_vec);
    printf("---------------------------------\n");
    printf("v1 dot v2:  | %f\n", dotted_vec);
    printf("---------------------------------\n");
    printf("v1 cross v2:| %f\n", crossed_vec);
    printf("---------------------------------\n");
    printf("v1 len:     | %f\n", len_vec);
    printf("---------------------------------\n");
    print_vector("normal v1:  | ", normalized_vec);
    printf("---------------------------------\n");
    print_vector("v1 90°:     | ", rot90cw_vec);
    printf("---------------------------------\n");
    print_vector("v1 -90°:    | ", rot90ccw_vec);
    printf("---------------------------------\n");
    print_vector("v1 20°:     | ", rot_vec);
    return 0;
}
