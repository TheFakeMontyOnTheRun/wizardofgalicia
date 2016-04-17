#include <memory>
#include <cstdlib>
#include <iostream>

#include "Vec2i.h"
#include "IMapElement.h"
#include "CActor.h"
#include "CWizard.h"

namespace WizardOfGalicia {
  CWizard::CWizard() : CActor() {
    emission = 20;
    view = '^';
    team = Team::HEROES;
    hp = 16;
    attack = 5;
    defence = 4;
    magicEnergy = 20;
  }

  void CWizard::update(std::shared_ptr<CMap> map) {
    if ( abs( magicEnergy ) > 0.0f ) {
      magicEnergy += ( magicEnergy / abs( magicEnergy ) );
    }

    emission = abs( magicEnergy ) / 50;
    attack = 4 + abs( magicEnergy ) / 50;
    defence = 2 + abs( magicEnergy ) / 50;
  }

  void CWizard::turnLeft() {
    CActor::turnLeft();
    updateView();
  }

  void CWizard::turnRight() {
    CActor::turnRight();
    updateView();
  }

  void CWizard::updateView() {
    switch (direction) {
    case Direction::N:
      view = '^';
      break;
    case Direction::W:
      view = '<';
      break;
    case Direction::E:
      view = '>';
      break;
    case Direction::S:
      view = 'V';
      break;
    }
  }
}
