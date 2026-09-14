CC = cc
DEBUG_CC = clang -fsanitize=address
CFLAGS = -Wall -Wextra -pedantic

default: *
	$(CC) $(CFLAGS) ./00-hello-world.c -o hello
	$(CC) $(CFLAGS) ./10-bouncing-ball.c -o ball
	$(CC) $(CFLAGS) ./20-ngl-math.c -o math
	$(CC) $(CFLAGS) ./30-pong.c -o pong

debug: 
	$(DEBUG_CC) $(CFLAGS) ./00-hello-world.c -o hello
	$(DEBUG_CC) $(CFLAGS) ./10-bouncing-ball.c -o ball
	$(DEBUG_CC) $(CFLAGS) ./20-ngl-math.c -o math
	$(DEBUG_CC) $(CFLAGS) ./30-pong.c -o pong

clean:
	-rm hello ball math pong
