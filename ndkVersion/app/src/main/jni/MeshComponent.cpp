//
// Created by monty on 20/04/16.
//
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "IComponent.h"
#include "Trig.h"
#include "MeshComponent.h"

namespace odb {
	MeshComponent::MeshComponent() { }

	void MeshComponent::addTriangle(std::shared_ptr<Trig> triangle) {
		mTriangles.push_back(triangle);
	}

	std::vector<std::shared_ptr<Trig>> MeshComponent::getTriangles() {
		return mTriangles;
	}
}