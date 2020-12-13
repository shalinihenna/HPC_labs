TARGETS = suma2D suma2Dshm

CC_C = $(CROSS_TOOL)nvcc


all: clean $(TARGETS)

$(TARGETS):
	$(CC_C) $@.cu -o $@

clean:
	rm -f $(TARGETS)