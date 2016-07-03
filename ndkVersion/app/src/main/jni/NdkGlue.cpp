/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "GLES2/gl2.h"

#include <jni.h>
#include <android/bitmap.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/log.h>
#include <memory>
#include <vector>
#include <thread>
#include <mutex>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <unordered_set>
#include <map>

#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <map>

#include "Vec2i.h"
#include "IMapElement.h"
#include "CActor.h"
#include "CDoorway.h"
#include "CWizard.h"
#include "CMap.h"

#include "IRenderer.h"
#include "CGame.h"


#include "NativeBitmap.h"
#include "Trig.h"

#include "IComponent.h"
#include "SceneElement.h"
#include "MeshComponent.h"

#include "GLES2Renderer.h"
#include "NdkGlue.h"

#include "android_asset_operations.h"

WizardOfGalicia::CGame game;

std::string gVertexShader;
std::string gFragmentShader;
std::string worldData;
std::map<GLuint, std::vector<Trig> > batches;
odb::GLES2Renderer *gles2Lesson = nullptr;
std::vector<std::shared_ptr<NativeBitmap>> textures;
std::map<std::shared_ptr<NativeBitmap>, char> representations;

void loadShaders(JNIEnv *env, jobject &obj) {
	AAssetManager *asset_manager = AAssetManager_fromJava(env, obj);
	FILE *fd;
	fd = android_fopen("vertex.glsl", "r", asset_manager);
	gVertexShader = readToString(fd);
	fclose(fd);
	fd = android_fopen("fragment.glsl", "r", asset_manager);
	gFragmentShader = readToString(fd);
	fclose(fd);
}

bool setupGraphics(int w, int h) {
	gles2Lesson = new odb::GLES2Renderer();

	gles2Lesson->setTextures(textures, representations);

	for (auto &pair : batches) {
		gles2Lesson->addTrigsForTexture(pair.first, pair.second);
	}

	game.runGame(gles2Lesson, worldData);

	return gles2Lesson->init(w, h, gVertexShader.c_str(), gFragmentShader.c_str());
}

void renderFrame() {
	if (gles2Lesson != nullptr) {
		gles2Lesson->render();
	}
}

void shutdown() {
	odb::GLES2Renderer *local = gles2Lesson;
	gles2Lesson = nullptr;
	local->shutdown();
	delete local;
}

void tick() {
	if (gles2Lesson != nullptr) {
		gles2Lesson->tick(game);
	}
}

extern "C" {
JNIEXPORT void JNICALL Java_br_odb_wog_GL2JNILib_onCreate(JNIEnv *env, void *reserved,
                                                          jobject assetManager);

JNIEXPORT void JNICALL
		Java_br_odb_wog_GL2JNILib_setTexture(JNIEnv *env, jclass type, jobjectArray bitmap,
		                                     jcharArray chars_);

JNIEXPORT void JNICALL Java_br_odb_wog_GL2JNILib_onDestroy(JNIEnv *env, jobject obj);

JNIEXPORT void JNICALL Java_br_odb_wog_GL2JNILib_init(JNIEnv *env, jobject obj,
                                                      jint width, jint height);
JNIEXPORT void JNICALL Java_br_odb_wog_GL2JNILib_step(JNIEnv *env, jobject obj);

JNIEXPORT void JNICALL Java_br_odb_wog_GL2JNILib_tick(JNIEnv *env, jobject obj);

JNIEXPORT void JNICALL
		Java_br_odb_wog_GL2JNILib_reset(JNIEnv *env, jclass type);

JNIEXPORT void JNICALL
		Java_br_odb_wog_GL2JNILib_strafeRight(JNIEnv *env, jclass type);

JNIEXPORT void JNICALL
		Java_br_odb_wog_GL2JNILib_setEyeMatrix(JNIEnv *env, jclass type, jfloatArray eyeMatrix_);

JNIEXPORT void JNICALL
		Java_br_odb_wog_GL2JNILib_fire(JNIEnv *env, jclass type);

JNIEXPORT void JNICALL
		Java_br_odb_wog_GL2JNILib_setPerspectiveMatrix(JNIEnv *env, jclass type, jfloatArray perspective_);

JNIEXPORT void JNICALL
		Java_br_odb_wog_GL2JNILib_strafeLeft(JNIEnv *env, jclass type);

JNIEXPORT void JNICALL
		Java_br_odb_wog_GL2JNILib_moveForward(JNIEnv *env, jclass type);

JNIEXPORT void JNICALL
		Java_br_odb_wog_GL2JNILib_moveBackward(JNIEnv *env, jclass type);

JNIEXPORT void JNICALL
		Java_br_odb_wog_GL2JNILib_turnRight(JNIEnv *env, jclass type);

JNIEXPORT void JNICALL
		Java_br_odb_wog_GL2JNILib_turnLeft(JNIEnv *env, jclass type);

JNIEXPORT void JNICALL
		Java_br_odb_wog_GL2JNILib_onTouchNormalized(JNIEnv *env, jclass type, jfloat x,
		                                            jfloat y);

JNIEXPORT void JNICALL
		Java_br_odb_wog_GL2JNILib_setLookAtMatrix(JNIEnv *env, jclass type, jfloatArray lookAt_);

JNIEXPORT void JNICALL
		Java_br_odb_wog_GL2JNILib_setXZAngle(JNIEnv *env, jclass type, jfloat xz);
}

float toFloat(const char *nptr) {
	return (strtod(nptr, NULL));
}

void readWorld(JNIEnv *env, void *reserved,
               jobject assetManager) {


	AAssetManager *asset_manager = AAssetManager_fromJava(env, assetManager);
	FILE *fd;
	fd = android_fopen("map1.txt", "r", asset_manager);
	worldData = readToString(fd);
	fclose(fd);

	auto position = worldData.find('\n');

	while (position != std::string::npos) {
		worldData.replace(position, 1, "");
		position = worldData.find('\n', position + 1);
	}
}

void JNICALL Java_br_odb_wog_GL2JNILib_onCreate(JNIEnv *env, void *reserved,
                                                jobject assetManager) {
	loadShaders(env, assetManager);
	readWorld(env, reserved, assetManager);
}

JNIEXPORT void JNICALL Java_br_odb_wog_GL2JNILib_init(JNIEnv *env, jobject obj,
                                                      jint width, jint height) {
	setupGraphics(width, height);
}

JNIEXPORT void JNICALL Java_br_odb_wog_GL2JNILib_step(JNIEnv *env, jobject obj) {
	renderFrame();
}

JNIEXPORT void JNICALL Java_br_odb_wog_GL2JNILib_tick(JNIEnv *env, jobject obj) {
	tick();
}

JNIEXPORT void JNICALL Java_br_odb_wog_GL2JNILib_onDestroy(JNIEnv *env, jobject obj) {
	shutdown();
}

std::shared_ptr<NativeBitmap> makeNativeBitmapFromJObject(JNIEnv *env, jobject bitmap) {

	void *addr;
	AndroidBitmapInfo info;
	int errorCode;

	if ((errorCode = AndroidBitmap_lockPixels(env, bitmap, &addr)) != 0) {
		LOGI("error %d", errorCode);
	}

	if ((errorCode = AndroidBitmap_getInfo(env, bitmap, &info)) != 0) {
		LOGI("error %d", errorCode);
	}

	LOGI("bitmap info: %d wide, %d tall, %d ints per pixel", info.width, info.height, info.format);


	long size = info.width * info.height * info.format;
	int *pixels = new int[size];
	memcpy(pixels, addr, size * sizeof(int));
	auto toReturn = std::make_shared<NativeBitmap>(128, 128, pixels);

	if ((errorCode = AndroidBitmap_unlockPixels(env, bitmap)) != 0) {
		LOGI("error %d", errorCode);
	}

	return toReturn;
}

JNIEXPORT void JNICALL
Java_br_odb_wog_GL2JNILib_reset(JNIEnv *env, jclass type) {
	if (gles2Lesson != nullptr) {
		gles2Lesson->reset();
	}
}

JNIEXPORT void JNICALL
Java_br_odb_wog_GL2JNILib_moveForward(JNIEnv *env, jclass type) {
	if (gles2Lesson != nullptr) {
		gles2Lesson->moveForward();
		gles2Lesson->tick(game);
	}
}

JNIEXPORT void JNICALL
Java_br_odb_wog_GL2JNILib_moveBackward(JNIEnv *env, jclass type) {
	if (gles2Lesson != nullptr) {
		gles2Lesson->moveBackward();
		gles2Lesson->tick(game);
	}
}

JNIEXPORT void JNICALL
Java_br_odb_wog_GL2JNILib_turnRight(JNIEnv *env, jclass type) {
	if (gles2Lesson != nullptr) {
		gles2Lesson->turnRight();
		gles2Lesson->tick(game);
	}
}

JNIEXPORT void JNICALL
Java_br_odb_wog_GL2JNILib_turnLeft(JNIEnv *env, jclass type) {
	if (gles2Lesson != nullptr) {
		gles2Lesson->turnLeft();
		gles2Lesson->tick(game);
	}
}

JNIEXPORT void JNICALL
Java_br_odb_wog_GL2JNILib_onTouchNormalized(JNIEnv *env, jclass type, jfloat x,
                                            jfloat y) {
}


JNIEXPORT void JNICALL
Java_br_odb_wog_GL2JNILib_setTexture(JNIEnv *env, jclass type, jobjectArray bitmaps,
                                     jcharArray representation) {

	jboolean trueAfterAll = true;
	int length = env->GetArrayLength(bitmaps);
	for (int c = 0; c < length; ++c) {
		auto bitmap = makeNativeBitmapFromJObject(env, env->GetObjectArrayElement(bitmaps, c));
		textures.push_back(bitmap);
		auto element = env->GetCharArrayElements(representation, &trueAfterAll)[c];
		representations[bitmap] = element;
	}
}

JNIEXPORT void JNICALL
Java_br_odb_wog_GL2JNILib_setLookAtMatrix(JNIEnv *env, jclass type, jfloatArray lookAt_) {
	jfloat *lookAt = env->GetFloatArrayElements(lookAt_, NULL);
	env->ReleaseFloatArrayElements(lookAt_, lookAt, 0);
}

JNIEXPORT void JNICALL
Java_br_odb_wog_GL2JNILib_setXZAngle(JNIEnv *env, jclass type, jfloat xz) {
}

JNIEXPORT void JNICALL
Java_br_odb_wog_GL2JNILib_strafeRight(JNIEnv *env, jclass type) {
	if (gles2Lesson != nullptr) {
		gles2Lesson->strafeRight();
		gles2Lesson->tick(game);
	}
}

JNIEXPORT void JNICALL
Java_br_odb_wog_GL2JNILib_strafeLeft(JNIEnv *env, jclass type) {
	if (gles2Lesson != nullptr) {
		gles2Lesson->strageLeft();
		gles2Lesson->tick(game);
	}
}

JNIEXPORT void JNICALL
Java_br_odb_wog_GL2JNILib_fire(JNIEnv *env, jclass type) {
	if (gles2Lesson != nullptr) {
		gles2Lesson->fire();
		gles2Lesson->tick(game);
	}
}

JNIEXPORT void JNICALL
Java_br_odb_wog_GL2JNILib_setEyeMatrix(JNIEnv *env, jclass type, jfloatArray eyeMatrix_) {
	jfloat *eyeMatrix = env->GetFloatArrayElements(eyeMatrix_, NULL);

	if (gles2Lesson != nullptr) {
		gles2Lesson->setViewMatrix(eyeMatrix);
	}

	env->ReleaseFloatArrayElements(eyeMatrix_, eyeMatrix, 0);
}

JNIEXPORT void JNICALL
Java_br_odb_wog_GL2JNILib_setPerspectiveMatrix(JNIEnv *env, jclass type, jfloatArray perspective_) {
	jfloat *perspective = env->GetFloatArrayElements(perspective_, NULL);

	if (gles2Lesson != nullptr) {
		gles2Lesson->setPerspectiveMatrix(perspective);
	}

	env->ReleaseFloatArrayElements(perspective_, perspective, 0);
}