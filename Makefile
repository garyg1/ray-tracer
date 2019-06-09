LIBS = -lglut -lGLU -lGL -lm
CFLAGS += -std=c++11 -O3 -I. -g

#Copied this makefile from stack overflow
#https://stackoverflow.com/questions/2908057/can-i-compile-all-cpp-files-in-src-to-os-in-obj-then-link-to-binary-in

SRC_DIR := src

OBJ_DIR := bin
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

raytracer: $(OBJ_FILES)
	g++ -o $@ $^ $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(SRC_DIR)/Settings.h
	g++ $(CFLAGS) -c -o $@ $<

clean:
	rm $(OBJ_DIR)/*.o
