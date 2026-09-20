CC = gcc
DEBUG_CC = clang -fsanitize=address
CFLAGS = -std=c99 -Wall -Wextra -pedantic #-static

# Build default static executables
default: *
	$(CC) $(CFLAGS) -O2 -static ./00-hello-world.c   -o hello
	$(CC) $(CFLAGS) -O2 -static ./10-bouncing-ball.c -o ball
	$(CC) $(CFLAGS) -O2 -static ./20-ngl-math.c      -o math -lm
	$(CC) $(CFLAGS) -O2 -static ./30-pong.c          -o pong

all: default small debug

# Minimize executable size
small: *
	$(CC) $(CFLAGS) -Oz ./00-hello-world.c   -o small_hello
	$(CC) $(CFLAGS) -Oz ./10-bouncing-ball.c -o small_ball
	$(CC) $(CFLAGS) -Oz ./20-ngl-math.c      -o small_math -lm
	$(CC) $(CFLAGS) -Oz ./30-pong.c          -o small_pong
	strip small_hello small_ball small_math small_pong

# Build with Additional debug info and Address sanitization
debug: *
	$(DEBUG_CC) $(CFLAGS) -Werror -ggdb ./00-hello-world.c   -o debug_hello
	$(DEBUG_CC) $(CFLAGS) -Werror -ggdb ./10-bouncing-ball.c -o debug_ball
	$(DEBUG_CC) $(CFLAGS) -Werror -ggdb ./20-ngl-math.c      -o debug_math
	$(DEBUG_CC) $(CFLAGS) -Werror -ggdb ./30-pong.c          -o debug_pong

clean:
	-rm hello ball math pong
	-rm small_hello small_ball small_math small_pong
	-rm debug_hello debug_ball debug_math debug_pong
