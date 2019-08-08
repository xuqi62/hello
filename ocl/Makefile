CC := aarch64-linux-android-gcc

SRC := test.cpp
OBJ := test.o

TARGET:= test

COMPILE_FLAGS =  -fPIE -pie -fPIC -g -O0 -I.
LINK_FLAGS = -lc  -lm  -Wl,--allow-shlib-undefined -fPIE -pie -lOpenCL -L./lib_android/android-aarch64
.PHONY:all  clean

all: $(TARGET)
$(OBJ): %.o:%.cpp
	$(CC) -c $< -o $@  $(COMPILE_FLAGS)
$(TARGET): $(OBJ)
	$(CC) -o $@ $(OBJ)  $(LINK_FLAGS) 

clean:
	rm -rf $(TARGET) $(OBJ)
