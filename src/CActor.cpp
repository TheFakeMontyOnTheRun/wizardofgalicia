#include "Vec2i.h"
#include "IMapElement.h"
#include "CActor.h"

namespace WizardOfGalicia {
  CActor::CActor() {
    
    stance = STANDING;
    attack = 0;
    defence = 0;
    hp = 0;
    direction = N;
  }
  
  
  bool CActor::canMove() {
    return true;
  }

  bool CActor::canAttack() {
    return true;
  }
  
  void CActor::onMove() {
  }
  
  void CActor::onAttack() {
  }
  
  void CActor::endOfTurn() {
  }
  
  void CActor::performAttack( CActor &other ) {
    
    onAttack();
    
    int diff = ( attack - other.defence );
    
    if ( diff > 0 ) {
        other.hp -= diff;
    }
  }
}
