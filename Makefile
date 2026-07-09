PREFIX  ?= /usr/local
DESTDIR ?=

CC     ?= gcc
AR     ?= ar
RANLIB ?= ranlib

CFLAGS += -Wall -Wextra -Wpedantic -Werror -std=c11 -O3
CPPFLAGS += -Iinclude

NAME := b4se
LIB  := lib$(NAME).a

INCLUDEDIR := $(DESTDIR)$(PREFIX)/include/$(NAME)
LIBDIR     := $(DESTDIR)$(PREFIX)/lib

MODULES := $(filter-out include,$(filter %/,$(wildcard */)))
SRCS    := $(foreach m,$(MODULES),$(wildcard $(m)*.c))
OBJS    := $(SRCS:.c=.o)

PUBLIC_HEADERS := $(wildcard include/*.h)

all: $(LIB)

$(LIB): $(OBJS)
	$(AR) rcs $@ $^
	$(RANLIB) $@

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

install: $(LIB)
	install -d $(INCLUDEDIR)
	install -d $(LIBDIR)

	install -m 644 $(PUBLIC_HEADERS) $(INCLUDEDIR)/
	install -m 644 $(LIB) $(LIBDIR)/

uninstall:
	rm -f $(addprefix $(INCLUDEDIR)/,$(notdir $(PUBLIC_HEADERS)))
	rm -f $(LIBDIR)/$(LIB)

clean:
	rm -f $(OBJS) $(LIB)

.PHONY: all install uninstall clean
