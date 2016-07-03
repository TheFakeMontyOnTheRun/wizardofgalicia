//
// Created by monty on 02/07/16.
//
#include <memory>
#include <map>
#include <vector>
#include <glm/gtc/type_ptr.hpp>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "IComponent.h"
#include "Trig.h"
#include "MeshComponent.h"
#include "SceneElement.h"
#include "SceneCube.h"


namespace odb {

	SceneCube::SceneCube(uint textureId, float from, float to, bool renderWalls) : SceneElement() {

		glm::vec2 t0;
		glm::vec2 t1;
		glm::vec2 t2;
		glm::vec2 t3;

		glm::vec3 p0;
		glm::vec3 p1;
		glm::vec3 p2;
		glm::vec3 p3;

		glm::vec3 p4;
		glm::vec3 p5;
		glm::vec3 p6;
		glm::vec3 p7;

		auto mesh = std::make_shared<MeshComponent>();

		t0 = glm::vec2{0.0f, 1.0f};
		t1 = glm::vec2{1.0f, 1.0f};
		t2 = glm::vec2{1.0f, 0.0f};
		t3 = glm::vec2{0.0f, 0.0f};


		p0 = glm::vec3{-1.0f, from, -1.0f};
		p1 = glm::vec3{1.0f, from, -1.0f};
		p2 = glm::vec3{1.0f, from, 1.0f};
		p3 = glm::vec3{-1.0f, from, 1.0f};

		p4 = glm::vec3{-1.0f, to, -1.0f};
		p5 = glm::vec3{1.0f, to, -1.0f};
		p6 = glm::vec3{1.0f, to, 1.0f};
		p7 = glm::vec3{-1.0f, to, 1.0f};

		//floor
		makeWall( p0, t0, p1, t1, p2, t2, p3, t3, textureId, mesh );

		if ( renderWalls ) {
			//north
			makeWall(p0, t0, p1, t1, p5, t2, p4, t3, textureId, mesh);

			//south
			makeWall(p2, t0, p3, t1, p7, t2, p6, t3, textureId, mesh);


			//west
			makeWall(p7, t0, p4, t1, p0, t2, p3, t3, textureId, mesh);

			//east
			makeWall(p6, t0, p5, t1, p1, t2, p2, t3, textureId, mesh);
		}

		//ceiling
		makeWall( p4, t0, p5, t1, p6, t2, p7, t3, textureId, mesh );




		addComponent("mesh", mesh);

	}

	void SceneCube::makeWall(glm::vec3 p0, glm::vec2 t0, glm::vec3 p1, glm::vec2 t1,
	                         glm::vec3 p2, glm::vec2 t2, glm::vec3 p3, glm::vec2 t3, uint textureId,
	                         std::shared_ptr<MeshComponent> mesh) {

		mesh->addTriangle(std::make_shared<Trig>(p0, t0, p1, t1, p2, t2, textureId));
		mesh->addTriangle(std::make_shared<Trig>(p2, t2, p3, t3, p0, t0, textureId));
	}
}
