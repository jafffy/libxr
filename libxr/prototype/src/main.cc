#include <cstdio>

#include <Windows.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
    if (!glfwInit())
        return -1;

    GLFWwindow * window = glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

	glfwMakeContextCurrent(window);

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		char buf[512];
		sprintf_s(buf, "Error: %s\n", glewGetErrorString(err));
		OutputDebugStringA(buf);
		return -1;
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0, 0, 0.3f, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}