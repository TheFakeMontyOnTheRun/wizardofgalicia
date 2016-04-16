#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <map>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include "Vec2i.h"
#include "IMapElement.h"
#include "CActor.h"
#include "CDoorway.h"
#include "CWizard.h"
#include "CMap.h"
#include "IRenderer.h"
#include "CSDLRelativeRenderer.h"


namespace WizardOfGalicia {

  Mix_Chunk *meeleeSound;
  Mix_Chunk *fireballSound;
  Mix_Chunk *powerUpSound;

  void CSDLRelativeRenderer::playFireballSound() {
    Mix_PlayChannel( -1, fireballSound, 0 );
  }

  void CSDLRelativeRenderer::playMeeleeSound() {
    Mix_PlayChannel( -1, meeleeSound, 0 );
  }
  
  void CSDLRelativeRenderer::playPowerUpSound() {
    Mix_PlayChannel( -1, powerUpSound, 0 );
  }

  bool waitForFirePressed() {
    SDL_Event event;
    
    if ( SDL_PollEvent( &event ) ) {

      if( event.type == SDL_QUIT ) {
	exit(0);
      }

      if ( event.type == SDL_KEYUP ) {

	switch ( event.key.keysym.sym ) {
	case SDLK_q:
	  exit(0);
	case SDLK_SPACE:
	  return true;
	}
      }
    }

    return false;
  }
  
  void CSDLRelativeRenderer::init() {
    SDL_Init(  SDL_INIT_EVERYTHING );
    video = SDL_SetVideoMode( 255, 255, 24, 0 );

    if ( Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1 ) {
      std::cout << "coudlnt init mixer" << std::endl;
    }

    if (Mix_Init(MIX_INIT_OGG) != MIX_INIT_OGG ) {
      std::cout << "coudlnt init ogg" << std::endl;
    }
    sprites['*'] = SDL_LoadBMP("res/fireball.bmp");
    sprites['#'] = SDL_LoadBMP("res/wall.bmp");

    sprites['^'] = SDL_LoadBMP("res/facingN.bmp");
    sprites['>'] = SDL_LoadBMP("res/facingE.bmp");
    sprites['<'] = SDL_LoadBMP("res/facingW.bmp");
    sprites['V'] = SDL_LoadBMP("res/facingS.bmp");

    sprites['@'] = SDL_LoadBMP("res/cuco.bmp");

    sprites['B'] = SDL_LoadBMP("res/door.bmp");
    sprites['&'] = SDL_LoadBMP("res/jewel.bmp");
    sprites['W'] = SDL_LoadBMP("res/nimph.bmp");


    sprites['.'] = SDL_LoadBMP("res/floor.bmp");

    titleScreen = SDL_LoadBMP("res/wog.bmp");
    gameOverScreen = SDL_LoadBMP("res/gameover.bmp");
    victoryScreen = SDL_LoadBMP("res/win.bmp");


    if (!(meeleeSound = Mix_LoadWAV( "res/meelee.wav" ) ) ) {
      std::cout << "Loading meelee sound failed" << std::endl;
    }
    
    if  ( !(fireballSound = Mix_LoadWAV( "res/fireball.wav" ) )  ) {
      std::cout << "Loading fireball sound failed" << std::endl;
    }
    
    if ( !(powerUpSound = Mix_LoadWAV( "res/relic.wav" ) )  ) {
      std::cout << "Loading power up sound failed" << std::endl;
    }
    



    waitingForFire = false;
    showing = nullptr;
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

  void castLight( Direction from, int lightMap[][20], int emission, CMap &map, const Vec2i position ) {

    if ( abs( emission ) <= 1 ) {
      return;
    }

    if ( !map.isValid(position.x, position.y) ) {
      return;
    }

    if (map.isBlockAt( position.x, position.y ) ) {
      return;
    }

    lightMap[ position.y ][ position.x ] += emission;

    int power = lightMap[ position.y ][ position.x ];

    if ( abs( power ) > 255 ) {
      lightMap[ position.y ][ position.x ] = 255 * (power/abs(power));    
    }

    Vec2i newPosition;

    newPosition = Vec2i{ position.x, position.y - 1 };
    if ( map.getActorAt( newPosition ) == nullptr ) {
      castLight( Direction::N, lightMap, emission / ( from == Direction::N ? 4 : 8 ), map, newPosition );
    } 
    
    newPosition = Vec2i{ position.x - 1, position.y };
    if ( map.getActorAt( newPosition ) == nullptr ) {
      castLight( Direction::W, lightMap, emission / ( from == Direction::W ? 4 : 8 ), map, newPosition ); 
    }

    newPosition = Vec2i{ position.x, position.y + 1 };
    if ( map.getActorAt( newPosition ) == nullptr ) {
      castLight( Direction::S, lightMap, emission / ( from == Direction::S ? 4 : 8 ), map, newPosition ); 
    }

    newPosition = Vec2i{ position.x + 1, position.y };
    if ( map.getActorAt( newPosition ) == nullptr ) {
      castLight( Direction::E, lightMap, emission / ( from == Direction::E ? 4 : 8 ), map, newPosition ); 
    }
  }


  bool isLitAt( int x, int y, int lightMap[][20], CMap& map ) {
    return 
      ( map.isValid( x - 1, y ) && abs(lightMap[ y ][ x - 1 ]) > 0 ) || 
      ( map.isValid( x + 1, y ) && abs(lightMap[ y ][ x + 1 ]) > 0 ) || 
      ( map.isValid( x, y + 1 ) && abs(lightMap[ y + 1 ][ x ]) > 0 ) || 
      ( map.isValid( x, y - 1 ) && abs(lightMap[ y - 1 ][ x ]) > 0 ) || 

      ( map.isValid( x + 1, y + 1) && abs(lightMap[ y + 1][ x + 1]) > 0 ) || 
      ( map.isValid( x + 1, y - 1) && abs(lightMap[ y - 1][ x + 1]) > 0 ) || 
      ( map.isValid( x - 1, y - 1 ) && abs(lightMap[ y - 1 ][ x - 1]) > 0 ) || 
      ( map.isValid( x - 1, y + 1) && abs(lightMap[ y + 1][ x - 1]) > 0 ) || 
      (false)
      ;
  }

 

  void CSDLRelativeRenderer::showTitleScreen() {
    showing = titleScreen;
    while( !waitForFirePressed() ) {
      SDL_BlitSurface( showing, nullptr, video, nullptr );
      SDL_Flip( video );
    }
  }

  void CSDLRelativeRenderer::showGameOverScreen() {
    showing = gameOverScreen;
    while( !waitForFirePressed() ) {
      SDL_BlitSurface( showing, nullptr, video, nullptr );
      SDL_Flip( video );
    }
  }
  
  void CSDLRelativeRenderer::showVictoryScreen() {
    showing = victoryScreen;
    while( !waitForFirePressed() ) {
      SDL_BlitSurface( showing, nullptr, video, nullptr );
      SDL_Flip( video );
    }
  }

  void CSDLRelativeRenderer::drawMap( CMap &map, std::shared_ptr<CActor> current ) {

    int color;
    SDL_Rect rect;

    rect.x = 0;
    rect.y = 0;
    rect.w = 255;
    rect.h = 255;

    SDL_FillRect( video, &rect, 0 );

    int lightMap[ 20 ][ 20 ];

    for ( int y = 0; y < 20; ++y ) {
      for ( int x = 0; x < 20; ++x ) {
	lightMap[ y ][ x ] = 0;
      }
    }

    for ( auto actor : map.actors ) {
      for ( int c = 0; c < 4; ++c ) {
	castLight( static_cast<Direction>(c), lightMap, actor->magicEnergy, map, actor->position );
      }
    }

    int screenX = 0;
    int screenY = 0;

    for ( int y = -4 + current->position.y; y < 4 + current->position.y; ++y ) {
      screenX = 0;
      for ( int x = -4 + current->position.x; x < 4 + current->position.x; ++x ) {
	++screenX;
	if ( !map.isValid( x, y ) ) {
	  continue;
	}

	rect.x = screenX * 32;
	rect.y = screenY * 32;
	rect.w = 32;
	rect.h = 32;
	
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


	
	int alpha = color = lightMap [ y ][ x ];
	
	if ( alpha < 0 ) {
	  alpha = abs( color );
	  color = abs( color ) << 16;
	}

	SDL_FillRect( video, &rect, color );
	
	if ( bitmap != nullptr ) { // && isLitAt( x, y, lightMap, map ) ) {
	  SDL_SetAlpha( bitmap, SDL_SRCALPHA, 128 + ( 255 - alpha ) / 2 );
	  SDL_BlitSurface( bitmap, nullptr, video, &rect);
	}
	
      }
      ++screenY;
    }   

    if ( current != nullptr ) {

      rect.w = 32;
      rect.h = (255 / 16 );

      rect.x = 0;
      for ( int c = 0; c < 16; ++c ) {
	rect.y =  ( (16 - c) * (255/16) );
	SDL_FillRect( video, &rect, ( c < current->hp ) ? ( c * ( 255 / 16 ) ) << 16 : 0x0 );
      }
    }

    SDL_Flip(video);
    SDL_Delay( 20 );
  }
}
