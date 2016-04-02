#include "Vec2i.h"
#include "IMapElement.h"
#include "CActor.h"
#include "CCuco.h"

namespace WizardOfGalicia {
  
  CCuco::CCuco() : CActor() {
    view = '@';
    team = VILLAINS;
    hp = 5;
    attack = 4;
    defence = 1;
  }
}
