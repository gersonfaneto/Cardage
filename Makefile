CC := cc

WFLAGS := -Wall -Wextra -Wpedantic -Werror

CFLAGS := -std=gnu99
CFLAGS += -I external/caffe/src/
CFLAGS += -I external/raylib/include/

LFLAGS := -L external/raylib/lib/
LFLAGS += -l:libraylib.a -lm -lX11 -lGL


all:
	[[ ! -d ./dist ]] && mkdir ./dist || exit 0
	cc -o dist/main src/main.c $(WFLAGS) $(CFLAGS) $(LFLAGS)

debug:
	[[ ! -d ./dist ]] && mkdir ./dist || exit 0
	cc -o dist/main src/main.c $(WFLAGS) $(CFLAGS) -D__DEBUG__ $(LFLAGS)

ui:
	[[ ! -d ./dist ]] && mkdir ./dist || exit 0
	cc -o dist/game src/main.c $(WFLAGS) $(CFLAGS) -D__GAME_UI__ $(LFLAGS)

compilation-db:
	[[ ! -d ./.build ]] && mkdir ./.build || exit 0
	bear --output ./.build/compile_commands.json -- make

clean:
	rm -r -f -v ./dist/
	rm -r -f -v ./.build/
