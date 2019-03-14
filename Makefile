.POSIX:

.SUFFIXES: .cat .msg

default: all

CFLAGS=-g -Wall -Wextra -Wpedantic -Werror -Wno-unused-parameter
UTILITY=ed
SOURCES=ed.c commands.c stubs.c env.c io.c signals.c input.c re.c
HEADERS=ed.h
OBJECTS=ed.o commands.o stubs.o env.o io.o signals.o input.o re.o
L10N=
all: $(UTILITY) $(L10N)

$(UTILITY): $(OBJECTS) $(HEADERS)

.msg.cat:
	gencat $@ $<

.c.cat:
	sed -ne '/^\/\*\*cat/,/cat\*\*\//p;' $< | grep -v ^/ | grep -v ^\* | gencat $@ -

clean:
	rm -f *.o $(L10N) $(UTILITY)
