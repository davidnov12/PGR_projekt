#pragma warning (disable : 4996)

#include "shaders.h"
#include "terrain.h"
#include "physics.h"
#include "camera.h"
#include "car.h"

#define WIDTH 1500
#define HEIGHT 1000

//#include <stdio.h>


int main() {

	if (!glfwInit())
		return false;

	glfwWindowHint(GLFW_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 16);

	GLFWwindow* window = glfwCreateWindow(800, 800, "PGR", nullptr, nullptr);

	glfwMakeContextCurrent(window);

	//Chyta vstupy z klavesnice
	glfwSetKeyCallback(window, key_callback);

	//updatuje pozici mysi
	glfwSetCursorPosCallback(window, mouse_callback);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		glfwTerminate();
		return false;
	}

	//vytvari programy pro shadery
	Shader shTeren("../shaders/shTeren.vert", "../shaders/shTeren.frag", 0);
	Shader carShader("../shaders/car.vs", "../shaders/car.fs", 0);

	//bere vstupy mysi ale neukazuje kurzor
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//teren
	Terrain* _terrain;
	_terrain = loadTerrain("../images/hgmap.bmp", 2);

	setupCarData();
	//carInitPos = glm::vec3(2.50257, 0.0844681, -0.403819);
	cameraPos = glm::vec3(4.26767, -0.0402569, -3.15559);

	//nejdulezitejsi pole ÅEobsahuje veskere vertexy terenu
	int g = 0;
	GLfloat* vertices;
	vertices = new GLfloat[(terrainy - 1)*terrainx * 16];
	for (int z = 0; z < terrainy - 1; z++)
		for (int x = 0; x < terrainx; x++)
		{
			//prvni vertex
			vertices[g] = (x - (int)(terrainx / 2)) / 10.0f;
			g++;
			vertices[g] = _terrain->getHeight(x, z) / 3.0f;
			g++;
			vertices[g] = (z - (int)(terrainy / 2)) / 10.0f;
			g++;
			//normala
			glm::fvec3 normala = _terrain->getNormaly(x, z); //vyrobi novou Vec3f normalu, nahraje do ni normalu daneho bodu (tzn. normal = normals[z][x]
			vertices[g] = normala[0];
			g++;
			vertices[g] = normala[1];
			g++;
			vertices[g] = normala[2];
			g++;
			//textura
			vertices[g] = (float)x / ((float)terrainx / 2);
			g++;
			vertices[g] = (float)z / ((float)terrainy / 2);
			g++;
			//protejsi vertex
			vertices[g] = (x - (int)(terrainx / 2)) / 10.0f;
			g++;
			vertices[g] = _terrain->getHeight(x, z + 1) / 3.0f;
			g++;
			vertices[g] = (z + 1 - (int)(terrainy / 2)) / 10.0f;
			g++;
			//protejsi normala
			normala = _terrain->getNormaly(x, z + 1); //vyrobi novou Vec3f normalu, nahraje do ni normalu daneho bodu (tzn. normal = normals[z][x]
			vertices[g] = normala[0];
			g++;
			vertices[g] = normala[1];
			g++;
			vertices[g] = normala[2];
			g++;
			//protejsi textura
			vertices[g] = (float)x / ((float)terrainx / 2);
			g++;
			vertices[g] = (float)(z + 1) / ((float)terrainy / 2);
			g++;
		}

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, ((terrainy - 1)*terrainx * 10) * 4, vertices, GL_STATIC_DRAW);
	//tri vertexy pro pozici
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//dalsi tri pro normalu
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	//dalsi dve pro texturu
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);


	///////////////////////////TEXTURY//////////////////////
	//textura pro travu
	unsigned char * travaTexData;
	travaTexData = loadTexture("../images/grass.bmp", 512, 512);
	GLuint travaTex;
	glGenTextures(1, &travaTex);
	glBindTexture(GL_TEXTURE_2D, travaTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, travaTexData);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);



	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE_ARB);
	//projekcni matice
	glm::mat4 projection = glm::perspective(20.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

	while (!glfwWindowShouldClose(window))
	{
		//Clear colorbuffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.45, 0.85, 1, 1);
		glfwPollEvents();
		do_movement();
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glm::mat4 model;
		glm::mat4 view;

		// -----AUTO-------
		// ----------------
		carShader.Use();

		glm::mat4 mod;

		mod = glm::translate(mod, cameraPos);
		mod = glm::rotate(mod, glm::radians(yaw), glm::vec3(0.0, 1.0, 0.0));
		mod = glm::scale(mod, glm::vec3(0.082));

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0, -0.64, -0.3) + carInitPos);
		model = glm::scale(model, glm::vec3(0.082));

		if (toLeft) model = glm::rotate(model, glm::radians(deltaTimeCentr > 1.2 ? -12.f : -10.f * deltaTimeCentr), glm::vec3(0.0, 1.0, 0.0));

		else if (toRight) model = glm::rotate(model, glm::radians(deltaTimeCentr > 1.2 ? 12.f : 10.f * deltaTimeCentr), glm::vec3(0.0, 1.0, 0.0));

		toLeft = toRight = false;

		glUniformMatrix4fv(glGetUniformLocation(carShader.Program, "trans"), 1, GL_FALSE, glm::value_ptr(mod));
		glUniformMatrix4fv(glGetUniformLocation(carShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniform3f(glGetUniformLocation(carShader.Program, "viewPos"), cameraPos.x, cameraPos.y, cameraPos.z);

		glBindVertexArray(carVAO);

		glDrawArrays(GL_TRIANGLES, 0, carVerticesCount);

		glBindVertexArray(0);
		// ----------------
		// -----AUTO-------


		///////////////////vykresleni terenu//////////////////////
		shTeren.Use();
		//model a view matice
		model = glm::mat4();
		view = glm::mat4();
		model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		//Uniformni promenne pro shadery
		GLint modelLocT = glGetUniformLocation(shTeren.Program, "model");
		GLint viewLocT = glGetUniformLocation(shTeren.Program, "view");
		GLint projLocT = glGetUniformLocation(shTeren.Program, "projection");
		glUniformMatrix4fv(modelLocT, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLocT, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLocT, 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(VAO);

		//textury
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, travaTex);
		glUniform1i(glGetUniformLocation(shTeren.Program, "Textura"), 0);

		//vykresleni
		for (int g = 0; g < terrainy - 1; g++)
		{
			glDrawArrays(GL_TRIANGLE_STRIP, g*(terrainx * 2), (terrainx * 2));
		}
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);

		

		//vymena bufferu
		glfwSwapBuffers(window);
	}

	clearCarData();

	return 0;
}