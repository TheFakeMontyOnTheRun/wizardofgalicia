#include <string>
#include <vector>
#include <memory>

#include "Vec2i.h"
#include "IMapElement.h"
#include "CActor.h"
#include "CDoorway.h"
#include "CWizard.h"
#include "CMap.h"
#include "CBaphomet.h"

namespace WizardOfGalicia {
  
  CBaphomet::CBaphomet() : CActor() {
    view = 'W';
    team = Team::VILLAINS;
    hp = 500;
    attack = 4;
    defence = 1;
    magicEnergy = -50000;
  }

  bool CBaphomet::dealWith( std::shared_ptr<CMap> map, int x, int y ) {
    int dx = x - position.x;
    int dy = y - position.y;

    auto sharedThis = shared_from_this();

    if ( abs( dx ) > abs( dy ) ) {
      
      if ( dx < 0 ) {
	map->move( Direction::W, sharedThis );       
	return true;
      } else if ( dx > 0 ) {
	map->move( Direction::E, sharedThis ); 
	return true;
      }
    } else {

      if ( dy < 0 ) {
	map->move( Direction::N, sharedThis );       
	return true;
      } else if (dy > 0 ){
	map->move( Direction::S, sharedThis ); 
	return true;
      }
    }

    return false;
  }

  bool CBaphomet::actOn( int newX, int newY,  std::shared_ptr<CMap> map ) { 

    if (map->isValid( newX, newY ) ) {
      
      if (map->map[ newY][ newX ] != nullptr
	  && map->map[ newY][ newX ]->team == Team::HEROES) {
	
	if (dealWith( map, newX, newY ) ) {

	  return true;
	}
      }
      
      if ( map->isBlockAt( newX, newY ) ) {
	return true;
      }
      
    } 
    return false;
  }

  void CBaphomet::update( std::shared_ptr<CMap> map ) {

    Vec2i scan;
    int newX;
    int newY;
    
    for (int x = 0; x < 10; ++x) {

      newX =  (x + position.x);
      newY =  (position.y);
      scan.x = newX;
      scan.y = newY;
      if ( actOn( newX, newY, map ) ) {
	break;
      }
    }

    for (int x = 0; x > -10; --x) {

      newX =  (x + position.x);
      newY =  (position.y);
      scan.x = newX;
      scan.y = newY;
      
      if ( actOn( newX, newY, map ) ) {
	break;
      }
    }

    for (int y = 0; y < 10; ++y) {

      newX = (position.x);
      newY = (y + position.y);
      scan.x = newX;
      scan.y = newY;

      if ( actOn( newX, newY, map ) ) {
	break;
      }
    }

    for (int y = 0; y > -10; --y) {

      newX = (position.x);
      newY = (y + position.y);
      scan.x = newX;
      scan.y = newY;

      if ( actOn( newX, newY, map ) ) {
	break;
      }
    }
  }
}
