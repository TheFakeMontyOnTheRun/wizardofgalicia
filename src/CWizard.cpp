#include <memory>

#include "Vec2i.h"
#include "IMapElement.h"
#include "CActor.h"
#include "CWizard.h"

namespace WizardOfGalicia {
  CWizard::CWizard() : CActor() {
    view = '^';
    team = Team::HEROES;
    hp = 5;
    attack = 4;
    defence = 1;
    magicEnergy = 20;
  }

  void CWizard::update(std::shared_ptr<CMap> map) {
    magicEnergy += ( magicEnergy / abs( magicEnergy ) );
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
