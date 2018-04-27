SRC := select_test.c
OBJ := select_test.o

TARGET:= test

.PHONY:all  clean

all: $(TARGET)
$(OBJ): %.o:%.c
	gcc -c $< -o $@
$(TARGET): $(OBJ)
	gcc -o $@ $(OBJ)

clean:
	rm -rf $(TARGET) $(OBJ)
