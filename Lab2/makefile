sort_out = sort

sort_headers = funciones.h
sort_source  = $(sort_headers:.h=.c) sort.c
sort_objects = $(sort_source:.c=.o)

CC     = gcc
CFLAGS = -Wno-unknown-pragmas -Wall

depends = .depends

build : $(sort_out) 

$(sort_out) : $(sort_objects)
	$(CC) $(CFLAGS) -o $@ $^ -lm -fopenmp

$(objects) :
	$(CC) $(CFLAGS) -c -o $@ $*.c -fopenmp

$(depends) : $(sort_source) $(sort_headers)
	@$(CC) -MM $(sort_source) > $@


clean :
	$(RM) $(sort_out) $(sort_objects) $(zipfile) $(depends)

.PHONY : build zip clean

sinclude $(depends)