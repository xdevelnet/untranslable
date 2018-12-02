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

struct replace_table {
	char from[4];
	char to[4];
	unsigned char to_size;
};

#define entry(a, b) { .from = a , .to = b , .to_size = strizeof( b ) }

const struct replace_table t1[] = {
	entry("E", "Е"),
	entry("e", "е"),
	entry("T", "Т"),
	entry("A", "А"),
	entry("a", "а"),
	entry("O", "О"),
	entry("o", "о"),
	entry("I", "І"),
	entry("i", "і"),
	entry("H", "Н"),
	entry("C", "С"),
	entry("c", "с"),
	entry("M", "М"),
	entry("y", "у"),
	entry("P", "Р"),
	entry("p", "р"),
	entry("B", "В"),
	entry("b", "ь"),
	entry("K", "К"),
	entry("k", "к"),
	entry("X", "х"),
	entry("x", "х"),
	{ .to_size = 0 }
};

const struct replace_table t2[] = {
	entry("О", "O"),
	entry("о", "o"),
	entry("Е", "E"),
	entry("е", "e"),
	entry("А", "A"),
	entry("а", "a"),
	entry("Н", "H"),
	entry("Т", "T"),
	//entry("т", "t"),
	entry("С", "C"),
	entry("і", "i"),
	entry("І", "I"),
	entry("с", "c"),
	entry("Р", "P"),
	entry("р", "p"),
	entry("В", "B"),
	entry("К", "K"),
	//entry("к", "k"),
	entry("М", "M"),
	entry("у", "y"),
	entry("Ы", "bI"),
	entry("Ь", "b"),
	//entry("ь", "b"),
	entry("З", "3"),
	entry("Х", "X"),
	entry("х", "x"),
	{ .to_size = 0 }
};

const struct replace_table t3[] = {
	{ .to_size = 0 }
};

const struct replace_table t4[] = {
	{ .to_size = 0 }
};

const struct replace_table * const tables[4] = { t1, t2, t3, t4 };
