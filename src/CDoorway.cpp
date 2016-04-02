#include "Vec2i.h"
#include "IMapElement.h"
#include "CDoorway.h"

namespace WizardOfGalicia {
  CDoorway::CDoorway( DoorwayFunction f ): doorFunction( f ) {
    if ( f == DoorwayFunction::EXIT ) {
      view = 'E';
    } else {
      view = 'B';
    }
  }
}
