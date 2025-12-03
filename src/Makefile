CC=gcc
COMP_FLAG=-O3
LIBS=-pthread
all:julia


julia : julia.o
	$(CC) -o $@ $^ $(LIBS)


%.o : %.c
	$(CC) $(COMP_FLAG) -o $@ -c $<

clean:
	rm -rf *.o julia