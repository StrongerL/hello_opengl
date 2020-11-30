#pragma once
#include <random>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "HalfEdgeDataStructure.h"
#include "Shader.h"
#include "Camera.h"

using namespace std;

class Draw {
private:
	// settings
	const unsigned int SCR_WIDTH = 800;
	const unsigned int SCR_HEIGHT = 600;

	// camera
	Camera camera;
	float lastX = SCR_WIDTH / 2.0f;
	float lastY = SCR_HEIGHT / 2.0f;
	bool firstMouse = true;

	// timing
	float deltaTime = 0.0f;	// time between current frame and last frame
	float lastFrame = 0.0f;
	
	// 生成需要绘制的顶点位置属性和颜色属性
	// 必须将多边形坐标分为多个三角形，然后才能绘制
	vector<float> build_vertex_array(Solid* solid) {
		vector<float> vertices;
		vector<Face*> faces = solid->sfaces;
		default_random_engine engine;
		uniform_real_distribution<float> float_rand(0.0, 1.0);

		for (int i = 0; i < faces.size(); i++) {
			// 同一个面的点生成同一种rgb颜色
			float r = float_rand(engine);
			float g = float_rand(engine);
			float b = float_rand(engine);

			vector<Loop*> loops = faces[i]->floops;

			// 如果一个面有多个环，就不绘制这个面
			// 因为暂时我还不会怎么把带孔的多边形分为多个三角形
			if (loops.size() > 1)
				continue;

			for (int j = 0; j < loops.size(); j++) {
				
				// 同一个环上的点生成同一种颜色
				r = float_rand(engine);
				g = float_rand(engine);
				b = float_rand(engine);

				Loop* loop = loops[j];
				HalfEdge* he = loop->ledge;
				HalfEdge* src_edge = he;

				vector<float> tmp;

				tmp.push_back((float)he->startv->x);
				tmp.push_back((float)he->startv->y);
				tmp.push_back((float)he->startv->z);
				tmp.push_back(r);
				tmp.push_back(g);
				tmp.push_back(b);
				he = he->nxt;
				while (he != src_edge) {
					tmp.push_back((float)he->startv->x);
					tmp.push_back((float)he->startv->y);
					tmp.push_back((float)he->startv->z);
					tmp.push_back(r);
					tmp.push_back(g);
					tmp.push_back(b);
					he = he->nxt;
				}

				for (int k = 0; k < tmp.size() / 6 - 2; k++) {
					for (int t = 0; t < 6; t++) {
						vertices.push_back(tmp[t]);
					}
					for (int t = (k + 1) * 6; t < (k + 1) * 6 + 12; t++) {
						vertices.push_back(tmp[t]);
					}
				}
			}
		}
		// 规范化，将点的坐标限制在[-1,1]范围内
		float max_val = *max_element(vertices.begin(), vertices.end());
		float min_val = *min_element(vertices.begin(), vertices.end());
		float max_abs = max_val + min_val > 0 ? max_val : -min_val;
		for (int i = 0; i < vertices.size(); i++) {
			vertices[i] /= max_abs;
			if ((i + 1) % 3 == 0)
				i += 3;
		}

		return vertices;
	}

public:
	Draw() {
		camera = *(new Camera(glm::vec3(0.0f, 0.0f, 3.0f)));
	}

	// 按照面和环的顺序，输出实体顶点的坐标
	// 输出顺序也是环的走向
	static void print_coordinate(Solid* solid) {
		vector<Face*> faces = solid->sfaces;
		for (int i = 0; i < faces.size(); i++) {
			cout << "face[" << i << "]" << endl;
			vector<Loop*> loops = faces[i]->floops;
			for (int j = 0; j < loops.size(); j++) {
				cout << "face[" << i << "] -> " << "loop[" << j << "]" << endl;
				Loop* loop = loops[j];
				HalfEdge* he = loop->ledge;
				HalfEdge* src_edge = he;
				cout << "[" << he->startv->x << ", " << he->startv->y << ", " << he->startv->z << "]  ";
				he = he->nxt;
				while (he != src_edge) {
					cout << "[" << he->startv->x << ", " << he->startv->y << ", " << he->startv->z << "]  ";
					he = he->nxt;
				}
				cout << endl;
			}
		}
	}

	void processInput(GLFWwindow* window)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.ProcessKeyboard(FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.ProcessKeyboard(BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera.ProcessKeyboard(RIGHT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera.ProcessKeyboard(LEFT, deltaTime);
	}

	
	void draw(Solid* solid) {
			// glfw: initialize and configure
		// ------------------------------
			glfwInit();
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	#ifdef __APPLE__
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif

			// glfw window creation
			// --------------------
			GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
			if (window == NULL)
			{
				std::cout << "Failed to create GLFW window" << std::endl;
				glfwTerminate();
				return;
			}
			glfwMakeContextCurrent(window);

			// tell GLFW to capture our mouse
			//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			// glad: load all OpenGL function pointers
			// ---------------------------------------
			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			{
				std::cout << "Failed to initialize GLAD" << std::endl;
			}

			// configure global opengl state
			// -----------------------------
			glEnable(GL_DEPTH_TEST);

			// build and compile our shader zprogram
			// ------------------------------------
			Shader ourShader("shader.vs", "shader.fs");


			vector<float> v = build_vertex_array(solid);
			float vertices[1000];
			int vertices_count = v.size();
			for (int i = 0; i < vertices_count; i++) {
				vertices[i] = v[i];
				//cout << v[i] << " ";
				//if ((i + 1) % 6 == 0)
				//	cout << endl;
			}

			unsigned int VBO, VAO;
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);

			glBindVertexArray(VAO);

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(float), vertices, GL_STATIC_DRAW);

			// position attribute
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			// color attribute
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);


			// render loop
			// -----------
			while (!glfwWindowShouldClose(window))
			{
				// per-frame time logic
				// --------------------
				float currentFrame = glfwGetTime();
				deltaTime = currentFrame - lastFrame;
				lastFrame = currentFrame;

				// input
				// -----
				processInput(window);

				// render
				// ------
				//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
				glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				// activate shader
				ourShader.use();

				// pass projection matrix to shader (note that in this case it could change every frame)
				glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
				ourShader.setMat4("projection", projection);

				// camera/view transformation
				glm::mat4 view = camera.GetViewMatrix();
				ourShader.setMat4("view", view);

				// render boxes
				glBindVertexArray(VAO);

				// calculate the model matrix for each object and pass it to shader before drawing
				glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
				model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
				float angle = 0.0f;
				model = glm::rotate(model, glm::radians(angle) + (float)glfwGetTime(), glm::vec3(1.0f, 0.3f, 0.5f));
				ourShader.setMat4("model", model);

				glDrawArrays(GL_TRIANGLES, 0, v.size() / 6);



				// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
				// -------------------------------------------------------------------------------
				glfwSwapBuffers(window);
				glfwPollEvents();
			}

			// optional: de-allocate all resources once they've outlived their purpose:
			// ------------------------------------------------------------------------
			glDeleteVertexArrays(1, &VAO);
			glDeleteBuffers(1, &VBO);

			// glfw: terminate, clearing all previously allocated GLFW resources.
			// ------------------------------------------------------------------
			glfwTerminate();
	}

	
};
