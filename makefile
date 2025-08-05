SOURCES = $(wildcard src/*.c)
OBJECTS = $(patsubst src/%.c, out/%.o, $(SOURCES))

NAME = ruby

ifeq ($(shell uname -s), Darwin)
LIB = -lglfw.3 -Llib/macos -rpath @executable_path/lib/macos
else
LIB = $(shell pkg-config --libs glfw3 gl) -lm
endif

$(NAME): $(OBJECTS)
	gcc $(OBJECTS) -o $(NAME) $(LIB)

out/%.o: src/%.c | out
	gcc $< -c -o $@ -Wall -Ilib/include -g

out:
	mkdir -p out

.PHONY: clean
clean:
	rm -r out
	rm $(NAME)
