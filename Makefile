CC = gcc
TARGET = term 
SRC = main.c cmd_utils.c ls.c cd.c tee.c cat.c
     
CFLAGS =
FLAGS =
LIBS =  -lpthread


OBJ = $(SRC:%.cpp=%.o)

%.cpp.o: 
	$(CC) -c $(CFLAGS) $<  -o $@ 
$(TARGET): $(OBJ)
	$(CC) $(FLAGS)  -o $(TARGET) $(OBJ) $(LIBS)
clean:
	rm *.o $(TARGET) 