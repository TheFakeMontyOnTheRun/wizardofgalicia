#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <map>
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

    sprites['*'] = SDL_LoadBMP("res/fireball.bmp");
    sprites['#'] = SDL_LoadBMP("res/wall.bmp");

    sprites['^'] = SDL_LoadBMP("res/facingN.bmp");
    sprites['>'] = SDL_LoadBMP("res/facingE.bmp");
    sprites['<'] = SDL_LoadBMP("res/facingW.bmp");
    sprites['V'] = SDL_LoadBMP("res/facingS.bmp");

    sprites['@'] = SDL_LoadBMP("res/monster.bmp");

    sprites['B'] = SDL_LoadBMP("res/door.bmp");
    sprites['&'] = SDL_LoadBMP("res/jewel.bmp");
    sprites['W'] = SDL_LoadBMP("res/boss.bmp");
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
	case SDLK_COMMA:
	  return '<';
	case SDLK_PERIOD:
	  return '>';
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
	
	char toRender = '.';

	for ( auto doorway : map.mDoorways ) {
	  if ( doorway->position.x == x && doorway->position.y == y ) {
	    toRender = doorway->view;
	    color = 0xFFFFFF;
	    toRender = 'B';
	  }
	}

	if ( map.block[ y ][ x ] ) {
	  color = 0x00FF00;
	  toRender = '#';
	}

	if ( map.map[ y ][ x ] != nullptr && map.map[ y ][ x ]->hp > 0 ) {
	  toRender = map.map[ y ][ x ]->view;
	} 

	auto bitmap = sprites[ toRender ];

	if ( bitmap != nullptr ) {
	  SDL_BlitSurface( bitmap, nullptr, video, &rect);
	} else {
	  SDL_FillRect( video, &rect, color );
	}
      }
      ++screenY;
    }   
    if ( current != nullptr ) {
    }

    SDL_Flip(video);
    SDL_Delay( 20 );
  }
}
