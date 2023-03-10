SRC=spam_detector.c keywords.c
CFLAGS=-lm -Wall
EXEC=spam_detector
DEPS=utils.h

build: $(EXEC) $(DEPS)
$(EXEC): $(SRC) 
	gcc $(SRC) $(CFLAGS) -o $@
run: $(EXEC)
	./$(EXEC)

clean:
	rm -f $(EXEC)