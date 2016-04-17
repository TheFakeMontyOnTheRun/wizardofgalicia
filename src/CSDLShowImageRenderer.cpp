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
#include "CSDLShowImageRenderer.h"

namespace WizardOfGalicia {

  void CSDLShowImageRenderer::init(){
  }
  
  void CSDLShowImageRenderer::drawMap( CMap &map, std::shared_ptr<CActor> current ) {
    SDL_BlitSurface( mImageToShow, nullptr, mTarget, nullptr );
    SDL_Flip( mTarget );
  }
  
  char CSDLShowImageRenderer::update() {
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
	  return 'f';
	}
      }
    }
    
    return '.';
  }
    
  void CSDLShowImageRenderer::shutdown(){
  }

  void CSDLShowImageRenderer::showImage( SDL_Surface *target, SDL_Surface *image ) {
    mTarget = target;
    mImageToShow = image;
  }
}
