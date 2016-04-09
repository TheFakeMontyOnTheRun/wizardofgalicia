#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>

#include "Vec2i.h"
#include "IMapElement.h"
#include "CActor.h"
#include "CDoorway.h"
#include "CMap.h"


#include "CWizard.h"
#include "IRenderer.h"
#include "CConsoleRenderer.h"
#include "CGame.h"



int main ( int argc, char **argv ) {

  WizardOfGalicia::CGame game; 
  int level = 1;

  while ( level > 0 && level <= 2 ) {
    WizardOfGalicia::GameResult result = game.runGame( new WizardOfGalicia::CConsoleRenderer(), level );

    if ( result == WizardOfGalicia::GameResult::PlayerHasFinishedLevel ) {
      ++level;
    } else if ( result == WizardOfGalicia::GameResult::PlayerHasReturnedALevel ) {
      --level;
    } else {
      return 0;
    }
  }
  return 0;
}
