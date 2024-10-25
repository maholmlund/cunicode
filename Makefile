BUILDDIR = build

SRCDIR = src

TESTDIR = tests

CFILES = $(wildcard $(SRCDIR)/*.c)
  
OFILES = $(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(CFILES))

TESTFILES = $(wildcard $(TESTDIR)/*.c)

TESTOFILES = $(patsubst $(TESTDIR)/%.c, $(BUILDDIR)/%.o, $(TESTFILES))

CFLAGS = -Wall -Wextra -Werror

NAME = cunicode.a
TESTNAME = test

all: $(BUILDDIR) $(NAME)

$(BUILDDIR):
	mkdir build

$(NAME): $(OFILES)
	ar rcs $(BUILDDIR)/$(NAME) $(OFILES) 

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	gcc -c $< -o $@ $(CFLAGS)

$(BUILDDIR)/%.o: $(TESTDIR)/%.c
	gcc -c $< -o $@ $(CFLAGS)

test: all $(TESTOFILES)
	gcc $(TESTOFILES) $(BUILDDIR)/$(NAME) -o $(BUILDDIR)/test -lcriterion
	$(BUILDDIR)/$(TESTNAME)

clean:
	rm build/*

.PHONY:  all clean