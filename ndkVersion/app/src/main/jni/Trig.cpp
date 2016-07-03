//
// Created by monty on 21/02/16.
//
#include <glm/glm.hpp>
#include "Trig.h"

Trig::Trig() {
	cachedVertexData = nullptr;
	cachedUVData = nullptr;
}

const float *Trig::getUVData() {
	if (cachedUVData == nullptr) {
		cachedUVData = new float[3 * 2];

		cachedUVData[0] = t0.x;
		cachedUVData[1] = t0.y;

		cachedUVData[2] = t1.x;
		cachedUVData[3] = t1.y;

		cachedUVData[4] = t2.x;
		cachedUVData[5] = t2.y;
	}

	return cachedUVData;
}


const float *Trig::getVertexData() {
	if (cachedVertexData == nullptr) {
		cachedVertexData = new float[3 * 3];

		cachedVertexData[0] = p0.x;
		cachedVertexData[1] = p0.y;
		cachedVertexData[2] = p0.z;

		cachedVertexData[3] = p1.x;
		cachedVertexData[4] = p1.y;
		cachedVertexData[5] = p1.z;

		cachedVertexData[6] = p2.x;
		cachedVertexData[7] = p2.y;
		cachedVertexData[8] = p2.z;
	}

	return cachedVertexData;
}

Trig::~Trig() {
	delete cachedUVData;
	delete cachedVertexData;
}

Trig::Trig(glm::vec3 aP0, glm::vec2 aT0, glm::vec3 aP1, glm::vec2 aT1, glm::vec3 aP2,
           glm::vec2 aT2, uint aTextureId) : p0( aP0 ), t0( aT0 ), p1( aP1 ), t1( aT1 ), p2( aP2 ), t2( aT2 ), textureId( aTextureId)  {
	cachedVertexData = nullptr;
	cachedUVData = nullptr;

}
