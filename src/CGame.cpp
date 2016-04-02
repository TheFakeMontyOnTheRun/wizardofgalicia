#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include "Vec2i.h"
#include "IMapElement.h"
#include "CActor.h"
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

  int turn = 1;
  
  
  std::string readMap( const char *mapName ) {
    
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
  
  void endOfTurn( CMap &map ) {
    
    map.endOfTurn();
    ++turn;
  }
  
  void CGame::runGame( IRenderer *renderer ) {
    std::string entry;
    std::string mapData =  readMap( "res/map1.txt" );
    
    auto map = new CMap( mapData );
    
    CActor * avatar = map->mWizard;
    
    while ( true ) {
      
      renderer->drawMap( *map, avatar );
      
      if ( avatar != nullptr &&  avatar->hp <= 0 ) {
	std::cout << "DEAD" << std::endl;
	avatar = nullptr;
      }
      
      std::cin >> entry;
      
      if ( avatar != nullptr ) {
	
	if ( entry == "t" ) {
	  endOfTurn( *map );
	}
	
	if ( entry == "s" ) {
	  map->move( E, *avatar );
	}
	
	if ( entry == "w" ) {
	  map->move( N, *avatar );
	}
	
	if ( entry == "a" ) {
	  map->move( W, *avatar );
	}
	
	if ( entry == "z" ) {
	  map->move( S, *avatar );
	}
	
	if ( entry == "c" ) {
	  
	  int x;
	  int y;
	  
	  std::cout << "target x?" << std::endl;
	  std::cin >> x;
	  std::cout << "target y?" << std::endl;
	  std::cin >> y;
	  
	  map->attack( *avatar, x, y, false );
	}
      } else {
	std::cout << "please, select a new character!" << std::endl;
      }
    }
  }
}
