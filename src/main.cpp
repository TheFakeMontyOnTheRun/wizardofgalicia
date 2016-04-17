#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <map>
#include <SDL/SDL.h>

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
#endif

WizardOfGalicia::CGame game; 

void gameLoopTick() {
  game.tick();
}

int main ( int argc, char **argv ) {

  auto renderer = new WizardOfGalicia::CSDLRelativeRenderer();

  renderer->init();

  renderer->showTitleScreen();
  game.runGame( renderer, 1 );

#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop( gameLoopTick, 30, 1 );
#else
  while ( true ) {
    gameLoopTick();
  }
#endif

  renderer->shutdown();
  return 0;
}
