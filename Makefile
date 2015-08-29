vpath %.h ./
vpath %.c ./

all: ringbuff

objects := $(patsubst %.c,%.o,$(wildcard *.c))
#$(objects): %.o: %.c
#	$(CC) -c $(CFLAGS) $< -o $@

.PHONY: ringbuff
ringbuff: $(objects)
	$(CC) -o $@ $(objects)

test: 
	@echo test

.PHONY: clean
clean: 
	-rm ringbuff *.o

