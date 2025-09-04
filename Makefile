CC := cc

WFLAGS := -Wall -Wextra -Wpedantic -Werror

CFLAGS := -std=gnu99
CFLAGS += -I external/caffe/src/


all:
	[[ ! -d ./dist ]] && mkdir ./dist || exit 0
	cc -o dist/main src/main.c $(WFLAGS) $(CFLAGS)

debug:
	[[ ! -d ./dist ]] && mkdir ./dist || exit 0
	cc -o dist/main src/main.c $(WFLAGS) $(CFLAGS) -D__DEBUG__

compilation-db:
	[[ ! -d ./.build ]] && mkdir ./.build || exit 0
	bear --output ./.build/compile_commands.json -- make

clean:
	rm -r -f -v ./dist/
	rm -r -f -v ./.build/
