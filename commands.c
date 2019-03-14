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
#include <stddef.h>
#include <string.h>
#include <ctype.h>

#define D "."	/* [D]ot */
#define S "1"	/* [S]tart */
#define E "$"	/* [E]nd */
#define P ".+1"	/* [P]lus one */

struct ed_command ed_commands[] = {
	['a'] = { 1, D, 0,	ed_append,		ARG_NONE },
	['c'] = { 2, D, D,	ed_change,		ARG_NONE },
	['d'] = { 2, D, D,	ed_delete,		ARG_NONE },
	['e'] = { 0, 0, 0,	ed_edit,		ARG_FILE },
	['E'] = { 0, 0, 0,	ed_Edit,		ARG_FILE },
	['f'] = { 0, 0, 0,	ed_filename,		ARG_FILE },
	['g'] = { 2, S, E,	ed_global,		ARG_RE },
	['G'] = { 2, S, E,	ed_Global,		ARG_RE },
	['h'] = { 0, 0, 0,	ed_help,		ARG_NONE },
	['H'] = { 0, 0, 0,	ed_help_mode,		ARG_NONE },
	['i'] = { 1, D, 0,	ed_insert,		ARG_NONE },
	['j'] = { 2, D, P,	ed_join,		ARG_NONE },
	['k'] = { 1, D, 0,	ed_mark,		ARG_MARK },
	['l'] = { 2, D, D,	ed_list,		ARG_NONE },
	['m'] = { 2, D, D,	ed_move,		ARG_ADDRESS },
	['n'] = { 2, D, D, 	ed_number,		ARG_NONE },
	['p'] = { 2, D, D, 	ed_print,		ARG_NONE },
	['P'] = { 0, 0, 0,	ed_prompt,		ARG_NONE },
	['q'] = { 0, 0, 0,	ed_quit,		ARG_NONE },
	['Q'] = { 0, 0, 0,	ed_Quit,		ARG_NONE },
	['r'] = { 1, E, 0,	ed_read,		ARG_FILE },
	['s'] = { 2, D, D,	ed_substitute,		ARG_RE },
	['t'] = { 2, D, D,	ed_copy,		ARG_ADDRESS },
	['u'] = { 0, 0, 0,	ed_undo,		ARG_NONE },
	['v'] = { 2, S, E,	ed_global_non_matched,	ARG_RE },
	['V'] = { 2, S, E,	ed_Global_non_matched,	ARG_RE },
	['w'] = { 2, S, E,	ed_write,		ARG_FILE },
	['='] = { 1, E, 0,	ed_line_number,		ARG_NONE },
	['!'] = { 0, 0, 0,	ed_shell_escape,	ARG_COMMAND },
	['\0'] = { 1, P, 0,	ed_null,		ARG_NONE },
};
static int ncommands = sizeof(ed_commands) / sizeof(ed_commands[0]);

static intmax_t ed_range(char *cmdline, char **next)
{
	intmax_t line = 0;
	int advance = 0;
	if (cmdline == 0) {
		if (next) {
			*next = NULL;
		}
		return 0;
	}

	switch (cmdline[0]) {
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
		line = strtoimax(cmdline, next, 10);
		break;

	case '.':
		advance = 1;
		line = ed_state.line;
		break;

	case '$':
		advance = 1;
		line = ed_state.nlines;
		break;

	case '\'':
		if (!islower(cmdline[1])) {
			ed_error("'%c' is not a valid mark name", cmdline[1]);
			return -1;
		}

		if (ed_state.marks['a' - cmdline[1]] == 0) {
			ed_error("Mark '%c' is not set", cmdline[1]);
			return -1;
		}

		/* TODO: marked line is not currently present in the edit buffer */

		advance = 2;
		line = ed_state.marks['a' - cmdline[1]];
		break;

	case '/':
		/* TODO: RE starting from line 1 */
		advance = 1;
		line = 1;
		break;

	case '?':
		/* TODO: RE starting from end */
		advance = 1;
		line = ed_state.nlines;
		break;

	case '+':
	case '-':
		if (!isdigit(cmdline[1])) {
			advance = 1;
			line = 1;
			break;
		}
		line = ed_state.line + strtoimax(cmdline, next, 10);
		break;

	default:
		*next = cmdline;
		break;
	}

	/* TODO: Check for + or - */

	if (next) {
		*next = cmdline + advance;
	}
	return line;
}

void ed_command(char *cmdline)
{
	char *next = NULL;
	cmdline[strlen(cmdline)-1] = '\0';
	while (isblank(*cmdline)) {
		cmdline++;
	}

	intmax_t begin = ed_range(cmdline, &next);
	if (begin == -1) {
		return;
	}

	while (isblank(*next)) {
		next++;
	}

	char sep = *next;

	if (sep == ',' || sep == ';') {
		next++;
	}

	while (isblank(*next)) {
		next++;
	}

	intmax_t end = ed_range(next, &next);

	if (begin == 0 && sep == ',' && end == 0) {
		begin = 1;
		end = ed_state.nlines;
	} else if (begin == 0 && sep == ',') {
		begin = 1;
	} else if (sep == ',' && end == 0) {
		end = begin;
	} else if (begin == 0 && sep == ';' && end == 0) {
		begin = ed_state.line;
		end = ed_state.nlines;
	} else if (begin == 0 && sep == ';') {
		begin = ed_state.line;
	} else if (sep == ';' && end == 0) {
		end = begin;
	}

	while (isblank(*next)) {
		next++;
	}

	int cmd = next[0];
	if (cmd > ncommands || ed_commands[cmd].fn == 0) {
		ed_error("%c: no such command", cmd);
		return;
	}

	if (sep != ',' && sep != ';') {
		if (begin == 0) {
			begin = ed_range(ed_commands[cmd].default_range1, NULL);
		}
		if (end == 0) {
			end = ed_range(ed_commands[cmd].default_range2, NULL);
		}
	}

	/* TODO: validate parameters */
	/* TODO: suffix l, n, or p */

	ed_commands[cmd].fn(begin, end, next + 1);
	ed_state.lastcmd = cmd;
}
