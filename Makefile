CXX = g++
CXXFLAGS = -std=c++17 -O2
TARGET = face_cropper
SRC = src/main.cpp

OPENCV_FLAGS = `pkg-config --cflags --libs opencv4`

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(OPENCV_FLAGS)

clean:
	rm -f $(TARGET)
