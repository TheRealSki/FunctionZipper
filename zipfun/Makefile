CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -fPIC
LIB_NAME = libzipfun
TARGET = $(LIB_NAME).a
SOURCES = zipfun.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	ar rcs $(TARGET) $(OBJECTS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET) *.enc

install: $(TARGET)
	cp $(TARGET) /usr/local/lib/
	cp zipfun.h /usr/local/include/

.PHONY: all clean install 