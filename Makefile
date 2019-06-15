# Stellar Fury - 2D Space Shooter
# Makefile for macOS/Linux

CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
LDFLAGS = $(shell sdl2-config --cflags --libs)

TARGET = stellar_fury
SRCS = main.cpp Game.cpp Entity.cpp Player.cpp Enemy.cpp Bullet.cpp Particle.cpp Starfield.cpp HUD.cpp Boss.cpp PowerUp.cpp
OBJS = $(SRCS:.cpp=.o)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(LDFLAGS)

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)
