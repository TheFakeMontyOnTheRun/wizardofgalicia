CXX = clang++

SDL_LIB = -L/usr/lib -lSDL -lSDL_gfx -lSDL_image -lSDL_mixer
SDL_INCLUDE = -I/usr/local/include `sdl-config --cflags --libs`

CXXFLAGS = -std=c++0x -O2 -g -Wall -fno-exceptions -fno-rtti -Bstatic -Weffc++ -fmessage-length=0 $(SDL_INCLUDE) -Iinclude
OBJS =	src/main.o src/CActor.o  src/CBaphomet.o  src/CConsoleRelativeRenderer.o  src/CCuco.o  src/CDemon.o  src/CDoorway.o  src/CGame.o src/CMap.o src/CMoura.o  src/CWizard.o  src/IMapElement.o  src/Vec2i.o src/CFireball.o src/CRelic.o src/CSDLRelativeRenderer.o src/CSDLShowImageRenderer.o
LIBS = $(SDL_LIB)
TARGET = wizard

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:   $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
