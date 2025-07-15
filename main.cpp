#include <glad/glad.h>
#include <string>
#include <iostream>
#include <GLFW/glfw3.h>
#include "application/Game.h"
#include "loader/Loader.h"

// 回调函数声明
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//定义窗口大小
const GLuint SCREEN_WIDTH = 800;
const GLuint SCREEN_HEIGHT = 600;

Game Breakout(SCREEN_WIDTH, SCREEN_HEIGHT);

int main(int argc, char* argv[])
{
    //初始化
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Breakout", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    glGetError(); 

    glfwSetKeyCallback(window, key_callback);
    // 设置窗口大小改变时的回调函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 配置OpenGL
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //游戏初始化
    Breakout.Init();
    Breakout.State = GameState::GAME_MENU;

    GLfloat deltaTime = 0.0f;
    GLfloat lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window))
    {
        //计算帧时间差
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();


        //输入响应
        Breakout.ProcessInput(deltaTime);

        //游戏更新
        Breakout.Update(deltaTime);

        // 渲染
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        Breakout.Render();

        glfwSwapBuffers(window);
    }

    //释放内存
    ResourceManager::Clear();

    glfwTerminate();
    return 0;
}


//键盘响应回调函数
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            Breakout.Keys[key] = GL_TRUE;
        else if (action == GLFW_RELEASE)
            Breakout.Keys[key] = GL_FALSE;
    }
}

// 回调函数定义
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);  // 更新 OpenGL 视口

}