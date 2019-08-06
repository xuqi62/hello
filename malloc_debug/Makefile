CC := aarch64-linux-android-gcc

SRC := test.cpp
OBJ := test.o

TARGET:= test

COMPILE_FLAGS =  -fPIE -pie -fPIC -g -O0
LINK_FLAGS = -lc -lc_malloc_debug -lm -L./debug_memory -Wl,--allow-shlib-undefined -fPIE -pie
.PHONY:all  clean

all: $(TARGET)
$(OBJ): %.o:%.cpp
	$(CC) -c $< -o $@  $(COMPILE_FLAGS)
$(TARGET): $(OBJ)
	$(CC) -o $@ $(OBJ)  $(LINK_FLAGS) 

clean:
	rm -rf $(TARGET) $(OBJ)
