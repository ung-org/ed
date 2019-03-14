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

#define _XOPEN_SOURCE 700
#include <inttypes.h>
#include <stdbool.h>

struct ed_line {
	struct ed_line *next;
	struct ed_line *prev;
	char *text;
};

struct ed_state {
	bool suppress;
	bool dirty;
	char lastcmd;

	bool prompt_mode;
	char *prompt;

	bool help_mode;
	char *help;

	char *filename;

	intmax_t nlines;
	intmax_t line;
	intmax_t marks[26];
	struct ed_line *head;
	struct ed_line *tail;
	struct ed_line *current;
};
extern struct ed_state ed_state;

struct ed_command {
	int range_params;
	char *default_range1;
	char *default_range2;
	void (*fn)(intmax_t start, intmax_t end, const char *arg);
	enum {
		ARG_NONE = 0,
		ARG_FILE,
		ARG_RE,
		ARG_ADDRESS,
		ARG_COMMAND,
		ARG_MARK,
	} argtype;
};
extern struct ed_command ed_commands[];

void ed_command(char *cmd);
void ed_error(const char *fmt, ...);

void ed_sigint(int sig);
void ed_sighup(int sig);

void ed_append(intmax_t start, intmax_t end, const char *arg);
void ed_change(intmax_t start, intmax_t end, const char *arg);
void ed_delete(intmax_t start, intmax_t end, const char *arg);
void ed_edit(intmax_t start, intmax_t end, const char *arg);
void ed_Edit(intmax_t start, intmax_t end, const char *arg);
void ed_filename(intmax_t start, intmax_t end, const char *arg);
void ed_global(intmax_t start, intmax_t end, const char *arg);
void ed_Global(intmax_t start, intmax_t end, const char *arg);
void ed_help(intmax_t start, intmax_t end, const char *arg);
void ed_help_mode(intmax_t start, intmax_t end, const char *arg);
void ed_insert(intmax_t start, intmax_t end, const char *arg);
void ed_join(intmax_t start, intmax_t end, const char *arg);
void ed_mark(intmax_t start, intmax_t end, const char *arg);
void ed_list(intmax_t start, intmax_t end, const char *arg);
void ed_move(intmax_t start, intmax_t end, const char *arg);
void ed_number(intmax_t start, intmax_t end, const char *arg);
void ed_print(intmax_t start, intmax_t end, const char *arg);
void ed_prompt(intmax_t start, intmax_t end, const char *arg);
void ed_quit(intmax_t start, intmax_t end, const char *arg);
void ed_Quit(intmax_t start, intmax_t end, const char *arg);
void ed_read(intmax_t start, intmax_t end, const char *arg);
void ed_substitute(intmax_t start, intmax_t end, const char *arg);
void ed_copy(intmax_t start, intmax_t end, const char *arg);
void ed_undo(intmax_t start, intmax_t end, const char *arg);
void ed_global_non_matched(intmax_t start, intmax_t end, const char *arg);
void ed_Global_non_matched(intmax_t start, intmax_t end, const char *arg);
void ed_write(intmax_t start, intmax_t end, const char *arg);
void ed_line_number(intmax_t start, intmax_t end, const char *arg);
void ed_shell_escape(intmax_t start, intmax_t end, const char *arg);
void ed_null(intmax_t start, intmax_t end, const char *arg);
