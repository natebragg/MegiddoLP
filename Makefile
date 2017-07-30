CC = gcc

OUTDIR = build
SRCDIR = src
EXDIR = examples
EXAMPLES = $(OUTDIR)/lemonade
TARGET = $(OUTDIR)/libmegiddolp.a
OBJS = $(OUTDIR)/clp_wrapper.o

CPPFLAGS = -Iinclude
CFLAGS = -std=c89 -Wall -Werror -pedantic
ARFLAGS = rcvs

all: $(TARGET)
.PHONY: all

$(TARGET): $(OBJS)
	@mkdir -p $(OUTDIR)
	$(AR) $(ARFLAGS) $@ $^

$(OBJS): $(OUTDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OUTDIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

examples: $(EXAMPLES)
.PHONY: examples

$(EXAMPLES): $(TARGET)
$(EXAMPLES): $(OUTDIR)/%: $(EXDIR)/%.c
	@mkdir -p $(OUTDIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $< $(TARGET) -o $@

clean:
	rm -rf $(OUTDIR)
.PHONY: clean