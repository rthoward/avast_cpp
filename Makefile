SOURCES = $(wildcard src/*.cpp)
OBJS = $(SOURCES:.cpp=.o)
LIBFLAGS = -Iinclude -L. -ltcod_debug -ltcodxx_debug -Wl,-rpath=.
CC = clang++
CFLAGS = -Wall -g

tuto: $(OBJS)
	$(CC) $(CFLAGS) $(LIBFLAGS) $(OBJS) -o tuto

src/%.o : src/%.cpp
	$(CC) $< -c -o $@ -Iinclude -Wall -g

clean :
	rm tuto src/*.o
