//
// Created by monty on 20/04/16.
//

#ifndef CARDBOARDVERSION_MESHCOMPONENT_H
#define CARDBOARDVERSION_MESHCOMPONENT_H

namespace odb {
	class MeshComponent : public IComponent {

		std::vector<std::shared_ptr<Trig>> mTriangles;
	public:

		MeshComponent();

		std::vector<std::shared_ptr<Trig>> getTriangles();

		void addTriangle(std::shared_ptr<Trig> triangle);
	};
}

#endif //CARDBOARDVERSION_MESHCOMPONENT_H
