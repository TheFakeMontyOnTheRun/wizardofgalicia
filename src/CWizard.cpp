#include <memory>
#include <cstdlib>

#include "Vec2i.h"
#include "IMapElement.h"
#include "CActor.h"
#include "CWizard.h"

namespace WizardOfGalicia {
  CWizard::CWizard() : CActor() {
    emission = 20;
    view = '^';
    team = Team::HEROES;
    hp = 10000;
    attack = 2000;
    defence = 20000;
    magicEnergy = 20;
  }

  void CWizard::update(std::shared_ptr<CMap> map) {
    if ( abs( magicEnergy ) > 0.0f ) {
      magicEnergy += ( magicEnergy / abs( magicEnergy ) );
    }

    emission = abs( magicEnergy );
    attack = 4 + abs( magicEnergy );
    defence = 2 + abs( magicEnergy );
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
