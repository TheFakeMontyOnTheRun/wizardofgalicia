//
// Created by monty on 2015/02/06
//
#include <GLES2/gl2.h>
#include <glm/gtc/type_ptr.hpp>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <memory>
#include <vector>
#include <thread>
#include <mutex>
#include <random>
#include <map>
#include <android/log.h>


#include "Vec2i.h"
#include "IMapElement.h"
#include "CActor.h"
#include "CDoorway.h"
#include "CWizard.h"
#include "CMap.h"

#include "IRenderer.h"
#include <CGame.h>

#include "WalkBouncer.h"
#include "NativeBitmap.h"
#include "Trig.h"
#include "TrigBatch.h"

#include "IComponent.h"
#include "SceneElement.h"
#include "Material.h"
#include "MeshComponent.h"

#include "GLES2Renderer.h"
#include "NdkGlue.h"

namespace odb {

	void buildMipMap(const std::shared_ptr<NativeBitmap> &texture) {
		auto bitmap = texture;
		std::shared_ptr<NativeBitmap> old;
		int level = 1;
		while (bitmap->getWidth() > 1) {
			old = bitmap;
			bitmap = old->makeBitmapWithHalfDimensions();
			glTexImage2D(GL_TEXTURE_2D, level, GL_RGBA, bitmap->getWidth(), bitmap->getHeight(), 0,
			             GL_RGBA, GL_UNSIGNED_BYTE,
			             bitmap->getPixelData());
			++level;

		}
	}

	GLuint uploadTextureData(const std::shared_ptr<NativeBitmap> &texture) {
		// Texture object handle
		GLuint textureId = 0;

		//Generate texture storage
		glGenTextures(1, &textureId);

		//specify what we want for that texture
		glBindTexture(GL_TEXTURE_2D, textureId);

		//upload the data
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);


		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->getWidth(), texture->getHeight(), 0,
		             GL_RGBA, GL_UNSIGNED_BYTE,
		             texture->getPixelData());

		buildMipMap(texture);

		return textureId;
	}


	extern void printGLString(const char *name, GLenum s) {
		const char *v = (const char *) glGetString(s);
		LOGI("GL %s = %s\n", name, v);
	}

	extern void checkGlError(const char *op) {
		for (GLint error = glGetError(); error; error = glGetError()) {
			LOGI("after %s() glError (0x%x)\n", op, error);
		}
	}

	GLint GLES2Renderer::loadShader(GLenum shaderType, const char *pSource) {
		GLint shader = glCreateShader(shaderType);
		if (shader) {
			glShaderSource(shader, 1, &pSource, NULL);
			glCompileShader(shader);
			GLint compiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
			if (!compiled) {
				GLint infoLen = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
				if (infoLen) {
					char *buf = (char *) malloc(infoLen);
					if (buf) {
						glGetShaderInfoLog(shader, infoLen, NULL, buf);
						LOGE("Could not compile shader %d:\n%s\n", shaderType, buf);
						free(buf);
					}
					glDeleteShader(shader);
					shader = 0;
				}
			}
		}
		return shader;
	}

	GLint GLES2Renderer::createProgram(const char *pVertexSource, const char *pFragmentSource) {
		GLint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
		if (!vertexShader) {
			return 0;
		}

		GLint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
		if (!pixelShader) {
			return 0;
		}

		GLint program = glCreateProgram();
		if (program) {
			glAttachShader(program, vertexShader);
			checkGlError("glAttachShader");
			glAttachShader(program, pixelShader);
			checkGlError("glAttachShader");
			glLinkProgram(program);
			GLint linkStatus = GL_FALSE;
			glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
			if (linkStatus != GL_TRUE) {
				GLint bufLength = 0;
				glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
				if (bufLength) {
					char *buf = (char *) malloc(bufLength);
					if (buf) {
						glGetProgramInfoLog(program, bufLength, NULL, buf);
						LOGE("Could not link program:\n%s\n", buf);
						free(buf);
					}
				}
				glDeleteProgram(program);
				program = 0;
			}
		}
		return program;
	}

	void GLES2Renderer::printVerboseDriverInformation() {
		printGLString("Version", GL_VERSION);
		printGLString("Vendor", GL_VENDOR);
		printGLString("Renderer", GL_RENDERER);
		printGLString("Extensions", GL_EXTENSIONS);
	}

	GLES2Renderer::GLES2Renderer() {
//start off as identity - later we will init it with proper values.
		projectionMatrix = glm::mat4(1.0f);
		vertexAttributePosition = 0;
		modelMatrixAttributePosition = 0;
		projectionMatrixAttributePosition = 0;
		gProgram = 0;
		nextAction = '.';
		reset();
	}

	GLES2Renderer::~GLES2Renderer() {
		glDeleteTextures(1, &textureId);
	}

	bool GLES2Renderer::init(float w, float h, const std::string &vertexShader,
	                         const std::string &fragmentShader) {

		printVerboseDriverInformation();

		gProgram = createProgram(vertexShader.c_str(), fragmentShader.c_str());

		if (!gProgram) {
			LOGE("Could not create program.");
			return false;
		}

		fetchShaderLocations();

		glViewport(0, 0, w, h);
		checkGlError("glViewport");

		projectionMatrix = glm::perspective(45.0f, w / h, 0.1f, 100.0f);

		camera = glm::vec3(0.0f, 0.5f, 0.0f);
		cameraDirection = glm::vec3(0.0f, 0.0f, -1.0f);

		glFrontFace(GL_CW);
		glDepthMask(true);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		return true;
	}

	void GLES2Renderer::resetTransformMatrices() {
		viewMatrix = glm::lookAt(camera, camera + cameraDirection,
		                         glm::vec3(0.0, 1.0f, 0.0f));
		glUniformMatrix4fv(viewMatrixAttributePosition, 1, false, &viewMatrix[0][0]);
	}

	void GLES2Renderer::fetchShaderLocations() {

		vertexAttributePosition = glGetAttribLocation(gProgram, "aPosition");
		modelMatrixAttributePosition = glGetUniformLocation(gProgram, "uModel");
		viewMatrixAttributePosition = glGetUniformLocation(gProgram, "uView");
		projectionMatrixAttributePosition = glGetUniformLocation(gProgram, "uProjection");
		samplerUniformPosition = glGetUniformLocation(gProgram, "sTexture");
		textureCoordinatesAttributePosition = glGetAttribLocation(gProgram, "aTexCoord");
	}

	void GLES2Renderer::clearBuffers() {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClearDepthf(1.0f);
		checkGlError("glClearColor");
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		checkGlError("glClear");
	}

	void GLES2Renderer::setPerspective() {
		glUniformMatrix4fv(projectionMatrixAttributePosition, 1, false, &projectionMatrix[0][0]);
	}

	void GLES2Renderer::prepareShaderProgram() {
		glUseProgram(gProgram);
		checkGlError("glUseProgram");

		glUniform1i(samplerUniformPosition, 0);

		glActiveTexture(GL_TEXTURE0);
	}

	void GLES2Renderer::render() {

		clearBuffers();
		prepareShaderProgram();
		setPerspective();
		resetTransformMatrices();

		glEnable(GL_DEPTH_TEST);

		checkGlError("before drawing");


		glEnableVertexAttribArray(vertexAttributePosition);
		glEnableVertexAttribArray(textureCoordinatesAttributePosition);

		glBindTexture(GL_TEXTURE_2D, 1);

		for (auto &element : mSceneElements) {

			if (!element->mVisible) {
				continue;
			}

			auto mesh = (MeshComponent *) &(*element->getComponent("mesh"));

			for (auto triangle : mesh->getTriangles()) {
				drawTriangle(triangle, element->transform);
			}
		}

		for (auto &element : mCharacters) {

			if (!element->mVisible) {
				continue;
			}

			auto mesh = (MeshComponent *) &(*element->getComponent("mesh"));

			for (auto triangle : mesh->getTriangles()) {
				drawTriangle(triangle, element->transform);
			}
		}

		glDisableVertexAttribArray(vertexAttributePosition);
		glDisableVertexAttribArray(textureCoordinatesAttributePosition);


		checkGlError("after drawing");
	}

	void GLES2Renderer::tick(WizardOfGalicia::CGame &game) {

		if (game.map == nullptr) {
			return;
		}

		game.tick();

		auto camera = game.mPlayer;

		if ( camera == nullptr ) {
			return;
		}

		mCharacters.clear();

		for (auto &character : game.map->actors) {
			if ( character->hp > 0 ) {
				auto pos = character->position;
				std::shared_ptr<SceneElement> element = makeQuadElementAt(pos.x - camera->position.x,
				                                                          10.0f + pos.y - camera->position.y,
				                                                          character->view);
				mQuadsForCharacters[character] = element;
				mCharacters.push_back(element);
			}

		}

		for (int y = 0; y < 20; ++y) {
			for (int x = 0; x < 20; ++x) {
				tileMap[y][x]->transform = glm::translate(glm::mat4(1), getVec3For( x - camera->position.x, 10 + y - camera->position.y));
			}
		}
	}

	void GLES2Renderer::shutdown() {
		LOGI("Shutdown!\n");
	}

	void GLES2Renderer::reset() {
	}

	void GLES2Renderer::addTrigsForTexture(GLint textureId, const std::vector<Trig> &newTrigs) {
	}

	void GLES2Renderer::setTextures(std::vector<std::shared_ptr<NativeBitmap>> vector,
	                                std::map<std::shared_ptr<NativeBitmap>, char> representation) {
		mTextures.insert(mTextures.end(), vector.begin(), vector.end());

		for (auto &texture : mTextures) {
			unsigned int id = uploadTextureData(texture);

			LOGI("pushing id %d", id);
			mMaterials.push_back(id);
			mRepresentations[representation[texture]] = id;
		}
	}

	void GLES2Renderer::playFireballSound() {
		LOGI( "WOOOOOOSH!" );
	};

	void GLES2Renderer::playMeeleeSound() { };

	void GLES2Renderer::playPowerUpSound() { };

	bool GLES2Renderer::waitingForKey() {
		return false;
	};

	void GLES2Renderer::init(std::shared_ptr<WizardOfGalicia::CMap> map) {

		mSceneElements.clear();

		for (int y = 0; y < 20; ++y) {
			for (int x = 0; x < 20; ++x) {
				bool block = map->isBlockAt(x, y);
				std::shared_ptr<SceneElement> element = makeQuadElementAt(x, y, block ? '#' : '.');
				mSceneElements.push_back(element);
				tileMap[y][x] = element;

			}
		}

		mCharacters.clear();

		for (auto &character : map->actors) {
			auto pos = character->position;
			std::shared_ptr<SceneElement> element = makeQuadElementAt(pos.x, pos.y,
			                                                          character->view);
			mQuadsForCharacters[character] = element;
			mCharacters.push_back(element);
		}
	}

	std::shared_ptr<SceneElement> GLES2Renderer::makeQuadElementAt(int x, int y,
	                                                               char representation) const {
		glm::vec3 p0{-1.0f, -1.0f, -1.0f};
		glm::vec2 t0{0.0f, 1.0f};

		glm::vec3 p1{1.0f, -1.0f, -1.0f};
		glm::vec2 t1{1.0f, 1.0f};

		glm::vec3 p2{1.0f, 1.0f, -1.0f};
		glm::vec2 t2{1.0f, 0.0f};

		glm::vec3 p3{-1.0f, 1.0f, -1.0f};
		glm::vec2 t3{0.0f, 0.0f};

		auto trig1 = std::make_shared<Trig>();
		auto trig2 = std::make_shared<Trig>();

		trig1->p0 = p0;
		trig1->p1 = p1;
		trig1->p2 = p2;

		trig1->t0 = t0;
		trig1->t1 = t1;
		trig1->t2 = t2;

		trig2->p0 = p2;
		trig2->p1 = p3;
		trig2->p2 = p0;

		trig2->t0 = t2;
		trig2->t1 = t3;
		trig2->t2 = t0;

		trig1->textureId = getMaterialForRepresentation(representation);
		trig2->textureId = trig1->textureId;

		auto element = std::make_shared<SceneElement>();
		element->transform = glm::translate(glm::mat4(1),
		                                    getVec3For(x, y));
		auto mesh = std::make_shared<MeshComponent>();
		mesh->addTriangle(trig1);
		mesh->addTriangle(trig2);
		element->addComponent("mesh", mesh);
		return element;
	}

	glm::mediump_vec3 GLES2Renderer::getVec3For(int x, int y) const {
		return glm::vec3(-1.0f + (2.0f * x),
		                                              1.0f + (20.0f - (2.0f * y)), -40.0f);
	};

	void GLES2Renderer::drawMap(WizardOfGalicia::CMap &map,
	                            std::shared_ptr<WizardOfGalicia::CActor> current) { };

	char GLES2Renderer::update() {

		char toReturn = nextAction;

		nextAction = '.';

		return toReturn;
	};

	void GLES2Renderer::showTitleScreen() { };

	void GLES2Renderer::showGameOverScreen() { };

	void GLES2Renderer::showVictoryScreen() { };

	void GLES2Renderer::showInstructionsScreen() { };

	void GLES2Renderer::drawTriangle(std::shared_ptr<Trig> triangle, glm::mat4 transform) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, triangle->textureId);
		glUniform1i(samplerUniformPosition, 0);
		glUniformMatrix4fv(modelMatrixAttributePosition, 1, false, &transform[0][0]);
		glVertexAttribPointer(vertexAttributePosition, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
		                      triangle->getVertexData());
		glVertexAttribPointer(textureCoordinatesAttributePosition, 2, GL_FLOAT, GL_FALSE,
		                      2 * sizeof(float), triangle->getUVData());
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	GLuint GLES2Renderer::getMaterialForRepresentation(char representation) const {

		return mRepresentations.at(representation);
	}

	void GLES2Renderer::moveForward() {
		nextAction = 'o';
	}

	void GLES2Renderer::moveBackward() {
	}

	void GLES2Renderer::turnRight() {
		nextAction = 'p';
	}

	void GLES2Renderer::turnLeft() {
		nextAction = 'i';
	}

	void GLES2Renderer::strafeRight() {
		nextAction = '>';
	}

	void GLES2Renderer::strageLeft() {
		nextAction = '<';
	}

	void GLES2Renderer::fire() {
		LOGI( "FIRE" );
		nextAction = 'f';
	}
}