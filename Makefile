CC = clang
CFLAGS = -Wall -Wextra  -Wconversion -Wdeprecated -Wpedantic -Wshadow -Wuninitialized -Wunused -Wcast-qual -Wfloat-equal -Wformat=2 -Winit-self -Wstrict-prototypes -Wswitch-default -Wswitch-enum -Wundef -Wunused-macros -Wwrite-strings -Wmissing-prototypes --std=gnu17


all: clean build

dev: clean build-dbg

build:
	mkdir build/
	$(CC) $(CFLAGS) client/main.c client/config.c client/crack.c client/calculateHash.c shared.c -o build/client -lcrypto -lbsd
	$(CC) $(CFLAGS) server/main.c server/config.c shared.c -o build/server -lbsd

build-dbg:
	mkdir build/
	$(CC) $(CFLAGS) -g client/main.c client/config.c client/log.c client/crack.c client/calculateHash.c shared.c -o build/client -lcrypto -lbsd
	$(CC) $(CFLAGS) -g server/main.c server/config.c server/log.c shared.c -o build/server -lcrypto -lbsd

clean:
	rm -rf build/

lint:
	find . -type f -name '*.c' -exec clang-tidy {} \;
