#include <string>
#include <iostream>
#include <vector>

#include "Vec2i.h"
#include "IMapElement.h"
#include "CActor.h"
#include "CMap.h"
#include "IRenderer.h"
#include "CConsoleRenderer.h"


namespace WizardOfGalicia {
  enum ConsoleColour {
    BLACK = 30,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE
  };

  void setColourFor( std::ostream &stream, ConsoleColour c ) {
    stream << "\033[1;" << c << "m";
  }
  
  void clear( std::ostream &stream ) {
    stream << "\x1B[2J\x1B[H";
  }
  
  void CConsoleRenderer::drawMap( CMap &map, CActor *current ) {
    
    clear( std::cout );
    
    for ( int y = 0; y < 20; ++y ) {
      for ( int x = 0; x < 20; ++x ) {
	
	if ( map.map[ y ][ x ] != nullptr ) {
	  if ( map.map[ y ][ x ] == current && current != nullptr ) {
	    setColourFor( std::cout, YELLOW );
	  } else {
	    setColourFor( std::cout, RED );
	  }
	  std::cout << map.map[ y ][ x ]->view;
	} else {
	  if ( map.block[ y ][ x ] ) {
                    setColourFor( std::cout, CYAN );
                    std::cout << "#";
	  } else {
	    
	    setColourFor( std::cout, GREEN );
	    std::cout << ".";
	  }
	}

      }
      std::cout << std::endl;
    }
    
    setColourFor( std::cout, GREEN );
    
    std::cout << "-=-=-=-=-=-=-" << std::endl;
    
    if ( current != nullptr ) {
      std::cout << "HP: " << current->hp << std::endl;
      std::cout << "AT: " << current->attack << std::endl;
      std::cout << "DF: " << current->defence << std::endl;
    }
    
    std::cout << "-//-" << std::endl;
  }
}
