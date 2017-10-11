#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

int main() {
	
	if (!glfwInit())
		return false;

	glfwWindowHint(GLFW_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_VISIBLE, GL_TRUE);

	GLFWwindow* a = glfwCreateWindow(500, 500, "PGR", nullptr, nullptr);
	
	glfwMakeContextCurrent(a);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		glfwTerminate();
		return false;
	}

	int w, h;
	glfwGetFramebufferSize(a, &w, &h);
	glViewport(0, 0, w, h);

	glm::vec3 color = glm::vec3(0.2, 0.8, 0.5);

	glClearColor(color.r, color.g, color.b, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	

	glfwSwapBuffers(a);
	system("pause");
	
	glfwTerminate();
	
	return 0;
}