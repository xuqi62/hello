#CC := aarch64-linux-android-gcc
CC := arm-linux-androideabi-gcc

SRC := test.c
OBJ := test.o

TARGET:= test_ion

COMPILE_FLAGS =  -fPIE -pie -fPIC -fstack-protector-all -s -D_FORTIFY_SOURCE=2 -I.
LINK_FLAGS = -lc  -lm  -Wl,--allow-shlib-undefined -fPIE -pie 
.PHONY:all  clean

all: $(TARGET)
$(OBJ): %.o:%.c
	$(CC) -c $< -o $@  $(COMPILE_FLAGS)
$(TARGET): $(OBJ)
	$(CC) -o $@ $(OBJ)  $(LINK_FLAGS) 

clean:
	rm -rf $(TARGET) $(OBJ)
