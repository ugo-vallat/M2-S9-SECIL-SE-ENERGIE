CC=gcc
COMP_FLAG=-O0 -Wall
LIBS=-pthread -lpng
APP_SRC=algos.c \
		util_png.c \
		main.c


APP_OBJ = $(APP_SRC:%.c=%.o)

all:app

app : $(APP_OBJ)
	$(CC) -o $@ $^ $(LIBS)

%.o : %.c
	$(CC) $(COMP_FLAG) -o $@ -c $<

clean:
	rm -rf *.o app
