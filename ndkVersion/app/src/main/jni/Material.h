//
// Created by monty on 20/04/16.
//

#ifndef CARDBOARDVERSION_MATERIAL_H
#define CARDBOARDVERSION_MATERIAL_H

namespace odb {
    class Material {
    public:
	    Material();
	    std::string mGlossinessMap;
	    std::string mNormalMap;
	    std::string mSpecularMap;
	    std::string mColorMap;
    };
}

#endif //CARDBOARDVERSION_MATERIAL_H
