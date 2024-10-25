BUILDDIR = build

SRCDIR = src

CFILES = $(wildcard $(SRCDIR)/*.c)
  
OFILES = $(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(CFILES))

CFLAGS = -Wall -Wextra -Werror

NAME = cunicode.a

all: $(BUILDDIR) $(NAME)

$(BUILDDIR):
	mkdir build

$(NAME): $(OFILES)
	ar rcs $(BUILDDIR)/$(NAME) $(OFILES) 

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	gcc -c $< -o $@ $(CFLAGS)

clean:
	rm build/*

.PHONY:  all clean