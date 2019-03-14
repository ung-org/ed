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
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <search.h>

void ed_edit(intmax_t start, intmax_t end, const char *arg)
{
	if (ed_state.dirty && ed_state.lastcmd != 'e') {
		ed_error("The current file has not been saved");
		return;
	}

	ed_Edit(start, end, arg);
}

void ed_Edit(intmax_t start, intmax_t end, const char *arg)
{
	struct ed_line *cursor = ed_state.head;
	while (cursor) {
		cursor = cursor->next;
		remque(cursor->prev);
	}
	ed_state.nlines = 0;
	ed_state.dirty = false;

	if (strlen(arg) > 0) {
		if (ed_state.filename) {
			free(ed_state.filename);
		}
		ed_state.filename = strdup(arg);
	}

	ed_state.current = NULL;
	ed_state.tail = NULL;
	ed_state.head = NULL;

	ed_read(start, end, arg);

	ed_state.current = ed_state.tail;
	ed_state.line = ed_state.nlines;
}

void ed_print(intmax_t start, intmax_t end, const char *arg)
{
	fputs(ed_state.current->text, stdout);
}

void ed_quit(intmax_t start, intmax_t end, const char *arg)
{
	if (ed_state.dirty && ed_state.lastcmd != 'q') {
		ed_error("The current file has not been saved");
		return;
	}
	ed_Quit(start, end, arg);
}

void ed_Quit(intmax_t start, intmax_t end, const char *arg)
{
	exit(0);
}

void ed_read(intmax_t start, intmax_t end, const char *arg)
{
	intmax_t total = 0;
	FILE *f = NULL;

	char *file = ed_state.filename;
	if (strlen(arg) > 0) {
		file = (char*)arg;
	}

	if (file[0] == '!') {
		f = popen(file + 1, "r");
	} else {
		f = fopen(file, "r");
	}

	if (f == NULL) {
		ed_error("Couldn't open %s: %s", ed_state.filename, strerror(errno));
		return;
	}

	while (!feof(f)) {
		struct ed_line *line = calloc(1, sizeof(*line));
		if (line == NULL) {
			ed_error("Out of memory");
			return;
		}

		size_t n = 0;
		total += getline(&(line->text), &n, f);

		ed_state.nlines++;
		insque(line, ed_state.current);
		ed_state.current = line;

		if (line->prev == NULL) {
			ed_state.head = line;
		}
	}

	if (file[0] == '!') {
		pclose(f);
	} else {
		fclose(f);
	}

	if (!ed_state.suppress) {
		printf("%"PRIdMAX"\n", total);
	}
}

void ed_write(intmax_t start, intmax_t end, const char *arg)
{
	intmax_t total = 0;
	FILE *f = NULL;

	char *file = ed_state.filename;
	if (strlen(arg) > 0) {
		file = (char*)arg;
	}

	if (file[0] == '!') {
		f = popen(file + 1, "w");
	} else {
		f = fopen(file, "w");
	}
	
	if (f == NULL) {
		ed_error("Couldn't open %s: %s", ed_state.filename, strerror(errno));
		return;
	}

	for (struct ed_line *cursor = ed_state.head; cursor; cursor = cursor->next) {
		fputs(cursor->text, f);
		total += strlen(cursor->text);
	}

	if (!ed_state.suppress) {
		printf("%"PRIdMAX"\n", total);
	}
}
