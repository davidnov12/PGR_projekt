#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define WIDTH 1500
#define HEIGHT 1000

GLfloat deltaTime = 0.0f;	// èas mezi minulym a nynejsim snimkem
GLfloat lastFrame = 0.0f;  	// èas posledniho snimku
							//pro zachytavani klavesnice
bool keys[1024];

//definuje pocatecni pozici a smer kamery
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

//nastaveni pro pohyb kamerou pomoci mysi
GLfloat yaw = -90.0f;
GLfloat pitch = 0.0f;
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool firstMouse = true;

bool toLeft = false, toRight = false;

//Obsluhuje vstupy z klavesnice
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	//escape vypne program
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;
}

//funkce pro pohyb kamerou pomoci klavesnice. Prevzato z https://learnopengl.com/#!Getting-started/Camera
void do_movement()
{

	calcSpeed();
	// Camera controls
	GLfloat cameraSpeed = carSpeed * deltaTime;
	cameraPos += cameraSpeed * cameraFront;

	if (keys[GLFW_KEY_W]) {
		//cameraPos += move;
		carForward = true;
	}
	if (keys[GLFW_KEY_S]) {
		//cameraPos -= move;
		carBrake = true;
	}
	if (keys[GLFW_KEY_A]) {
		yaw -= cameraSpeed * 40.f;//cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		toLeft = true;
		calcCentrifugal(toRight, toLeft, deltaTime);
		cameraPos += glm::cross(cameraFront, cameraUp) * centrifugalPower;
	}
	if (keys[GLFW_KEY_D]) {
		yaw += cameraSpeed * 40.f; //cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		toRight = true;
		calcCentrifugal(toRight, toLeft, deltaTime);
		cameraPos += glm::cross(cameraFront, cameraUp) * centrifugalPower;
	}
	if (!keys[GLFW_KEY_A] && !keys[GLFW_KEY_D] && deltaTimeCentr > 0.0)
		deltaTimeCentr -= 0.2;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);


	//std::cout << cameraPos.x << " " << cameraPos.y << " " << cameraPos.z << std::endl;
}

//funkce pro pohyb kamerou pomoci mysi. Prevzato z https://learnopengl.com/#!Getting-started/Camera
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to left
	lastX = xpos;
	lastY = ypos;

	GLfloat sensitivity = 0.07;	// Change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	//yaw += xoffset;
	pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}