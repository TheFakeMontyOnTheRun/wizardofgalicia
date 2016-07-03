//
// Created by monty on 21/02/16.
//

#ifndef LESSON10_TRIG_H
#define LESSON10_TRIG_H


class Trig {
	float *cachedVertexData;
	float *cachedUVData;
public:
	glm::vec3 p0;
	glm::vec2 t0;

	glm::vec3 p1;
	glm::vec2 t1;

	glm::vec3 p2;
	glm::vec2 t2;

	uint textureId;

	Trig();

	const float *getVertexData();

	const float *getUVData();

	virtual ~Trig();

	Trig(glm::vec3 tvec3, glm::vec2 tvec2, glm::vec3 tvec31, glm::vec2 tvec21, glm::vec3 tvec32,
		     glm::vec2 tvec22, uint textureId );
};


#endif //LESSON10_TRIG_H
