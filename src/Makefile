CC=gcc
COMP_FLAG=-Ofast -march=native -DNDEBUG
LIBS=-pthread -lpng
APP_SRC=algos.c \
		util_png.c \
		main.c


APP_OBJ = $(APP_SRC:%.c=%.o)

all:julia app

julia : julia.o
	$(CC) -o $@ $^ $(LIBS)

app : $(APP_OBJ)
	$(CC) -o $@ $^ $(LIBS)

%.o : %.c
	$(CC) $(COMP_FLAG) -o $@ -c $<

clean:
	rm -rf *.o julia
