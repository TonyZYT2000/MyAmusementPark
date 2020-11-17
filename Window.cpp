#include "Window.h"


// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "GLFW Starter Project";

// Skybox initialization
Skybox * Window::skybox;
unsigned int Window::skyboxTexture;

// Objects to Render
Sphere* Window::disco;

// Track key pressed
KeyRecord Window::keyPressed;

// Camera Matrices 
// Projection matrix:
glm::mat4 Window::projection; 

// View Matrix:
glm::vec3 Window::eyePos(0, 0, 20);			// Camera position.
glm::vec3 Window::lookAtPoint(0, 0, 0);		// The point we are looking at.
glm::vec3 Window::upVector(0, 1, 0);		// The up direction of the camera.
glm::mat4 Window::view = glm::lookAt(Window::eyePos, Window::lookAtPoint, Window::upVector);
glm::mat4 Window::skyboxView = glm::lookAt(Window::eyePos, Window::lookAtPoint, Window::upVector);

float Window::speed = 1;

// Shader Program ID
GLuint Window::phongShader; 
GLuint Window::skyboxShader;
GLuint Window::envmapShader;

bool Window::initializeProgram() {
	// Create a shader program with a vertex shader and a fragment shader.
	phongShader = LoadShaders("shaders/shader.vert", "shaders/shader.frag");
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
	disco = new Sphere(5.0f);

	return true;
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
	// std::cerr << eyePos.x << ", " << eyePos.y << ", " << eyePos.z << std::endl;
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
	glUniform3fv(glGetUniformLocation(envmapShader, "eyePos"), 1, glm::value_ptr(Window::eyePos));
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
	disco->draw(view, projection, envmapShader);

	// Render the objects
	// currObj->draw(view, projection, phongShader);

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

      view = glm::lookAt(Window::eyePos, Window::lookAtPoint, Window::upVector);
}
