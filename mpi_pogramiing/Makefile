SRC = $(wildcard *.c)

# $(variable:patter=replacement) for every word in variable. it replace .c with nothing means only executable left

EXECS = $(SRC:.c=) 

#COMPILER 
MPICC ?= mpicc

all: $(EXECS)

#what we are building 

%: %.c
	$(MPICC) -o $@ $<

clean: 
	rm  $(EXECS)



