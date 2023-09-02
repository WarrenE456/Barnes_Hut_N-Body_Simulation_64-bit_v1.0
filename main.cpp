#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <fstream>
#include <sstream>
 
float zoom;

//headers without implementation file
#include "checkError.h"
#include "enum.h"
#include "parseCompileCreateShader.h"
#include "scrollCallback.h"
#include "shaderPair.h"
#include "matrix.h"
#include "vectors.h"
#include "configurations.h"

//classes
#include "particle.h"
#include "octNode.h"
#include "quadNode.h"
#include "timer.h"

//functions
#include "THREADcreateRootNodeBoundingBox.h"
#include "THREADgravityInsertDissassemble.h"
#include "THREADloadingProgressLog.h"
#include "THREADspawnParticles.h"
#include "THREADupdate.h"
#include "setConfig.h"
#include "readConifg.h"

int main(void)
{
	const float PI = 3.1415926535;
	configuration config;

	config.height = 0;
	config.width = 0;

	while (true)
	{
		std::cout << "\nWould you like to use an existing (E) configuration, or create a new (N) configuration? (E/N):\n";
		std::string line;
		std::getline(std::cin, line);
		

		if (line[0] == 'N' || line[0] == 'n')
		{
			setConfig(config);
			break;
		}
		if (line[0] == 'E' || line[0] == 'e')
		{
			while (true)
			{
				std::cout << "\nEnter filepath to settings:\n";
				std::string filepath;
				std::getline(std::cin, filepath);

				std::fstream file;
				file.open(filepath);

				if (file.is_open())
				{
					file.close();
					readConifg(config, filepath);
					goto EXIT;
				}
				std::cout << "\nInvalid filepath!\n";
				
			}
		}
		std::cout << "\nPlease provide a valid input!\n";
	}
EXIT:

	//normalize color values from 0.0 - 1.0
	for (int i = 0; i < 3; i++)
		config.particleColor[i] /= 255;

	config.particleColor[3] /= 100;

	zoom = config.defaultZoom;

	//allocate memory for particles
	particle* parts = new particle[config.quantity];
	if (!parts)
	{
		std::cout << "OUT OF MEMORY" << std::endl;
		return 1;
	}

	//generate the location and velocity for all particles
	std::thread* generateThreads = new std::thread[config.multiThreadCount];
	unsigned long numberSpawned = 0;
	if (!config.threeDimensions)
	{
		for (int i = 0; i < config.multiThreadCount; i++)
		{
			generateThreads[i] = std::thread(THREADSpawnParticles2D, parts, i, &numberSpawned, &config);
		}
	}
	else
	{
		for (int i = 0; i < config.multiThreadCount; i++)
		{ 
			generateThreads[i] = std::thread(THREADSpawnParticles3D, parts, i, &numberSpawned, &config);
		}
	}

	//log the particle generation progress to the consol
	std::thread logProgressThread(THREADloadingProgressLog, &numberSpawned, config.quantity);

	for (int i = 0; i < config.multiThreadCount; i++) {
		generateThreads[i].join();
	}

	logProgressThread.join();
	
	//A bunch of boiler plate openGL code
	if (!glfwInit())
	{
		return -1;
	}

	float currentFPS = 1;

	GLFWwindow* window = glfwCreateWindow(config.width, config.height, "Window", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		return -1;
	}

	//vertex data just hold the position value of each particle, used only for graphics not calculations
	float* vertexData = nullptr;
	if (!config.threeDimensions)
	{
		vertexData = new float[2 * config.quantity];
		if (!vertexData)
		{
			std::cout << "OUT OF MEMORY" << std::endl;
			return 1;
		}
		for (int i = 0; i < config.quantity; i++)
		{
			vertexData[i * 2] = parts[i].position.x / config.width;
			vertexData[i * 2 + 1] = parts[i].position.y / config.height;
		}
	}
	else
	{

		vertexData = new float[3 * config.quantity];
		if (!vertexData)
		{
			std::cout << "OUT OF MEMORY" << std::endl;
			return 1;
		}
		for (int i = 0; i < config.quantity; i++)
		{
			vertexData[i * 3] = parts[i].position.x;
			vertexData[i * 3 + 1] = parts[i].position.y;
			vertexData[i * 3 + 2] = parts[i].position.z;
		}
	}

	//create space for vertex data in the GPU
	CHECKERROR(170,
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	if (!config.threeDimensions)
	{
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * config.quantity, vertexData, GL_DYNAMIC_DRAW);
	}
	else
	{
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * config.quantity, vertexData, GL_DYNAMIC_DRAW);
	}

	glEnableVertexAttribArray(0);

	if (!config.threeDimensions)
	{
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
	}
	else
	{
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	}
	)

	//process shaders
	std::string filepath = "shaders.shader";
	shaderPair shaders = parseShader(filepath, config.threeDimensions);
	unsigned int program = createShader(shaders.vertex, shaders.fragment);

	CHECKERROR(196, glUseProgram(program););

	unsigned int frameCount = 0;

	//uniform for telling the vertex shader what color to use
	CHECKERROR(203,
	unsigned int colorUniformLocation = glGetUniformLocation(program, "u_Color");
	glUniform4fv(colorUniformLocation, 1, config.particleColor);
	);

	float offset[2] = { 0, 0 };
	vec2<float> lookOrientation;
	int m1State = GLFW_RELEASE;
	vec2<double> mousePositionBefore(NULL, NULL);
	vec2<double> mousePositionAfter(NULL, NULL);

	while (!glfwWindowShouldClose(window))
	{
		timer programClock("Program");

		std::chrono::steady_clock::time_point startOfFrame = std::chrono::high_resolution_clock::now();

		//updates particle position based on their velocity and copys the position data from the particles to the vertex data array
		std::thread* threads3 = new std::thread[config.multiThreadCount];

			for (int i = 0; i < config.multiThreadCount; i++)
				threads3[i] = std::thread(THREADUpdate, parts, vertexData, i, currentFPS, &config);

		for (int i = 0; i < config.multiThreadCount; i++)
			threads3[i].join();

		//copy vertex data from CPU memory to GPU memory
		CHECKERROR(227,
		if (!config.threeDimensions)
		{
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 2 * config.quantity, vertexData);
		}
		else
		{
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 3 * config.quantity, vertexData);
		}
		);

		delete[] threads3;

		CHECKERROR(239,
			unsigned int zoomUniformLocation = glGetUniformLocation(program, "u_Zoom");
		glUniform1f(zoomUniformLocation, zoom);
		);
		//2d uniforms
		if (!config.threeDimensions)
		{

			float n_offset[2] = { offset[0] / config.width, offset[1] / config.height };
			CHECKERROR(247, unsigned int offsetUniformLocation = glGetUniformLocation(program, "u_Offset"););
			CHECKERROR(248, glUniform2fv(offsetUniformLocation, 1, &(n_offset[0])););
		}
		//3d uniforms
		else
		{
			CHECKERROR(252, unsigned int heightUniformLocation = glGetUniformLocation(program, "u_Height"););
			CHECKERROR(253, glUniform1f(heightUniformLocation, config.height););
			CHECKERROR(254, unsigned int widthtUniformLocation = glGetUniformLocation(program, "u_Width"););
			CHECKERROR(255, glUniform1f(widthtUniformLocation, config.width););

			//create rotation matrixs used to create the orbiting affect when you move your mouse
			matrix3x3<float> xAxisRotationMatrix(
				1, 0, 0,
				0, cos(lookOrientation.x), -sin(lookOrientation.x),
				0, sin(lookOrientation.x), cos(lookOrientation.x)
			);
			matrix3x3<float> yAxisRotationMatrix(
				cos(lookOrientation.y), 0, sin(lookOrientation.y),
				0, 1, 0,
				-sin(lookOrientation.y), 0, cos(lookOrientation.y)
			);

			matrix3x3<float> rotationMatrix3 = xAxisRotationMatrix * yAxisRotationMatrix;
			float rotationMatrix4[4][4] = { {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,1} };

			for (int i = 0; i < 3; i++) {

				for (int j = 0; j < 3; j++) {

					rotationMatrix4[i][j] = rotationMatrix3.data[i][j];
				}
			}

			CHECKERROR(279, unsigned int rotationMatUniformLocation = glGetUniformLocation(program, "u_RotationMat"););
			CHECKERROR(280, glUniformMatrix4fv(rotationMatUniformLocation, 1, GL_TRUE, &(rotationMatrix4[0][0])););
		}

		//clear the back buffer
		CHECKERROR(258,
			glClear(GL_COLOR_BUFFER_BIT);

		//enable transparency and draw all vertexes to the back buffer
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDrawArrays(GL_POINTS, 0, config.quantity);
		glDisable(GL_BLEND);

		//swap back buffer and front buffer
		glfwSwapBuffers(window);
		);
		
		quadNode* root2D = new quadNode(0);
		octNode* root3D = new octNode(0);
		
		if (!config.threeDimensions)
		{
			delete root3D;
		}
		else
		{
			delete root2D;
		}

		//create a box encompases ALL particles
		std::thread* boundingBoxThreads = new std::thread[config.multiThreadCount];

		for (int i = 0; i < config.multiThreadCount; i++)
		{
			if (!config.threeDimensions)
			{
				boundingBoxThreads[i] = std::thread(THREADCreateRootNodeBoundingBox2D, parts, i, &root2D->bounds, config.quantity, config.multiThreadCount);
			}
			else
			{
				boundingBoxThreads[i] = std::thread(THREADCreateRootNodeBoundingBox3D, parts, i, &root3D->bounds, config.quantity, config.multiThreadCount);
			}
		}
		for (int i = 0; i < config.multiThreadCount; i++)
		{
			boundingBoxThreads[i].join();
		}
		
		//subdivide the bigger box into lots of smaller boxes that contain only one particles each
		timer* insertClock = new timer("insert");
		std::thread* insertThreads = new std::thread[config.multiThreadCount];
		
		for (int i = 0; i < config.multiThreadCount; i++)
		{
			if (!config.threeDimensions)
			{
				insertThreads[i] = std::thread(THREADInsert2D, root2D, parts, i, config.quantity, config.multiThreadCount, config.maxTreeDepth);
			}
			else
			{
				insertThreads[i] = std::thread(THREADInsert3D, root3D, parts, i, config.quantity, config.multiThreadCount, config.maxTreeDepth);
			}
		}
		for (int i = 0; i < config.multiThreadCount; i++)
		{
			insertThreads[i].join();
		}

		delete[] insertThreads;
		delete insertClock;

		//calculate gravity efficiently using those boxes of particles
		timer* gravityClock = new timer("Gravity");
		int length = 0;

		std::thread* threads2 = new std::thread[config.multiThreadCount];

		for (int i = 0; i < config.multiThreadCount; i++)
		{
			if (!config.threeDimensions)
			{
				threads2[i] = std::thread(THREADGravity2D, root2D, parts, i, config.quantity, config.multiThreadCount, config.thresholdTheta, config.softening, config.G);			}
			else
			{
				threads2[i] = std::thread(THREADGravity3D, root3D, parts, i, config.quantity, config.multiThreadCount, config.thresholdTheta, config.softening, config.G);
			}
		}
		for (int i = 0; i < config.multiThreadCount; i++)
		{
			threads2[i].join();
		}
		delete[] threads2;
		delete gravityClock;

		//collect and process user input
		glfwPollEvents();

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {

			glfwGetCursorPos(window, &mousePositionAfter.x, &mousePositionAfter.y);
			if (mousePositionBefore.x != NULL && mousePositionBefore.y != NULL) {
				if (!config.threeDimensions)
				{
					offset[0] += mousePositionAfter.x - mousePositionBefore.x;
					offset[1] -= mousePositionAfter.y - mousePositionBefore.y;
				}
				else
				{
					lookOrientation.y -= ((mousePositionAfter.x - mousePositionBefore.x) * PI / 180) * config.orbitSensitivity;
					lookOrientation.x -= ((mousePositionAfter.y - mousePositionBefore.y) * PI / 180) * config.orbitSensitivity;

					if (lookOrientation.x > 85 * PI / 180)
					{
						lookOrientation.x = 85 * PI / 180;
					}
					if (lookOrientation.x < -85 * PI / 180)
					{
						lookOrientation.x = -85 * PI / 180;
					}
				}
			}
			mousePositionBefore.x = mousePositionAfter.x;
			mousePositionBefore.y = mousePositionAfter.y;
		}

		else 
		{
			mousePositionBefore.x = NULL;
			mousePositionBefore.y = NULL;
		}

		glfwSetScrollCallback(window, scroll_callback);


		currentFPS = 1 / ((std::chrono::high_resolution_clock::now() - startOfFrame).count() / 1000000000.0f);

		if (frameCount % 3 == 0 || frameCount == 0)
			glfwSetWindowTitle(window, ("FPS: " + std::to_string(currentFPS)).c_str());


		//free all memory used for quadtree/octtree
		timer dissassembleClock("Dissassemble");
		if (!config.threeDimensions)
		{
			root2D->disassemble();
		}
		else
		{
			root3D->disassemble();
		}
		
		if (!config.threeDimensions)
		{
			delete root2D;
		}
		else
		{
			delete root3D;
		}

		frameCount++;
	}

	std::cout << "\n" << frameCount << " frames generated!\n";

	delete[] parts;
	delete[] vertexData;
	glDeleteProgram(program);
	glDeleteBuffers(1, &buffer);
	glfwTerminate();
	return 0;
}