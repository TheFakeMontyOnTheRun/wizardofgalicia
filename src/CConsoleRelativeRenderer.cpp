#include <string>
#include <iostream>
#include <vector>
#include <memory>

#include "Vec2i.h"
#include "IMapElement.h"
#include "CActor.h"
#include "CDoorway.h"
#include "CWizard.h"
#include "CMap.h"
#include "IRenderer.h"
#include "CConsoleRenderer.h"


namespace WizardOfGalicia {
	enum ConsoleColour {
		BLACK = 30,
		RED,
		GREEN,
		YELLOW,
		BLUE,
		MAGENTA,
		CYAN,
		WHITE
	};

	void setColourFor(std::ostream &stream, ConsoleColour c) {
		stream << "\033[1;" << c << "m";
	}

	void clear(std::ostream &stream) {
		stream << "\x1B[2J\x1B[H";
	}


	char CConsoleRenderer::update() {
		char toReturn;

		std::cin >> toReturn;

		return toReturn;
	}

	void CConsoleRenderer::drawMap(CMap &map, std::shared_ptr<CActor> current) {

		clear(std::cout);

		for (int y = -4 + current->position.y; y < 4 + current->position.y; ++y) {
			for (int x = -4 + current->position.x; x < 4 + current->position.x; ++x) {

				if (!map.isValid(x, y)) {
					continue;
				}

				if (map.map[y][x] != nullptr && map.map[y][x]->hp > 0) {
					if (map.map[y][x] == current && current != nullptr) {
						setColourFor(std::cout, YELLOW);
					} else {
						setColourFor(std::cout, RED);
					}
					std::cout << map.map[y][x]->view;
				} else {
					if (map.block[y][x]) {
						setColourFor(std::cout, CYAN);
						std::cout << "#";
					} else {

						setColourFor(std::cout, GREEN);

						char toRender = '.';
						for (auto doorway : map.mDoorways) {
							if (doorway->position.x == x && doorway->position.y == y) {
								toRender = doorway->view;
							}
						}
						std::cout << toRender;
					}
				}
			}
			std::cout << std::endl;
		}
		setColourFor(std::cout, GREEN);

		std::cout << "-=-=-=-=-=-=-" << std::endl;

		if (current != nullptr) {
			std::cout << "HP: " << current->hp << std::endl;
			std::cout << "EN: " << current->magicEnergy << std::endl;
		}

		std::cout << "-//-" << std::endl;
	}
}
