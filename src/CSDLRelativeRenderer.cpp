#include <string>
#include <iostream>
#include <vector>
#include <memory>

#include <SDL/SDL.h>

#include "Vec2i.h"
#include "IMapElement.h"
#include "CActor.h"
#include "CDoorway.h"
#include "CMap.h"
#include "IRenderer.h"
#include "CSDLRelativeRenderer.h"


namespace WizardOfGalicia {
  
  void CSDLRelativeRenderer::init() {
    SDL_Init(  SDL_INIT_EVERYTHING );
    video = SDL_SetVideoMode( 64, 64, 24, 0 );
  }

  void CSDLRelativeRenderer::shutdown() {
    //    SDL_Free(video);
    SDL_Quit();
  }

  char CSDLRelativeRenderer::update() {
    SDL_Event event;


    if ( SDL_PollEvent( &event ) ) {

      if( event.type == SDL_QUIT ) {
	exit(0);
      }

      if ( event.type == SDL_KEYUP ) {

	switch ( event.key.keysym.sym ) {
	case SDLK_q:
	  exit(0);
	case SDLK_LEFT:
	  return 'i';
	case SDLK_RIGHT:
	  return 'p';
	case SDLK_UP:
	  return 'o';
	case SDLK_SPACE:
	  return 'f';
	}
      }
    }
    
    return '.';
  }

  
  void CSDLRelativeRenderer::drawMap( CMap &map, std::shared_ptr<CActor> current ) {
    
    int color;
    SDL_Rect rect;

    rect.x = 0;
    rect.y = 0;
    rect.w = 64;
    rect.y = 64;

    SDL_FillRect( video, &rect, 0 );

    int screenX = 0;
    int screenY = 0;

    for ( int y = -4 + current->position.y; y < 4 + current->position.y; ++y ) {
      screenX = 0;
      for ( int x = -4 + current->position.x; x < 4 + current->position.x; ++x ) {
	++screenX;
	if ( !map.isValid( x, y ) ) {
	  continue;
	}

	

	rect.x = screenX * 8;
	rect.y = screenY * 8;
	rect.w = 8;
	rect.h = 8;
	
	if ( map.map[ y ][ x ] != nullptr && map.map[ y ][ x ]->hp > 0 ) {
	  if ( map.map[ y ][ x ] == current && current != nullptr ) {
	    color = 0xFFFF00;
	  } else {
	    color = 0xFF0000;
	  }
	} else {
	  if ( map.block[ y ][ x ] ) {
	    color = 0x00FF00;
	  } else {
	    color = 0x0;
	    char toRender = '.';
	    for ( auto doorway : map.mDoorways ) {
	      if ( doorway->position.x == x && doorway->position.y == y ) {
		toRender = doorway->view;
		color = 0xFFFFFF;
	      }
	    }
	  }
	} 

	SDL_FillRect( video, &rect, color );
      }
      ++screenY;
    }   
    if ( current != nullptr ) {
    }

    SDL_Flip(video);
  }
}
