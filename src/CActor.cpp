#include <memory>

#include "Vec2i.h"
#include "IMapElement.h"
#include "CActor.h"

namespace WizardOfGalicia {
  CActor::CActor() {
    
    stance = Stance::STANDING;
    attack = 0;
    defence = 0;
    hp = 0;
    direction = Direction::N;
  }
  
  
  bool CActor::canMove() {
    return true;
  }

  bool CActor::canAttack() {
    return true;
  }

  void CActor::turnLeft() {
    
    if ( direction == Direction::N ) {
      direction = Direction::W;
    } else {
      direction = static_cast<Direction>( static_cast<int>(direction) - 1);
    }
  }
  
  void CActor::turnRight() {
    if ( direction == Direction::W ) {
      direction = Direction::N;
    } else {
      direction = static_cast<Direction>( static_cast<int>(direction) + 1);
    }
  }

  void CActor::onMove() {
  }
  
  void CActor::onAttack() {
  }
  
  void CActor::endOfTurn() {
  }
  
  void CActor::performAttack( std::shared_ptr<CActor> other ) {
    
    if ( abs( magicEnergy ) < abs( other->magicEnergy ) ) {
      return;
    }

    onAttack();
    
    int diff = ( attack - other->defence );
    
    if ( diff > 0 ) {
        other->hp -= diff;
    }

    int deltaEnergy = (magicEnergy + other->magicEnergy) / 2;

    magicEnergy += deltaEnergy;
  }
}
