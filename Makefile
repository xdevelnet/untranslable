.PHONY: all clean
all:
	cc --std=c99 -Wall -Wno-unused-variable -Wno-unused-result -Werror -O2 src/untranslable.c -I./src -o untranslable
clean:
	rm -f untranslable
