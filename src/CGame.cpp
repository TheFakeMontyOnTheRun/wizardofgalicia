#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>

#include "Vec2i.h"
#include "IMapElement.h"
#include "CActor.h"
#include "CDoorway.h"
#include "CMap.h"
#include "CDoorway.h"
#include "CBaphomet.h"
#include "CCuco.h"
#include "CDemon.h"
#include "CMoura.h"
#include "CWizard.h"
#include "IRenderer.h"
#include "CConsoleRenderer.h"
#include "CGame.h"


namespace WizardOfGalicia {

  std::string CGame::readMap( std::string mapName ) {
    
    std::string entry;
    std::ifstream mapFile( "res/map1.txt" );
    
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

  void CGame::update() {
    for ( auto actor : map->actors ) {
      if ( actor->team == Team::VILLAINS ) {
	actor->update(map);
      }
    }
  }
  
  void CGame::runGame( IRenderer *renderer ) {
    
    turn = 1;
    
    std::string entry;
    std::string mapData =  readMap( "res/map1.txt" );
    
    map = std::make_shared<CMap>( mapData );
    
    std::shared_ptr<CActor> avatar = map->mWizard;
    
    while ( true ) {
      
      renderer->drawMap( *map, avatar );
      
      if ( avatar != nullptr &&  avatar->hp <= 0 ) {
	std::cout << "DEAD" << std::endl;
	avatar = nullptr;
      }
      
      std::cin >> entry;
      
      if ( avatar != nullptr ) {
	
	if ( entry == "s" ) {
	  map->move( E, avatar );
	}
	
	if ( entry == "w" ) {
	  map->move( N, avatar );
	}
	
	if ( entry == "a" ) {
	  map->move( W, avatar );
	}
	
	if ( entry == "z" ) {
	  map->move( S, avatar );
	}
	
	if ( entry == "c" ) {
	  
	  int x;
	  int y;
	  
	  std::cout << "target x?" << std::endl;
	  std::cin >> x;
	  std::cout << "target y?" << std::endl;
	  std::cin >> y;
	  
	  map->attack( avatar, x, y, false );
	}

	update();
	endOfTurn();
      }
    }
  }
}
