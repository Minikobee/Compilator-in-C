OBJS=scanner.o parser.o TP1.o TP2.o TP2main.o
INTERM=scanner.c parser.c parser.h
PROGRAM=./TP2main
CFLAGS=-g

all: $(PROGRAM)

scanner.c: scanner.flex parser.h
	flex -o scanner.c scanner.flex

parser.c parser.h: parser.bison
	bison --defines=parser.h -o parser.c parser.bison

$(PROGRAM): $(OBJS)
	$(CC) -o $(PROGRAM) $(OBJS)
clean:
	rm -f $(PROGRAM) $(OBJS) $(INTERM) MipsCode.txt

test: $(PROGRAM)
	$(PROGRAM) example.txt
