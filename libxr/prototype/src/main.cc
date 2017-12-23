#include <cstdio>

#include <iostream>
#include <fstream>
#include <string>

#include <vector>

#include <Windows.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
    if (!glfwInit())
        return -1;

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow * window = glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

	glfwMakeContextCurrent(window);

	glewExperimental = true;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		char buf[512];
		sprintf_s(buf, "Error: %s\n", glewGetErrorString(err));
		OutputDebugStringA(buf);
		return -1;
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	GLuint programID;

	{
		const char* vertexShaderPath = "resources/passthrough.vert";
		const char* fragmentShaderPath = "resources/passthrough.frag";

		GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		
		std::string vertexShaderCode;
		std::ifstream vertexShaderStream(vertexShaderPath, std::ios::in);
		if (vertexShaderStream.is_open()) {
			std::string line = "";
			while (std::getline(vertexShaderStream, line))
				vertexShaderCode += "\n" + line;
			vertexShaderStream.close();
		}
		else {
			printf("Impossible to open %s. Are you in the right directory?\n", vertexShaderPath);
			programID = 0;
		}

		std::string fragmentShaderCode;
		std::ifstream fragmentShaderStream(fragmentShaderPath, std::ios::in);
		if (fragmentShaderStream.is_open()) {
			std::string line = "";
			while (std::getline(fragmentShaderStream, line))
				fragmentShaderCode += "\n" + line;
			fragmentShaderStream.close();
		}
		else {
			printf("Impossible to open %s. Are you in the right directory?\n", fragmentShaderPath);
			programID = 0;
		}

		GLint result = GL_FALSE;
		int infoLogLength;

		const char* vertexSourcePointer = vertexShaderCode.c_str();
		glShaderSource(vertexShaderID, 1, &vertexSourcePointer, nullptr);
		glCompileShader(vertexShaderID);

		glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
		glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0) {
			std::vector<char> vertexShaderErrorMessage(infoLogLength + 1);
			glGetShaderInfoLog(vertexShaderID, infoLogLength, nullptr, &vertexShaderErrorMessage[0]);
			printf("%s\n", &vertexShaderErrorMessage[0]);
		}

		const char* fragmentSourcePointer = fragmentShaderCode.c_str();
		glShaderSource(fragmentShaderID, 1, &fragmentSourcePointer, nullptr);
		glCompileShader(fragmentShaderID);

		glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
		glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0) {
			std::vector<char> fragmentShaderErrorMessage(infoLogLength + 1);
			glGetShaderInfoLog(fragmentShaderID, infoLogLength, nullptr, &fragmentShaderErrorMessage[0]);
			printf("%s\n", &fragmentShaderErrorMessage[0]);
		}

		programID = glCreateProgram();
		glAttachShader(programID, vertexShaderID);
		glAttachShader(programID, fragmentShaderID);
		glLinkProgram(programID);

		glGetProgramiv(programID, GL_LINK_STATUS, &result);
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0) {
			std::vector<char> programErrorMessage(infoLogLength + 1);
			glGetProgramInfoLog(programID, infoLogLength, nullptr, &programErrorMessage[0]);
			printf("%s\n", &programErrorMessage[0]);
		}

		glDetachShader(programID, vertexShaderID);
		glDetachShader(programID, fragmentShaderID);

		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
	}

	GLuint vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	static const GLfloat g_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 0.0f,  1.0f, 0.0f,
	};

	GLuint vertexBufferObject;
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	glUseProgram(programID);

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glDisableVertexAttribArray(0);

	glUseProgram(0);

	glBindVertexArray(0);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0, 0, 0.3f, 0);
        glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(programID);

		glBindVertexArray(vertexArrayObject);

		glEnableVertexAttribArray(0);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glDisableVertexAttribArray(0);

		glBindVertexArray(0);

		glUseProgram(0);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

	glDeleteBuffers(1, &vertexBufferObject);
	glDeleteVertexArrays(1, &vertexArrayObject);

	glDeleteProgram(programID);

    glfwTerminate();

    return 0;
}