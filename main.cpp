#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "shader.hpp"
#include "trackball.hpp"
#include "error.hpp"

// window-related variables
int width = 1024;
int height = 768;

// projection matrix
glm::mat4 projection;

// geometry-related variables
GLuint programId;
GLuint vertexBuffer;
GLuint indexBuffer;
GLenum positionBufferId;

const GLfloat vertexPositionData[] = {
   -1.0f, -1.0f,
    1.0f, -1.0f,
   -1.0f,  1.0f,
    1.0f,  1.0f
};
const GLushort indexData[] = { 0, 1, 2, 3 };

void createGeometry(void) {
   glGenBuffers(1, &vertexBuffer);
   glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositionData), vertexPositionData, GL_STATIC_DRAW);

   glGenBuffers(1, &indexBuffer);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData, GL_STATIC_DRAW);
}

void update(void) {
   int milliseconds = (int)(glfwGetTime() * 1000);

   // TODO: Some update

}

void render() {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // activate our shader program
	glUseProgram(programId);

   // turn on depth buffering
   glEnable(GL_DEPTH_TEST);

   // view matrix - orient everything around our preferred view
   glm::mat4 view = glm::lookAt(
      eyePosition,
      glm::vec3(0,0,0),    // where to look
      glm::vec3(0,1,0)     // up
   );

   glm::mat4 model = glm::mat4(1.0f);
   model = glm::mat4_cast(rotation);
   model = glm::scale(model, glm::vec3(scaleFactor, scaleFactor, scaleFactor));

   // model-view-projection matrix
   glm::mat4 mvp = projection * view * model;
   GLuint mvpMatrixId = glGetUniformLocation(programId, "u_MVPMatrix");
   glUniformMatrix4fv(mvpMatrixId, 1, GL_FALSE, &mvp[0][0]);

   // enable the vertex buffer
   glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

   // configure the attribute array (the layout of the vertex buffer)
   glVertexAttribPointer(positionBufferId, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, (void *)0);
   glEnableVertexAttribArray(positionBufferId);

   // draw the triangle strip
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
   glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, (void*)0);

   // disable the attribute array
   glDisableVertexAttribArray(positionBufferId);
}

void updateProjectionMatrix(int width, int height) {
   // projection matrix - perspective projection
   // FOV:           45°
   // Aspect ratio:  4:3 ratio
   // Z range:       between 0.1 and 100.0
   float aspectRatio = (float)width / (float)height;
   projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 1000.0f);

   // projection matrix - orthographic (non-perspective) projection
   // Note:  These are in world coordinates
   // xMin:          -10
   // xMax:          +10
   // yMin:          -10
   // yMax:          +10
   // zMin:           0
   // zMax:          +100
   //glm::mat4 projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f);
}

void reshape(GLFWwindow *window, int w, int h) {
   width = w;
   height = h;

   glViewport(0, 0, width, height);

   updateProjectionMatrix(w, h);
}

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {
   if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, true);
   }
}

void onError(int error, const char* description) {
   std::cerr << "Error: " << description << std::endl;
}

int main(int argc, char** argv) {

   // set a function to receive GLFW errors
   glfwSetErrorCallback(onError);

   // initialize GLFW (windowing abstraction library)
   if (!glfwInit()) {
      // initialization failed
      std::cerr << "GLFW Error:  Unable to initialize GLFW" << std::endl;
      return -1;
   }

   // create a window and an OpenGL context
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
   glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
   GLFWwindow* window = glfwCreateWindow(width, height, "CSCI 3090U Base OpenGL Project", NULL, NULL);
   if (!window) {
      // window or OpenGL context creation failed
      std::cerr << "GLFW Error:  Unable to create window" << std::endl;
      return -1;
   }
   glfwMakeContextCurrent(window);

   // initialize GLEW (OpenGL extension loading library)
   glewInit();
   if (!GLEW_VERSION_2_0) {
      std::cerr << "OpenGL 2.0 not available" << std::endl;
      return 1;
   }
   std::cout << "GLEW Version:   " << glewGetString(GLEW_VERSION) << std::endl;
   std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
   std::cout << "GLSL Version:   " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
   std::cout << "GPU Vendor:     " << glGetString(GL_VENDOR) << std::endl;
   std::cout << "GPU Model:      " << glGetString(GL_RENDERER) << std::endl;

   // setup the error handling
   GLint flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
   if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
      glEnable(GL_DEBUG_OUTPUT);
      glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
      glDebugMessageCallback(openGlDebugCallback, nullptr);
      glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
   }

   glfwSetMouseButtonCallback(window, mouse);
   glfwSetCursorPosCallback(window, drag);
   glfwSetKeyCallback(window, keyboard);
   glfwSetFramebufferSizeCallback(window, reshape);
   updateProjectionMatrix(width, height);
   glfwSwapInterval(1);

   // determine the initial camera position
   eyePosition.x = 0.0f;
   eyePosition.y = 0.0f;
   eyePosition.z = cameraDistance;

   createGeometry();
   programId = createShaderProgram("shaders/vertex.glsl", "shaders/fragment.glsl");

   while (!glfwWindowShouldClose(window)) {
      // perform updates (e.g. physics)
      update();

      // re-draw
      render();

      glfwSwapBuffers(window);
      glfwPollEvents();
   }

   glfwDestroyWindow(window);
   glfwTerminate();

   return 0;
}
