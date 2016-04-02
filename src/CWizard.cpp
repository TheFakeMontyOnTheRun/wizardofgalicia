#include "Vec2i.h"
#include "IMapElement.h"
#include "CActor.h"
#include "CWizard.h"

namespace WizardOfGalicia {
  CWizard::CWizard() : CActor() {
    view = '^';
    team = VILLAINS;
    hp = 5;
    attack = 4;
    defence = 1;
  }
}
