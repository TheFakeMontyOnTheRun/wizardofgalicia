#include <string>
#include <vector>
#include <iostream>
#include <typeinfo>
#include <memory>

#include "Vec2i.h"
#include "IMapElement.h"
#include "CDoorway.h"
#include "CActor.h"
#include "CMap.h"
#include "CWizard.h"
#include "CCuco.h"


namespace WizardOfGalicia {
  void CMap::endOfTurn() {
    for ( int y = 0; y < 20; ++y ) {
      for ( int x = 0; x < 20; ++x ) {
	if ( map[ y ][ x ] != nullptr ) {
	  map[ y ][ x ]->endOfTurn();
	}
      }
    }
  }
  
  
  CMap::CMap( const std::string &mapData ) {
    
    char element;
    std::shared_ptr<CActor> actor = nullptr;
    
    for ( int y = 0; y < 20; ++y ) {
      for ( int x = 0; x < 20; ++x ) {
	
	element = mapData[ ( y * 20 ) + x ];
	block[ y ][ x ] = false;
	map[ y ][ x ] = nullptr;
	
	switch( element ) {
	case '0':
	case '1':
	  block[ y ][ x ] = ( element == '1' );
	  break;
	  
	case '2':
	  actor = mWizard = std::make_shared<CWizard>();
	  break;
	case '9':
	case '*':
	  mDoorways.push_back(std::make_shared<CDoorway>( Vec2i( x, y ), element == '9' ? DoorwayFunction::EXIT : DoorwayFunction::ENTRY ) );
	  break;
	case '5':
	  actor = std::make_shared<CCuco>();
	  break;
	}
	
	if ( actor != nullptr ) {
	  actors.push_back( actor );
	  map[ y ][ x ] = actor;
	  actor->position.x = x;
	  actor->position.y = y;
	  actor = nullptr;
	}
      }
    }
  }
  
  
  std::shared_ptr<CActor> CMap::attack( std::shared_ptr<CActor> actor, int x, int y, bool mutual ) {
    
    std::shared_ptr<CActor> other = nullptr;
    
    other = map[ y ][ x ];
    
    if ( other != nullptr && actor->team != other->team ) {
      actor->performAttack( other );
      
      if ( mutual ) {
	other->performAttack( actor );
      }
      
      if ( actor->hp <= 0 ) {
	map[ actor->position.y ][ actor->position.x ] = nullptr;
      }
      
      if ( other->hp <= 0 ) {
	map[ other->position.y ][ other->position.x ] = nullptr;
      }
    }
    
    return other;
  }
  
  
  bool CMap::attackIfNotFriendly( Direction d, std::shared_ptr<CActor> actor, bool mutual ) {
    
    std::shared_ptr<CActor> other = nullptr;
    
    switch ( d ) {
      
    case E:
      other = attack( actor, actor->position.x + 1,  actor->position.y, mutual );
      break;
      
    case W:
      other = attack( actor, actor->position.x - 1,  actor->position.y, mutual );
      break;

    case S:
      other = attack( actor, actor->position.x,  actor->position.y + 1, mutual );
      break;
      
    case N:
      other = attack( actor, actor->position.x,  actor->position.y - 1, mutual );
      break;
    }
    
    return ( other != nullptr );
  }
  
  
  void CMap::move( Direction d, std::shared_ptr<CActor> actor ) {
    
    if ( actor->canAttack() && attackIfNotFriendly( d, actor, true ) ) {
      return;
    }
    
    if ( !actor->canMove() ) {
      return;
    }
    
    
    bool moved = false;
    
    switch ( d ) {
      
    case E:
      if ( !block[  actor->position.y ][ actor->position.x + 1 ] ) {
	moved = true;
	map[ actor->position.y ][  actor->position.x ] = nullptr;
	++actor->position.x;
	map[ actor->position.y ][ actor->position.x ] = actor;
      }
      break;
      
    case W:
      if ( !block[  actor->position.y ][ actor->position.x - 1 ] ) {
	moved = true;
	map[ actor->position.y ][  actor->position.x ] = nullptr;
	--actor->position.x;
	map[ actor->position.y ][ actor->position.x ] = actor;
      }
      break;
      
    case S:
      if ( !block[  actor->position.y + 1 ][ actor->position.x ] ) {
	moved = true;
	map[ actor->position.y ][  actor->position.x ] = nullptr;
	++actor->position.y;
	map[ actor->position.y ][ actor->position.x ] = actor;
      }
      break;
      
    case N:
      if ( !block[  actor->position.y -1 ][ actor->position.x ] ) {
	moved = true;
	map[ actor->position.y ][  actor->position.x ] = nullptr;
	--actor->position.y;
	map[ actor->position.y ][ actor->position.x ] = actor;
      }
      break; 
    }
    
    if ( moved ) {
      actor->onMove();
    }
  }
}
