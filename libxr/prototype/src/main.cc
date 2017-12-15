#include <GLFW/glfw3.h>
#include <Windows.h>

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

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0, 0, 0.3, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}