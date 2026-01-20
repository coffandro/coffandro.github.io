DEF_FLAGS=-Wall -std=c99 -D_DEFAULT_SOURCE -Wno-missing-braces -Wunused-result -Os
INCLUDES=-Iinclude -Iinclude/raylib/src -Iinclude/raylib/src/external
LINKING=-Linclude -Linclude/raylib/src
RAYLIB_ARGS=-sUSE_GLFW=3 -sASYNCIFY
SYS=include/libraylib.a -DPLATFORM_WEB -s 'EXPORTED_FUNCTIONS=["_free","_malloc","_set_dimensions","_main"]' -s EXPORTED_RUNTIME_METHODS=["ccall","cwrap"]

DEFAULT_ARGS=$(INCLUDES) $(LINKING) $(RAYLIB_ARGS)

GAMES_GAME_PATH="static/games/game/"

build-libs:
	./raylib.sh

build-games-game:
	mkdir -p $(GAMES_GAME_PATH)

	emcc -o $(GAMES_GAME_PATH)index.html games/src/games.c \
		$(DEFAULT_ARGS) \
		--shell-file include/raylib/src/minshell.html \
		$(SYS)
