TARGETS=p3

all: $(TARGETS)

clean:
	rm -f $(TARGETS)

%: %.c
	gcc -Wall -o $@ $<
