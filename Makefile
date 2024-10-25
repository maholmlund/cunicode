BUILDDIR = build
SRCDIR = src
TESTDIR = tests

SRCFILES = $(wildcard $(SRCDIR)/*.c)
SRCOFILES = $(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(SRCFILES))
TESTFILES = $(wildcard $(TESTDIR)/*.c)
TESTOFILES = $(patsubst $(TESTDIR)/%.c, $(BUILDDIR)/%.o, $(TESTFILES))

CFLAGS = -Wall -Wextra -Werror
CC = gcc

NAME = $(BUILDDIR)/cunicode.a
TESTNAME = $(BUILDDIR)/test

all: $(BUILDDIR) $(NAME)

$(BUILDDIR):
	mkdir -p $@

$(NAME): $(SRCOFILES)
	ar rcs $@ $^

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

$(BUILDDIR)/%.o: $(TESTDIR)/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

test: all $(TESTOFILES)
	$(CC) $(TESTOFILES) $(NAME) -o $(TESTNAME) -lcriterion
	$(TESTNAME)

clean:
	rm build/*

.PHONY:  all clean test