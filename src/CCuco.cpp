#include <string>
#include <vector>
#include <memory>

#include "Vec2i.h"
#include "IMapElement.h"
#include "CActor.h"
#include "CDoorway.h"
#include "CMap.h"
#include "CCuco.h"

#include <iostream>

namespace WizardOfGalicia {
  
  CCuco::CCuco() : CActor() {
    view = '@';
    team = VILLAINS;
    hp = 5;
    attack = 4;
    defence = 1;
  }

  bool CCuco::dealWith( std::shared_ptr<CMap> map, int x, int y ) {
    int dx = x - position.x;
    int dy = y - position.y;

    std::shared_ptr<CCuco> sharedThis = shared_from_this();

    if ( abs( dx ) < abs( dy ) ) {
      if ( dx < 0 ) {
	map->move( W, sharedThis );       
	return true;
      } else if ( dx > 0 ) {
	map->move( E, sharedThis ); 
	return true;
      }
    } else {
      if ( dy < 0 ) {
	map->move( N, sharedThis );       
	return true;
      } else if (dy > 0 ){
	map->move( S, sharedThis ); 
	return true;
      }
    }
    return false;
  }

  void CCuco::update( std::shared_ptr<CMap> map ) {

    Vec2i scan;
    int newX;
    int newY;
    
    for (int x = -10; x < 10; ++x) {

      newX =  (x + position.x);
      newY =  (position.y);
      scan.x = newX;
      scan.y = newY;

      if (newX >= 0 && newY >= 0 && newX < 20
                    && newY < 20
	  && map->map[ newY][ newX ] != nullptr
	  && map->map[ newY][ newX ]->team == Team::HEROES ) {

	if (dealWith( map, newX, 0 ) ) {
	  return;
	}
      }
    }

    for (int y = -10; y < 10; ++y) {

      newX = (position.x);
      newY = (y + position.y);
      scan.x = newX;
      scan.y = newY;

      if (newX >= 0 && newY >= 0 && newX < 20
	  && newY < 20
	  && map->map[ newY][ newX ] != nullptr
	  && map->map[ newY][ newX ]->team == Team::HEROES ) {

	if (dealWith( map, 0, newY ) ) {
	  return;
	}
      }
    }
  }
}
