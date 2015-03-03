CXX = g++ #i586-mingw32msvc-c++ #compilar en windows?
CXXFLAGS = -Wall -g -std=c++11 #-O3
LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

DEPS = engine.h painter.h juego.h control.h level.h LTexture.h animation.h player.h enemies/enemy.h enemies/ghost.h enemies/skelleton.h enemies/demobat.h enemies/imp.h enemies/mask.h enemies/jack.h enemies/vlad.h enemies/cherubil.h enemies/priest.h particles.h door.h pickups.h points.h jukebox.h
OBJ = main.o engine.o painter.o juego.o control.o level.o LTexture.o animation.o player.o enemies/enemy.o enemies/ghost.o enemies/skelleton.o enemies/demobat.o enemies/imp.o particles.o door.o pickups.o enemies/mask.o enemies/jack.o points.o enemies/vlad.o enemies/cherubil.o enemies/priest.o jukebox.o

%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

main: $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LDFLAGS)

clean:
	$(RM) $(OBJ) main
