#include "Window.h"


// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "GLFW Starter Project";

// Skybox initialization
Skybox * Window::skybox;
unsigned int Window::skyboxTexture;

// Objects to Render
//Sphere* Window::disco;
Transform* Window::world;
Transform* Window::pillarMove;
SphereNode* Window::discoMove;
std::vector<Transform*> Window::barMove;
std::vector<Transform*> Window::torusAndBarMove;
std::vector<Geometry*> Window::potMove;

// Track key pressed
KeyRecord Window::keyPressed;

// Camera Matrices 
// Projection matrix:
glm::mat4 Window::projection; 

// View Matrix:
glm::vec3 Window::eyePos(0, 0, -20);			// Camera position.
glm::vec3 Window::lookAtPoint(0, 0, 0);		// The point we are looking at.
glm::vec3 Window::upVector(0, 1, 0);		// The up direction of the camera.
glm::mat4 Window::view = glm::lookAt(Window::eyePos, Window::lookAtPoint, Window::upVector);
glm::mat4 Window::skyboxView = glm::lookAt(Window::eyePos, Window::lookAtPoint, Window::upVector);

float Window::speed = 1;

// Light source properties
glm::vec3 Window::lightPos(-20, 100, -50);
glm::vec3 Window::lightColor(1, 1, 1);

// Shader Program ID
GLuint Window::phongShader; 
GLuint Window::skyboxShader;
GLuint Window::envmapShader;

bool Window::initializeProgram() {
	// Create a shader program with a vertex shader and a fragment shader.
	phongShader = LoadShaders("shaders/phong.vert", "shaders/phong.frag");
	skyboxShader = LoadShaders("shaders/skybox.vert", "shaders/skybox.frag");
	envmapShader = LoadShaders("shaders/envmap.vert", "shaders/envmap.frag");

	// Check the shader program.
	if (!phongShader || !skyboxShader || !envmapShader)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}

	return true;
}

bool Window::initializeObjects()
{
	/*
	std::vector<std::string> faces =
	{
		"skybox/Skybox_Water222_right.jpg",
		"skybox/Skybox_Water222_left.jpg",
		"skybox/Skybox_Water222_top.jpg",
		"skybox/Skybox_Water222_base.jpg",
		"skybox/Skybox_Water222_front.jpg",
		"skybox/Skybox_Water222_back.jpg"
	};
	*/

	std::vector<std::string> faces =
	{
		"skybox/right.jpg",
		"skybox/left.jpg",
		"skybox/top.jpg",
		"skybox/bottom.jpg",
		"skybox/front.jpg",
		"skybox/back.jpg"
	};

	// load skybox
	skybox = new Skybox(500.0f);
	skyboxTexture = skybox->loadCubemap(faces);

	// load disco
	// disco = new Sphere(5.0f);

	// initialize scene graph of the ride
	initializeWorld();

	return true;
}

void Window::initializeWorld() {
	world = new Transform(glm::mat4(1), 0);
	// world to land
	Transform* worldToLand = new Transform(glm::translate(glm::vec3(0, -30, 100)), 0);
	Geometry* land = new Geometry("models/land.obj", glm::vec3(0.12, 0.08, 0.07), glm::vec3(0.6, 0.4, 0.3), glm::vec3(0.5, 0.4, 0.4), glm::vec3(200, 1, 200), 0);

	// world to pillar
	Transform* landToPillar = new Transform(glm::translate(glm::vec3(0, 20, 0)), 1);
	Geometry* pillar = new Geometry("models/cylinder.obj", glm::vec3(0.02, 0.1, 0.15), glm::vec3(0, 0.6, 0.9), glm::vec3(0, 0, 0), glm::vec3(3, 50, 3), 0);
	// level 1 movement control
	pillarMove = landToPillar;

	// pillar to disco
	Transform* pillarToDisco = new Transform(glm::translate(glm::vec3(0, 50, 0)), 0);
	SphereNode* disco = new SphereNode(5.0f, envmapShader);
	discoMove = disco;

	// pillar to bar
	Transform* pillarToBar1 = new Transform(glm::translate(glm::vec3(0, 30, 0)) * glm::rotate(glm::radians(60.0f), glm::vec3(0, 0, 1)), 2);
	Transform* pillarToBar2 = new Transform(glm::translate(glm::vec3(0, 30, 0)) * glm::rotate(glm::radians(60.0f), glm::vec3(1, 0, 0)), 2);
	Transform* pillarToBar3 = new Transform(glm::translate(glm::vec3(0, 30, 0)) * glm::rotate(glm::radians(60.0f), glm::vec3(0, 0, -1)), 2);
	Transform* pillarToBar4 = new Transform(glm::translate(glm::vec3(0, 30, 0)) * glm::rotate(glm::radians(60.0f), glm::vec3(-1, 0, 0)), 2);
	// level 2 movement control
	barMove.push_back(pillarToBar1);
	barMove.push_back(pillarToBar2);
	barMove.push_back(pillarToBar3);
	barMove.push_back(pillarToBar4);
	// bars
	Geometry* bar1 = new Geometry("models/cylinder.obj", glm::vec3(0.12, 0.05, 0.15), glm::vec3(0.8, 0.3, 0.9), glm::vec3(0.9, 0.5, 0.8), glm::vec3(1, 50, 1), 0);
	Geometry* bar2 = new Geometry("models/cylinder.obj", glm::vec3(0.06, 0.15, 0.12), glm::vec3(0.4, 0.9, 0.8), glm::vec3(0.3, 0.8, 0.9), glm::vec3(1, 50, 1), 0);
	Geometry* bar3 = new Geometry("models/cylinder.obj", glm::vec3(0.15, 0.08, 0.08), glm::vec3(0.9, 0.3, 0.3), glm::vec3(0.9, 0.1, 0.1), glm::vec3(1, 50, 1), 0);
	Geometry* bar4 = new Geometry("models/cylinder.obj", glm::vec3(0.02, 0.15, 0.05), glm::vec3(0.1, 0.9, 0.2), glm::vec3(0.1, 0.8, 0.1), glm::vec3(1, 50, 1), 0);

	// bar to cone (as the cover of the hole)
	Transform* bar1ToCone = new Transform(glm::translate(glm::vec3(0, 52, 0)), 0);
	Transform* bar2ToCone = new Transform(glm::translate(glm::vec3(0, 52, 0)), 0);
	Transform* bar3ToCone = new Transform(glm::translate(glm::vec3(0, 52, 0)), 0);
	Transform* bar4ToCone = new Transform(glm::translate(glm::vec3(0, 52, 0)), 0);
	// cones
	Geometry* cone1 = new Geometry("models/cone.obj", glm::vec3(0.12, 0.05, 0.15), glm::vec3(0.8, 0.3, 0.9), glm::vec3(0.9, 0.5, 0.8), glm::vec3(0.45, 1, 0.45), 0);
	Geometry* cone2 = new Geometry("models/cone.obj", glm::vec3(0.06, 0.15, 0.12), glm::vec3(0.4, 0.9, 0.8), glm::vec3(0.3, 0.8, 0.9), glm::vec3(0.45, 1, 0.45), 0);
	Geometry* cone3 = new Geometry("models/cone.obj", glm::vec3(0.15, 0.08, 0.08), glm::vec3(0.9, 0.3, 0.3), glm::vec3(0.9, 0.1, 0.1), glm::vec3(0.45, 1, 0.45), 0);
	Geometry* cone4 = new Geometry("models/cone.obj", glm::vec3(0.02, 0.15, 0.05), glm::vec3(0.1, 0.9, 0.2), glm::vec3(0.1, 0.8, 0.1), glm::vec3(0.45, 1, 0.45), 0);

	// bar to torus and torus bar
	Transform* bar1ToTorus = new Transform(glm::translate(glm::vec3(0, 50, 0)), 3);
	Transform* bar2ToTorus = new Transform(glm::translate(glm::vec3(0, 50, 0)), 3);
	Transform* bar3ToTorus = new Transform(glm::translate(glm::vec3(0, 50, 0)), 3);
	Transform* bar4ToTorus = new Transform(glm::translate(glm::vec3(0, 50, 0)), 3);
	Transform* bar1ToTorusBar = new Transform(glm::translate(glm::vec3(0, 50, 0)) * glm::rotate(glm::radians(90.0f), glm::vec3(0, 0, 1)), 3);
	Transform* bar2ToTorusBar = new Transform(glm::translate(glm::vec3(0, 50, 0)) * glm::rotate(glm::radians(90.0f), glm::vec3(0, 0, 1)), 3);
	Transform* bar3ToTorusBar = new Transform(glm::translate(glm::vec3(0, 50, 0)) * glm::rotate(glm::radians(90.0f), glm::vec3(0, 0, 1)), 3);
	Transform* bar4ToTorusBar = new Transform(glm::translate(glm::vec3(0, 50, 0)) * glm::rotate(glm::radians(90.0f), glm::vec3(0, 0, 1)), 3);
	// level 3 movement control
	torusAndBarMove.push_back(bar1ToTorus);
	torusAndBarMove.push_back(bar1ToTorusBar);
	torusAndBarMove.push_back(bar2ToTorus);
	torusAndBarMove.push_back(bar2ToTorusBar);
	torusAndBarMove.push_back(bar3ToTorus);
	torusAndBarMove.push_back(bar3ToTorusBar);
	torusAndBarMove.push_back(bar4ToTorus);
	torusAndBarMove.push_back(bar4ToTorusBar);
	// toruses and torus bars
	Geometry* torus1 = new Geometry("models/torus_hr.obj", glm::vec3(0.1, 0.1, 0.05), glm::vec3(0), glm::vec3(0.99, 0.96, 0.6), glm::vec3(10, 5, 10), 0);
	Geometry* torus2 = new Geometry("models/torus_hr.obj", glm::vec3(0.05, 0.1, 0.1), glm::vec3(0), glm::vec3(0.6, 0.96, 0.99), glm::vec3(10, 5, 10), 0);
	Geometry* torus3 = new Geometry("models/torus_hr.obj", glm::vec3(0.1, 0.1, 0.05), glm::vec3(0), glm::vec3(0.99, 0.96, 0.6), glm::vec3(10, 5, 10), 0);
	Geometry* torus4 = new Geometry("models/torus_hr.obj", glm::vec3(0.05, 0.1, 0.1), glm::vec3(0), glm::vec3(0.6, 0.96, 0.99), glm::vec3(10, 5, 10), 0);
	Geometry* torusBar1 = new Geometry("models/cylinder.obj", glm::vec3(0.1, 0.1, 0.1), glm::vec3(0), glm::vec3(1), glm::vec3(1, 10, 1), 0);
	Geometry* torusBar2 = new Geometry("models/cylinder.obj", glm::vec3(0.1, 0.1, 0.1), glm::vec3(0), glm::vec3(1), glm::vec3(1, 10, 1), 0);
	Geometry* torusBar3 = new Geometry("models/cylinder.obj", glm::vec3(0.1, 0.1, 0.1), glm::vec3(0), glm::vec3(1), glm::vec3(1, 10, 1), 0);
	Geometry* torusBar4 = new Geometry("models/cylinder.obj", glm::vec3(0.1, 0.1, 0.1), glm::vec3(0), glm::vec3(1), glm::vec3(1, 10, 1), 0);

	// bar to pot
	Transform* bar1ToPot = new Transform(glm::translate(glm::vec3(0, -50, 0)) * glm::rotate(glm::radians(-60.0f), glm::vec3(0, 0, 1)), 0);
	Transform* bar2ToPot = new Transform(glm::translate(glm::vec3(0, -50, 0)) * glm::rotate(glm::radians(-60.0f), glm::vec3(1, 0, 0)), 0);
	Transform* bar3ToPot = new Transform(glm::translate(glm::vec3(0, -50, 0)) * glm::rotate(glm::radians(-60.0f), glm::vec3(0, 0, -1)), 0);
	Transform* bar4ToPot = new Transform(glm::translate(glm::vec3(0, -50, 0)) * glm::rotate(glm::radians(-60.0f), glm::vec3(-1, 0, 0)), 0);
	// pots
	Geometry* pot1 = new Geometry("models/teapot.obj", glm::vec3(0.1, 0.1, 0.1), glm::vec3(0.3, 0.5, 0.5), glm::vec3(0.7, 0.8, 0.9), glm::vec3(0.2), 1);
	Geometry* pot2 = new Geometry("models/teapot.obj", glm::vec3(0.1, 0.1, 0.1), glm::vec3(0.3, 0.5, 0.5), glm::vec3(0.7, 0.8, 0.9), glm::vec3(0.2), 2);
	Geometry* pot3 = new Geometry("models/teapot.obj", glm::vec3(0.1, 0.1, 0.1), glm::vec3(0.3, 0.5, 0.5), glm::vec3(0.7, 0.8, 0.9), glm::vec3(0.2), 1);
	Geometry* pot4 = new Geometry("models/teapot.obj", glm::vec3(0.1, 0.1, 0.1), glm::vec3(0.3, 0.5, 0.5), glm::vec3(0.7, 0.8, 0.9), glm::vec3(0.2), 2);
	// level 3 movement control, geometry self rotation
	potMove.push_back(pot1);
	potMove.push_back(pot2);
	potMove.push_back(pot3);
	potMove.push_back(pot4);

	// add land and pillar
	world->addChild(worldToLand);
	worldToLand->addChild(land);
	land->addChild(landToPillar);
	landToPillar->addChild(pillar);

	// add disco ball to pillar
	pillar->addChild(pillarToDisco);
	pillarToDisco->addChild(disco);

	// add bars to pillar
	pillar->addChild(pillarToBar1);
	pillar->addChild(pillarToBar2);
	pillar->addChild(pillarToBar3);
	pillar->addChild(pillarToBar4);
	pillarToBar1->addChild(bar1);
	pillarToBar2->addChild(bar2);
	pillarToBar3->addChild(bar3);
	pillarToBar4->addChild(bar4);

	// add cones to bars
	bar1->addChild(bar1ToCone);
	bar2->addChild(bar2ToCone);
	bar3->addChild(bar3ToCone);
	bar4->addChild(bar4ToCone);
	bar1ToCone->addChild(cone1);
	bar2ToCone->addChild(cone2);
	bar3ToCone->addChild(cone3);
	bar4ToCone->addChild(cone4);

	// link torus and bar to bar1
	bar1->addChild(bar1ToTorus);
	bar1->addChild(bar1ToTorusBar);
	bar1ToTorus->addChild(torus1);
	bar1ToTorusBar->addChild(torusBar1);
	// link torus and bar to bar2
	bar2->addChild(bar2ToTorus);
	bar2->addChild(bar2ToTorusBar);
	bar2ToTorus->addChild(torus2);
	bar2ToTorusBar->addChild(torusBar2);
	// link torus and bar to bar3
	bar3->addChild(bar3ToTorus);
	bar3->addChild(bar3ToTorusBar);
	bar3ToTorus->addChild(torus3);
	bar3ToTorusBar->addChild(torusBar3);
	// link torus and bar to bar4
	bar4->addChild(bar4ToTorus);
	bar4->addChild(bar4ToTorusBar);
	bar4ToTorus->addChild(torus4);
	bar4ToTorusBar->addChild(torusBar4);

	// add pots
	bar1->addChild(bar1ToPot);
	bar1ToPot->addChild(pot1);
	bar2->addChild(bar2ToPot);
	bar2ToPot->addChild(pot2);
	bar3->addChild(bar3ToPot);
	bar3ToPot->addChild(pot3);
	bar4->addChild(bar4ToPot);
	bar4ToPot->addChild(pot4);
}
void Window::cleanUp()
{
	// Deallcoate the objects.
	delete skybox;

	// Delete the shader program.
	glDeleteProgram(phongShader);
	glDeleteProgram(skyboxShader);
	glDeleteProgram(envmapShader);
}

GLFWwindow* Window::createWindow(int width, int height)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}

	// 4x antialiasing.
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ 
	// Apple implements its own version of OpenGL and requires special treatments
	// to make it uses modern OpenGL.

	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window.
	GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

	// Check if the window could not be created.
	if (!window)
	{
		std::cerr << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window.
	glfwMakeContextCurrent(window);

#ifndef __APPLE__
	// On Windows and Linux, we need GLEW to provide modern OpenGL functionality.

	// Initialize GLEW.
	if (glewInit())
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return NULL;
	}
#endif

	// Set swap interval to 1.
	glfwSwapInterval(0);

	// Call the resize callback to make sure things get drawn immediately.
	Window::resizeCallback(window, width, height);

	return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	// In case your Mac has a retina display.
	glfwGetFramebufferSize(window, &width, &height); 
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size.
	glViewport(0, 0, width, height);

	// Set the projection matrix.
	Window::projection = glm::perspective(glm::radians(60.0), 
								double(width) / (double)height, 1.0, 1000.0);
}

// Perform any necessary updates here 
void Window::idleCallback()
{
	// Move according to key pressed
	movement();
	// disco->update();
	world->update();
}

void Window::displayCallback(GLFWwindow* window)
{	
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	
	// Render the skybox
	skybox->draw(skyboxView, projection, skyboxShader);

	// Render the disco ball
	// Use env map shader
	glUseProgram(envmapShader);
	glUniformMatrix4fv(glGetUniformLocation(envmapShader, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(envmapShader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniform3fv(glGetUniformLocation(envmapShader, "eyePos"), 1, glm::value_ptr(Window::eyePos));
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
	// disco->draw(view, projection, envmapShader);

	// Render the objects
	glUseProgram(phongShader);
	glUniformMatrix4fv(glGetUniformLocation(phongShader, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(phongShader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniform3fv(glGetUniformLocation(phongShader, "eyePos"), 1, glm::value_ptr(eyePos));
	glUniform3fv(glGetUniformLocation(phongShader, "lightPos"), 1, glm::value_ptr(lightPos));
	glUniform3fv(glGetUniformLocation(phongShader, "lightColor"), 1, glm::value_ptr(lightColor));
	world->draw(glm::mat4(1), phongShader);

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();

	// Swap buffers.
	glfwSwapBuffers(window);
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press.
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);				
			break;

		case GLFW_KEY_Q:
			keyPressed.qPressed = true;
			break;

		case GLFW_KEY_E:
			keyPressed.ePressed = true;
			break;

		case GLFW_KEY_W:
			keyPressed.wPressed = true;
			break;

		case GLFW_KEY_A:
			keyPressed.aPressed = true;
			break;

		case GLFW_KEY_S:
			keyPressed.sPressed = true;
			break;

		case GLFW_KEY_D:
			keyPressed.dPressed = true;
			break;

		case GLFW_KEY_LEFT_CONTROL:
			keyPressed.ctrlPressed = true;
			break;

		case GLFW_KEY_LEFT_SHIFT:
			keyPressed.shiftPressed = true;
			break;

		case GLFW_KEY_1:
			pillarMove->toggleMove();
			break;

		case GLFW_KEY_2:
			for (auto move : barMove) {
				move->toggleMove();
			}
			discoMove->toggleMove();
			break;

		case GLFW_KEY_3:
			for (auto move : torusAndBarMove) {
				move->toggleMove();
			}
			for (auto move : potMove) {
				move->toggleMove();
			}
			break;

		default:
			break;
		}
	}

	else {
		keyPressed.qPressed = false;
		keyPressed.ePressed = false;
		keyPressed.wPressed = false;
		keyPressed.aPressed = false;
		keyPressed.sPressed = false;
		keyPressed.dPressed = false;
		keyPressed.ctrlPressed = false;
		keyPressed.shiftPressed = false;
	}
}

void Window::movement() {
	if (keyPressed.qPressed) {
		glm::mat4 rotate = glm::rotate(glm::mat4(1), glm::radians(0.5f), upVector);
		lookAtPoint = eyePos + glm::vec3(rotate * glm::vec4(lookAtPoint - eyePos, 0));
            skyboxView = glm::inverse(rotate) * skyboxView;
	} else if (keyPressed.ePressed) {
		glm::mat4 rotate = glm::rotate(glm::mat4(1), glm::radians(-0.5f), upVector);
		lookAtPoint = eyePos + glm::vec3(rotate * glm::vec4(lookAtPoint - eyePos, 0));
            skyboxView = glm::inverse(rotate) * skyboxView;
	}

	if (keyPressed.wPressed) {
		glm::vec3 forward = glm::normalize(lookAtPoint - eyePos);
		lookAtPoint += forward;
		eyePos += forward;
	}

	if (keyPressed.aPressed) {
		glm::vec3 leftward = glm::normalize(glm::cross(upVector, lookAtPoint - eyePos));
		lookAtPoint += leftward;
		eyePos += leftward;
	}

	if (keyPressed.sPressed) {
		glm::vec3 backward = glm::normalize(eyePos - lookAtPoint);
		lookAtPoint += backward;
		eyePos += backward;
	}

	if (keyPressed.dPressed) {
		glm::vec3 rightward = glm::normalize(glm::cross(lookAtPoint - eyePos, upVector));
		lookAtPoint += rightward;
		eyePos += rightward;
	}

	if (keyPressed.ctrlPressed) {
		glm::vec3 downward = -upVector;
		lookAtPoint += downward;
		eyePos += downward;
	}

	if (keyPressed.shiftPressed) {
		glm::vec3 upward = upVector;
		lookAtPoint += upward;
		eyePos += upward;
	}

      view = glm::lookAt(Window::eyePos, Window::lookAtPoint, Window::upVector);
}
