CXX=clang++
CXXFLAGS=-std=c++11
LDLIBS=-lsfml-graphics -lsfml-window -lsfml-system -lGLEW -lGL
DEPS=main.o Shader.o Vao.o Texture2D.o RenderBuffer.o FrameBuffer.o
EXEC=opengl-tuto

all: $(DEPS)
	$(CXX) $(CXXFLAGS) $(DEPS) $(LDLIBS) -o $(EXEC)

release: CXXFLAGS += -O3
release: all

exec: all
	./$(EXEC)

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

Shader.o: Shader.cpp Shader.hpp
	$(CXX) $(CXXFLAGS) -c Shader.cpp -o Shader.o

Vao.o: Vao.hpp Vao.cpp
	$(CXX) $(CXXFLAGS) -c Vao.cpp -o Vao.o

Texture2D.o: Texture2D.hpp Texture2D.cpp
	$(CXX) $(CXXFLAGS) -c Texture2D.cpp -o Texture2D.o

RenderBuffer.o: RenderBuffer.hpp RenderBuffer.cpp
	$(CXX) $(CXXFLAGS) -c RenderBuffer.cpp -o RenderBuffer.o

FrameBuffer.o: FrameBuffer.hpp FrameBuffer.cpp
	$(CXX) $(CXXFLAGS) -c FrameBuffer.cpp -o FrameBuffer.o