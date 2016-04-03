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
  }

  void CWizard::update(std::shared_ptr<CMap> map) {
    
  }
}
