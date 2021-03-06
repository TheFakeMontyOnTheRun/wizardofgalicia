#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <algorithm>
#include <sstream>

#include "Vec2i.h"
#include "IMapElement.h"
#include "CActor.h"
#include "CDoorway.h"



#include "CDoorway.h"
#include "CBaphomet.h"
#include "CCuco.h"
#include "CDemon.h"
#include "CMoura.h"
#include "CWizard.h"
#include "CMap.h"
#include "IRenderer.h"
#include "CConsoleRenderer.h"
#include "CGame.h"
#include "CFireball.h"

namespace WizardOfGalicia {

  CGame::CGame() : mPlayer( std::make_shared<CWizard>() ) {
  }

  std::string CGame::readMap( int level ) {

    std::cout << "loading level " << level << std::endl;
    
    std::string entry;

    std::stringstream levelNameBuilder;

    levelNameBuilder << "res/map";
    levelNameBuilder << level;
    levelNameBuilder << ".txt";

    std::ifstream mapFile( levelNameBuilder.str() );
    
    char line[ 80 ];
    
    while ( !mapFile.eof() ) {
      mapFile >> line;
      entry += line;
    }
    
    auto position = entry.find('\n' );
    
    while ( position != std::string::npos ) {
      entry.replace( position, 1, "" );
      position = entry.find( '\n', position + 1);
    }
    
    return entry;
  }
  
  void CGame::endOfTurn() {    
    map->endOfTurn();
    ++turn;
  }

  void CGame::putAt( const Vec2i& position, std::shared_ptr<CActor> actor ) {
    map->map[ position.y ][ position.x ] = actor;
  }

  void CGame::update() {

    for ( auto actor : map->actors ) {
      if ( actor->hp > 0 ) {
	actor->update(map);
      } else {
	map->map[ actor->position.y][actor->position.x] = nullptr;
      }
    }
    
    map->actors.erase( std::remove_if( map->actors.begin(), map->actors.end(), 
				       [](std::shared_ptr<CActor> actor){ return actor->hp <= 0;}
				       ), map->actors.end() );
  
    for ( auto actor : map->actors ) {
      if (map-> getActorAt( actor->position ) == nullptr ) {
	putAt( actor->position, actor );
      }    
    }
  }


  bool CGame::updatePendingProjectiles() {
    bool needAnotherPass = true;


    needAnotherPass = false;
    for( auto actor : map->actors ) {
      if ( actor->view == '*' ) {
	putAt( actor->position, nullptr );
	actor->update( map );
	putAt( actor->position, actor );
	needAnotherPass = needAnotherPass || ( actor->hp > 0 );
      }
    }
    
    return needAnotherPass;
  }
  
  GameResult CGame::tick() {
    bool shouldEndTurn = false;
    bool attackHasHappened = false;
    std::string entry;    
    std::shared_ptr<CActor> avatar = map->mWizard;
    
    int sumOfHps = 0;

    for ( auto actor : map->actors ) {
      sumOfHps += actor->hp;
    }

    renderer->drawMap( *map, avatar );
    entry = renderer->update();
    
    if ( avatar != nullptr &&  avatar->hp <= 0 ) {
      std::cout << "DEAD" << std::endl;
      avatar = nullptr;
    }
    
    shouldEndTurn = false;
    
    if ( !updatePendingProjectiles() ) {
      
      if ( avatar != nullptr ) {
	
	if ( entry == "s" ) {
	  map->move( Direction::E, avatar );
	  shouldEndTurn = true;
	}
	
	if ( entry == "w" ) {
	  map->move( Direction::N, avatar );
	  shouldEndTurn = true;
	}
	
	if ( entry == "a" ) {
	  map->move( Direction::W, avatar );
	  shouldEndTurn = true;
	}
	
	if ( entry == "i" ) {
	  avatar->turnLeft();
	}
	
	if ( entry == "o" ) {
	  shouldEndTurn = true;
	  map->move( avatar->direction, avatar );
	}
	
	
	if ( entry == "<" ) {
	  shouldEndTurn = true;
	  avatar->turnLeft();
	  map->move( avatar->direction, avatar );
	  avatar->turnRight();
	}
	
	if ( entry == ">" ) {
	  shouldEndTurn = true;
	  avatar->turnRight();
	  map->move( avatar->direction, avatar );
	  avatar->turnLeft();
	}
	
	if ( entry == "p" ) {
	  avatar->turnRight();
	  }
	
	if ( entry == "k" ) {
	  
	}
	
	if ( entry == "l" ) {
	}
	  
	if ( entry == "end" ) {
	  return GameResult::PlayerHasDied;
	}
	
	if ( entry == "win" ) {
	  return GameResult::PlayerHasFinishedLevel;
	}
	
	if ( entry == "f" ) {
	  map->cast( avatar );
	  renderer->playFireballSound();
	  shouldEndTurn = true;
	}
	
	if ( entry == "z" ) {
	  map->move( Direction::S, avatar );
	}
	
	if ( entry == "t" ) {
	  shouldEndTurn = true;
	}
      }
      
      if ( shouldEndTurn ) {
	update();
	endOfTurn();
	shouldEndTurn = false;
      }

      int otherSumOfHps = 0;

      for ( auto actor : map->actors ) {
	otherSumOfHps += actor->hp;
      }
      
      if ( otherSumOfHps < sumOfHps ) {
	renderer->playMeeleeSound();
      }
      
      if ( playerIsDead( avatar ) ) {
	return GameResult::PlayerHasDied;
      }
      
      if ( playerHasFinishedLevel( avatar, map ) ) {
	return GameResult::PlayerHasFinishedLevel;
      }
      
      if ( hasPlayerReturnedToPreviousLevel( avatar, map ) ) {
	return GameResult::PlayerHasReturnedALevel;
      }
    }
    
    return GameResult::UndefinedBehaviour;
  }
  




  GameResult CGame::runGame( IRenderer *aRenderer, int level ) {

    renderer = aRenderer;
    
    turn = 1;
    std::string mapData =  readMap( level );
    map = std::make_shared<CMap>( mapData, mPlayer );


    while ( true ) {
      auto result = tick();
      if ( result != GameResult::UndefinedBehaviour ) {
	return result;
      }
    }
    return GameResult::UndefinedBehaviour;
  }


  bool CGame::playerIsDead( std::shared_ptr<CActor> avatar ) {
    return avatar->hp <= 0;
  }
  
  bool CGame::playerHasFinishedLevel( std::shared_ptr<CActor> avatar, std::shared_ptr<CMap> map ) {
    return map->isAtExit( avatar ) && map->hasClearedLevel();
  }

  bool CGame::hasPlayerReturnedToPreviousLevel( std::shared_ptr<CActor> avatar, std::shared_ptr<CMap> map ) {
    return map->isAtEntrance( avatar )&& map->hasClearedLevel();
  }
}
