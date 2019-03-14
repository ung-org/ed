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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <search.h>

static struct ed_line *ed_input(void)
{
	struct ed_line *head = NULL;
	struct ed_line *tail = NULL;

	for (;;) {
		struct ed_line *line = calloc(1, sizeof(*line));
		size_t n;

		getline(&(line->text), &n, stdin);
		if (!strcmp(line->text, ".\n")) {
			free(line->text);
			free(line);
			break;
		}

		insque(line, tail);
		if (head == NULL) {
			head = line;
		}
		tail = line;
	}

	return head;
}

void ed_append(intmax_t start, intmax_t end, const char *arg) { }
void ed_change(intmax_t start, intmax_t end, const char *arg) { }

void ed_insert(intmax_t start, intmax_t end, const char *arg)
{
	struct ed_line *input = ed_input();
	insque(input, ed_state.current);
}
