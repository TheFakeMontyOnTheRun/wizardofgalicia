#include "Vec2i.h"
#include "IMapElement.h"
#include "CDoorway.h"

namespace WizardOfGalicia {
	CDoorway::CDoorway(const Vec2i &pos, DoorwayFunction f) : doorFunction(f) {

		position = pos;

		if (f == DoorwayFunction::Exit) {
			view = 'E';
		} else {
			view = 'B';
		}
	}
}
