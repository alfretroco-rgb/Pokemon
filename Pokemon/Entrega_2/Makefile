CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
TARGET = entrega2

all: $(TARGET)

$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o $(TARGET)

clean:
	del /Q $(TARGET).exe 2>nul || exit 0
