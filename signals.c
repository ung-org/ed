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
#include <unistd.h>

void ed_sigint(int sig)
{
}

void ed_sighup(int sig)
{
	const char *filename = "ed.hup";

	if (!ed_state.dirty) {
		exit(sig);
	}

	if (access(filename, W_OK) == 0) {
		ed_write(1, ed_state.nlines, filename);
		exit(sig);
	}

	char *home = getenv("HOME");
	if (home == NULL) {
		exit(sig);
	}

	char path[strlen(home) + strlen(filename) + 2];
	sprintf(path, "%s/%s", home, filename);
	if (access(path, W_OK) == 0) {
		ed_write(1, ed_state.nlines, path);
	}

	exit(sig);
}
