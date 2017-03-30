CC = gcc
CFLAGS = -Wall
OBJS = src/Compress.o src/Decompress.o src/main.o
OUT = FileCompressing.exe

OUT: $(OBJS)
	$(CC) $(OBJS) -o $(OUT)

src/%.o: src/%.c
	$(CC) $< $(CFLAGS) -c -o $@

.PHONY:
clean:  
	rm src/*.o