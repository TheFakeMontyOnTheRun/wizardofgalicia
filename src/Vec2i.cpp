#include "Vec2i.h"

namespace WizardOfGalicia {
  Vec2i::Vec2i() {
    x = 0;
    y = 0;
  }

  Vec2i::Vec2i( int aX, int aY ) {
    x = aX;
    y = aY;
  }

  bool Vec2i::operator==( const Vec2i &other ) {
    return this->x == other.x && this->y == other.y;
  }
}
