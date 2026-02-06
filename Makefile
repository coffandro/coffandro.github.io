DEF_FLAGS=-Wall -std=c99 -D_DEFAULT_SOURCE -Wno-missing-braces -Wunused-result -Os
INCLUDES=-Iinclude -Iinclude/raylib/src -Iinclude/raylib/src/external -Igames/lib
LINKING=-Linclude -lraylib
RAYLIB_ARGS=-sUSE_GLFW=3 -sASYNCIFY
SYS=include/libraylib.a -DPLATFORM_WEB -s 'EXPORTED_FUNCTIONS=["_free","_malloc","_set_dimensions","_main"]' -s EXPORTED_RUNTIME_METHODS=["ccall","cwrap","FS","addRunDependency","removeRunDependency"]

DEFAULT_ARGS=$(INCLUDES) $(LINKING) $(RAYLIB_ARGS)

GAMES_GAME_PATH="static/games/game/"

build-libs:
	./raylib.sh

generate-assets:
	node scripts/generate-assets-manifest.js

generate-assets-game:
	mkdir -p $(GAMES_GAME_PATH)assets
	node scripts/generate-assets-manifest.js $(GAMES_GAME_PATH)

build-games-game: generate-assets-game
	mkdir -p $(GAMES_GAME_PATH)

	emcc -o $(GAMES_GAME_PATH)index.html games/lib/*.c games/src/games/*.c \
		$(DEFAULT_ARGS) \
		--shell-file include/raylib/src/minshell.html \
		$(SYS)
