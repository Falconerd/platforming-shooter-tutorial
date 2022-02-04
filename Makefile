FLAGS = -std=c99 -g3 -O0 -pedantic -Wall -Wextra -Werror -ldl
INCLUDE = -I./deps/include -I/usr/include/SDL2 -D_REENTRANT
LIBS = -L/usr/lib -pthread -lSDL2

build: ./src/main.c ./deps/src/gl.c ./io.o ./render.o ./render_init.o ./render_util.o ./input.o ./config.o ./config_init.o ./util.o ./physics.o ./physics_init.o ./time.o ./entity.o
	gcc $(FLAGS) $(INCLUDE) $(LIBS) $^

io.o: ./src/engine/io/io.c
	gcc $(FLAGS) -c $^

render.o: ./src/engine/render/render.c ./src/engine/render/render_util.c ./src/engine/render/render_init.c
	gcc $(FLAGS) -c $(INCLUDE) $^

input.o: ./src/engine/input/input.c
	gcc $(FLAGS) -c $(INCLUDE) $^

config.o: ./src/engine/config/config.c ./src/engine/config/config_init.c
	gcc $(FLAGS) -c $^

util.o: ./src/engine/util/util.c
	gcc $(FLAGS) -c $^

physics.o: ./src/engine/physics/physics.c ./src/engine/physics/physics_init.c
	gcc $(FLAGS) -c $(INCLUDE) $^

time.o: ./src/engine/time/time.c
	gcc $(FLAGS) -c $(INCLUDE) $^

entity.o: ./src/engine/entity/entity.c
	gcc $(FLAGS) -c $(INCLUDE) $^

clean:
	@rm -rf *.o
	@rm -rf a.out

run:
	./a.out
