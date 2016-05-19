//
// Created by monty on 20/04/16.
//

#ifndef CARDBOARDVERSION_SCENEELEMENT_H
#define CARDBOARDVERSION_SCENEELEMENT_H

namespace odb {
    class SceneElement {
	    std::map<std::string, std::shared_ptr<IComponent>> mComponents;
    public:

	    bool mVisible;

	    SceneElement();

	    glm::mat4 transform;

	    void addComponent(std::string name, std::shared_ptr<IComponent> component);

	    void removeComponent(std::string name);

	    std::shared_ptr<IComponent> getComponent(std::string name );
    };
}

#endif //CARDBOARDVERSION_SCENEELEMENT_H
