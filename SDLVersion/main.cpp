#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <map>
#include <SDL/SDL.h>
#include <sstream>
#include "Vec2i.h"
#include "IMapElement.h"
#include "CActor.h"
#include "CDoorway.h"
#include "CWizard.h"
#include "CMap.h"

#include "IRenderer.h"
#include "CConsoleRenderer.h"
#include "CSDLRelativeRenderer.h"
#include "CGame.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

WizardOfGalicia::CGame game; 

void gameLoopTick() {
  game.tick();
}

int main ( int argc, char **argv ) {

  auto renderer = new WizardOfGalicia::CSDLRelativeRenderer();

  renderer->init( game.map );

  renderer->showTitleScreen();

  std::ifstream data( "res/map1.txt" );
  
  std::string line;
  std::stringstream ss;

  while ( std::getline( data, line ) ) {
    ss << line;
  }

  game.runGame( renderer, ss.str() );

#ifdef __EMSCRIPTEN__
  //  emscripten_request_fullscreen(0, 1);
  emscripten_set_main_loop( gameLoopTick, 30, 1 );
#else
  while ( true ) {
    gameLoopTick();
    SDL_Delay(33);
  }
#endif

  renderer->shutdown();
  return 0;
}
