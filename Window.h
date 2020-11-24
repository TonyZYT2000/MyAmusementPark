#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "main.h"
#include "shader.h"
#include "Object.h"
#include "Cube.h"
#include "Skybox.h"
#include "Sphere.h"
#include "Transform.h"
#include "Geometry.h"
#include "SphereNode.h"

struct KeyRecord {
	bool qPressed;
	bool ePressed;
	bool wPressed;
	bool aPressed;
	bool sPressed;
	bool dPressed;
	bool ctrlPressed;
	bool shiftPressed;
};

class Window
{
public:

	// Window Properties
	static int width;
	static int height;
	static const char* windowTitle;

	// Skybox
	static Skybox* skybox;
	static unsigned int skyboxTexture;

	// Objects to Render
	// static Sphere* disco;
	static Transform* world;
	static Transform* pillarMove;
	static Transform* discoMove;
	static std::vector<Transform*> barMove;
	static std::vector<Transform*> torusAndBarMove;
	static std::vector<Geometry*> potMove;

	// Camera Matrices
	static glm::mat4 projection;
	static glm::mat4 view;
	static glm::mat4 skyboxView;
	static glm::vec3 eyePos, lookAtPoint, upVector;

	// Light Source
	static glm::vec3 lightPos;
	static glm::vec3 lightColor;

	// Shader Program ID
	static GLuint phongShader;
	static GLuint skyboxShader;
	static GLuint envmapShader;

	// Constructors and Destructors
	static bool initializeProgram();
	static bool initializeObjects();
	static void cleanUp();

	static void initializeWorld();

	// Window functions
	static GLFWwindow* createWindow(int width, int height);
	static void resizeCallback(GLFWwindow* window, int width, int height);

	// Draw and Update functions
	static void idleCallback();
	static void displayCallback(GLFWwindow*);

	// Callbacks
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	static KeyRecord keyPressed;
	static void movement();
	static float speed;
};

#endif
