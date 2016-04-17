CXX = em++
CXXFLAGS = -g  --preload-file res --use-preload-plugins -Iinclude -std=c++1y

OBJS =	src/main.o src/CActor.o  src/CBaphomet.o  src/CConsoleRelativeRenderer.o  src/CCuco.o  src/CDemon.o  src/CDoorway.o  src/CGame.o src/CMap.o src/CMoura.o  src/CWizard.o  src/IMapElement.o  src/Vec2i.o src/CFireball.o src/CRelic.o src/CSDLRelativeRenderer.o


TARGET = wizard.html

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(CXXFLAGS)

all:   $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
