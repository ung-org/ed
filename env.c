/*
 * UNG's Not GNU
 *
 * Copyright (c) 2011-2017, Jakob Kaivo <jkk@ung.org>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

#include "ed.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ed_error(const char *fmt, ...)
{
	if (ed_state.help) {
		free(ed_state.help);
	}

	va_list ap, ap2;
	va_start(ap, fmt);
	va_copy(ap2, ap);
	size_t n = vsnprintf(NULL, 0, fmt, ap);

	ed_state.help = calloc(n + 2, 1);
	if (ed_state.help == NULL) {
		fprintf(stderr, "ed: Out of memory\n");
		exit(1);
	}

	vsnprintf(ed_state.help, n + 1, fmt, ap2);
	va_end(ap);
	va_end(ap2);

	puts("?");

	if (ed_state.help_mode) {
		puts(ed_state.help);
	}
}

void ed_filename(intmax_t start, intmax_t end, const char *arg)
{
	if (ed_state.filename) {
		free(ed_state.filename);
	}

	ed_state.filename = strdup(arg);
}

void ed_help(intmax_t start, intmax_t end, const char *arg)
{
	if (ed_state.help) {
		puts(ed_state.help);
	}
}

void ed_help_mode(intmax_t start, intmax_t end, const char *arg)
{
	ed_state.help_mode = !ed_state.help_mode;
	if (ed_state.help_mode) {
		ed_help(start, end, arg);
	}
}

void ed_prompt(intmax_t start, intmax_t end, const char *arg)
{
	if (ed_state.prompt == NULL) {
		ed_state.prompt = "*";
	}

	ed_state.prompt_mode = !ed_state.prompt_mode;
}
