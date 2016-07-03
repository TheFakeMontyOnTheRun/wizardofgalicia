//
// Created by monty on 02/07/16.
//

#ifndef NDKVERSION_SCENECUBE_H
#define NDKVERSION_SCENECUBE_H

namespace odb {
	class SceneCube : public SceneElement {
	public:
		SceneCube(uint textureId,float from, float to, bool renderWalls);

		void makeWall(glm::vec3 tvec3, glm::vec2 tvec2, glm::vec3 p1, glm::vec2 t1, glm::vec3 p2,
	              glm::vec2 t2, glm::vec3 p3, glm::vec2 t3, uint id,
	              std::shared_ptr<MeshComponent> shared_ptr);
	};
}

#endif //NDKVERSION_SCENECUBE_H
