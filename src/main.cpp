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
  int level = 2;

  auto renderer = new WizardOfGalicia::CSDLRelativeRenderer();

  renderer->init();
  renderer->showTitleScreen();
  while ( level > 0 && level <= 2 ) {
    WizardOfGalicia::GameResult result = game.runGame( renderer, level );

    if ( result == WizardOfGalicia::GameResult::PlayerHasFinishedLevel ) {
      ++level;
    } else if ( result == WizardOfGalicia::GameResult::PlayerHasReturnedALevel ) {
      --level;
    } else {
      if ( result == WizardOfGalicia::GameResult::PlayerHasDied ) {
	renderer->showGameOverScreen();
      } else {

      }

      //should unify shutdown handling.
      return 0;
    }
  }

  renderer->showVictoryScreen();

  renderer->shutdown();
  return 0;
}
