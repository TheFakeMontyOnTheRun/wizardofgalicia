#include <memory>
#include <vector>

#include "Vec2i.h"
#include "IMapElement.h"
#include "CActor.h"
#include "CFireball.h"
#include "CDoorway.h"
#include "CMap.h"

namespace WizardOfGalicia {
  CFireball::CFireball(Direction d, const Vec2i& p ) : CActor() {
    view = '*';
    team = Team::HEROES;
    hp = 50;
    direction = d;
    position = p;
    attack = 10;
    defence = 0;
  }

  void CFireball::update(std::shared_ptr<CMap> map) {
    Vec2i previous = Vec2i( position );
    map->move( direction, shared_from_this() ); 
    if ( previous.x == position.x && previous.y == position.y ) {
      hp = 0;
      attack = 0;
    }
  }
}
