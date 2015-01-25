CXX = g++
CXXFLAGS = -Wall -g -std=c++11
LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf

DEPS = engine.h painter.h juego.h control.h level.h LTexture.h animation.h player.h enemy.h ghost.h
OBJ = main.o engine.o painter.o juego.o control.o level.o LTexture.o animation.o player.o enemy.o ghost.o

%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

main: $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LDFLAGS)

clean:
	$(RM) $(OBJ) main
