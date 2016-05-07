//
// Created by monty on 23/11/15.
//

#ifndef LESSON02_GLES2LESSON_H
#define LESSON02_GLES2LESSON_H

namespace odb {

    class GLES2Renderer : public WizardOfGalicia::IRenderer {

	    void fetchShaderLocations();

	    void setPerspective();

	    void prepareShaderProgram();

	    void clearBuffers();

	    void resetTransformMatrices();

	    void printVerboseDriverInformation();

	    GLint createProgram(const char *pVertexSource, const char *pFragmentSource);

	    GLint loadShader(GLenum shaderType, const char *pSource);

	    glm::mat4 projectionMatrix;
	    glm::mat4 viewMatrix;
//		std::mutex renderingMutex;
	    GLint vertexAttributePosition;
	    GLint modelMatrixAttributePosition;
	    GLint viewMatrixAttributePosition;
	    GLint samplerUniformPosition;
	    GLint textureCoordinatesAttributePosition;
	    GLint projectionMatrixAttributePosition;
	    GLint gProgram;
	    WalkBouncer mBouncer;
	    GLuint textureId;
	    glm::vec3 camera;
	    glm::vec3 cameraDirection;
	    std::vector<std::shared_ptr<NativeBitmap> > mTextures;
	    std::vector<unsigned int> mMaterials;
	    std::vector<std::shared_ptr<SceneElement>> mSceneElements;
	    std::vector<std::shared_ptr<SceneElement>> mCharacters;
	    std::map< std::shared_ptr<WizardOfGalicia::CActor>,std::shared_ptr<SceneElement>> mQuadsForCharacters;

    public:
	    explicit GLES2Renderer();

	    ~GLES2Renderer();

	    bool init(float w, float h, const std::string &vertexShader,
	              const std::string &fragmentShader);

	    void render();

	    void shutdown();

	    void tick(WizardOfGalicia::CGame &game);

	    void reset();

	    void addTrigsForTexture(GLint textureId, const std::vector<Trig> &newTrigs);

	    void setTextures(std::vector<std::shared_ptr<NativeBitmap>> vector,
		                         std::map<std::shared_ptr<NativeBitmap>,char> map);

	    virtual void playFireballSound() override;

	    virtual void playMeeleeSound() override;

	    virtual void playPowerUpSound() override;

	    virtual bool waitingForKey() override;

	    virtual void init(std::shared_ptr<WizardOfGalicia::CMap> map) override;

	    virtual void drawMap(WizardOfGalicia::CMap &map,
	                         std::shared_ptr<WizardOfGalicia::CActor> current) override;

	    virtual char update() override;

	    virtual void showTitleScreen() override;

	    virtual void showGameOverScreen() override;

	    virtual void showVictoryScreen() override;

	    virtual void showInstructionsScreen() override;

	    void drawTriangle(std::shared_ptr<Trig> triangle, glm::mat4 x4);

	    std::shared_ptr<SceneElement> makeQuadElementAt(int y, int x, char representation) const;

	    GLuint getMaterialForRepresentation(char representation) const;

	    std::map< char, GLuint> mRepresentations;

	    void moveForward();

	    void moveBackward();

	    void turnRight();

	    void turnLeft();

	    void strafeRight();

	    void strageLeft();

	    char nextAction;

	    void fire();

	    std::shared_ptr<SceneElement> tileMap[ 20 ][ 20 ];

	    glm::mediump_vec3 getVec3For(int x, int y) const;
    };
}
#endif //LESSON02_GLES2LESSON_H
