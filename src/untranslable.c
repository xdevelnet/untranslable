/*
 * Copyright (c) 2018, Xdevelnet (xdevelnet at xdevelnet dot org)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#define _XOPEN_SOURCE 700 // duh, POSIX.1-2008, blablabla, openat(), blablabla

#include <unistd.h>
#include <stdlib.h>
#include <iso646.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define strizeof(a) (sizeof(a)-1)
#include "rodata.h"

#if !defined deprintf
	#if defined DEBUG_PRINTS
		#define deprintf(a, ...) fprintf(stderr, a, __VA_ARGS__)
	#else
		#define deprintf(...)
	#endif
#endif

#if !defined PATH_SEPARATOR
	#if defined(WIN32) || defined(_WIN32) 
		#define PATH_SEPARATOR '\\'
	#else
		#define PATH_SEPARATOR '/'
	#endif
#endif

unsigned char utf8_byte_width(const void *ptr) {
	
	// |----------|----------|----------|----------| 
	// |     1    |     2    |     3    |     4    | |----------|-----|
	// |----------+----------+----------+----------| | 11000000 | 192 |
	// | 0xxxxxxx |          |          |          | |----------+-----|
	// |----------+----------+----------+----------| | 11100000 | 224 |
	// | 110xxxxx | 10xxxxxx |          |          | |----------+-----|
	// |----------+----------|----------+----------| | 11110000 | 240 |
	// | 1110xxxx | 10xxxxxx | 10xxxxxx |          | |----------+-----|
	// |----------+----------+----------+----------| | 11111100 | 252 |
	// | 11110xxx | 10xxxxxx | 10xxxxxx | 10xxxxxx | |----------|-----|
	// |----------|----------|----------|----------|

	const unsigned char *utf = ptr;

	if (*utf < 192) return 1;
	if (*utf < 224) return 2;
	if (*utf < 240) return 3;
	if (*utf < 252) return 4;
	return 1;
}

int prepare_wfile(const char *rfilename) {
	size_t namelen = strlen(rfilename);
	const char nameprefix[] = "u.";
	char newfname[namelen + strizeof(nameprefix) + 1]; // ye, I can use alloca() instead, but fuck off
	int fd;
	char *lastslash = strrchr(rfilename, PATH_SEPARATOR);
	if (lastslash == NULL) {
		memcpy(newfname, nameprefix, strizeof(nameprefix));
		memcpy(newfname + strizeof(nameprefix), rfilename, namelen + 1); // copy with null terminator
		deprintf("Filename for write: %s\n", newfname);
		fd = open(newfname, O_CREAT | O_TRUNC | O_WRONLY, 0644);
		if (fd < 0) return perror(newfname), fd;
	
	} else {
		memcpy(newfname, rfilename, lastslash - rfilename);
		newfname[lastslash - rfilename] = '\0';
		int dfd;
		// Just in case... The following shitty^W shiny-modern code could be replaced with
		// older DIR *dir = opendir(); dfd = dirfd(dir);
		if ((dfd = open(newfname, O_RDONLY)) < 0) return perror(newfname), dfd;
		// yes, I know that O_DIRECTORY exists. But it's Linux-specific. IDC about GNU std, I care
		// only about POSIX std
		memcpy(newfname, nameprefix, strizeof(nameprefix));
		memcpy(newfname + strizeof(nameprefix), lastslash + 1, namelen - (lastslash - rfilename));
		// Last part of expression above is calculation of filename lenght including null terminator.
		// You probably should pick up a whiteboard to check all deez path-related variables.
		if ((fd = openat(dfd, newfname, O_CREAT | O_WRONLY, 0644)) < 0) return close(dfd), perror(newfname), fd;
		// The overall bicycle-reinventing process is exist because of ugly avoiding rare TOCTOU cases. Welp.
	}
	return fd;
}

struct utf8_glyph {
	char bytes[4];
	unsigned char width;
};

void glyph_replacer(struct utf8_glyph *glyph) {
	const struct replace_table *mytable = *(tables + glyph->width - 1); // selecting correct table
	size_t pos = 0;
	while(mytable[pos].to_size != 0) {
		if (memcmp(mytable[pos].from, glyph->bytes, glyph->width) == 0) {
			memcpy(glyph->bytes, mytable[pos].to, mytable[pos].to_size);
			glyph->width = mytable[pos].to_size;
			return;
		}
		pos++;
	}
}

int main(int argc, char **argv) {
	if (argc < 2) return EXIT_FAILURE;
	int fd = open(argv[1], O_RDONLY);
	if (fd < 0) return perror(argv[1]), EXIT_FAILURE;
	int wfd = prepare_wfile(argv[1]);
	if (wfd < 0) return close(fd), EXIT_FAILURE;

	char buffer[11];
	ssize_t got;
	size_t pos = 0;
	size_t shift = 0;
	while(1) {
		got = read(fd, buffer + shift, sizeof(buffer) - shift) + shift;
		if (got < 0) {
			perror(argv[1]);
			break;
		}
		if (got == 0) break;
		shift = 0;
		while(pos < (size_t) got) {
			unsigned char byterange = utf8_byte_width(buffer + pos);
			if (pos + byterange > (size_t) got) {
				shift = got - pos;
				memmove(buffer, buffer + pos, shift);
				break;
			}
			struct utf8_glyph replacing;
			replacing.width = byterange;
			memcpy(replacing.bytes, buffer + pos, replacing.width);
			glyph_replacer(&replacing);
			write(wfd, replacing.bytes, replacing.width);
			pos += byterange;
		}
		pos = 0;
	}
	close(fd);
	close(wfd);
	return EXIT_SUCCESS;
}
