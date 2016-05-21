#include <memory>
#include <cstdlib>

#include "Vec2i.h"
#include "IMapElement.h"
#include "CActor.h"
#include "CWizard.h"

#include "CRelic.h"

namespace WizardOfGalicia {
	CRelic::CRelic() : CActor() {
		view = '&';
		team = Team::RELICS;
		hp = 1;
		attack = 0;
		defence = 0;
		magicEnergy = 0;
	}

	void CRelic::update(std::shared_ptr<CMap> map) {
	}

	void CRelic::performAttack(std::shared_ptr<CActor> other) {
		other->magicEnergy += (other->magicEnergy / abs(other->magicEnergy)) * 1000;
	}
}
