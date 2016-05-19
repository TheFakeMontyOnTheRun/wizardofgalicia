//
// Created by monty on 20/04/16.
//
#include <memory>
#include <map>
#include <glm/gtc/type_ptr.hpp>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "IComponent.h"
#include "SceneElement.h"

namespace odb {

	SceneElement::SceneElement() : mVisible{true} {
	}

    void SceneElement::addComponent(std::string name, std::shared_ptr<IComponent> component) {
	    mComponents[name] = component;
    }

	std::shared_ptr<IComponent> SceneElement::getComponent(std::string name) {
		return mComponents[ name ];
	}

    void SceneElement::removeComponent(std::string name) {

    }
}
