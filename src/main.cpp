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

int main ( int argc, char **argv ) {

  WizardOfGalicia::CGame game; 

  auto renderer = new WizardOfGalicia::CSDLRelativeRenderer();

  renderer->init();
  renderer->showTitleScreen();
  game.runGame( renderer, 1 );

  while ( true ) {
    WizardOfGalicia::GameResult result = game.tick();
  }

  renderer->shutdown();
  return 0;
}
