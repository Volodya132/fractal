#include <iostream>
#include <stdlib.h>
#include <string>
#include <math.h>
#include <algorithm>
#include <vector>

#include <glad/glad.h>
#include <GLFW\glfw3.h>
#include <GLM\glm.hpp>
#include <GLM\gtc\matrix_transform.hpp>
#include <GLM\gtc\type_ptr.hpp>
#include <glm\vec2.hpp>

#include "Shader.h"

glm::vec2 Resolution{ 800, 800 };

int num_frames{};
float last_time{};
glm::vec2 center{ 0, 0 };
float zoom{ 1.0 };

float vertices[] =
{
    //    x      y      z   
        -1.0f, -1.0f, -0.0f,
         1.0f,  1.0f, -0.0f,
        -1.0f,  1.0f, -0.0f,
         1.0f, -1.0f, -0.0f
};

unsigned int indices[] =
{
    //  2---,1
    //  | .' |
    //  0'---3
        0, 1, 2,
        0, 3, 1
};


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void process_input(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
    {
        center.y = center.y + 0.01f * zoom;
        if (center.y > 1.0f)
        {
            center.y = 1.0f;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    {
        center.y = center.y - 0.01f * zoom;
        if (center.y < -1.0f)
        {
            center.y = -1.0f;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    {
        center.x = center.x - 0.01f * zoom;
        if (center.x < -1.0f)
        {
            center.x = -1.0f;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    {
        center.x = center.x + 0.01f * zoom;
        if (center.x > 1.0f)
        {
            center.x = 1.0f;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        zoom = zoom * 1.1f;
        if (zoom > 1.0f)
        {
            zoom = 1.0f;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        zoom = zoom * 0.90f;
        if (zoom < 0.00000001f)
        {
            zoom = 0.00000001f;
        }
    }
}

void countFPS()
{
    double current_time = glfwGetTime();
    num_frames++;
    if (current_time - last_time >= 1.0)
    {
        std::cout << num_frames  << " frame / ms\n";
        num_frames = 0;
        last_time += 1.0;
    }
}



int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(Resolution.x, Resolution.y, "Mandelbrot", NULL, NULL);

    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window!\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGL())
    {
        std::cout << "Failed initializing GLEW\n";
    }

    glViewport(0, 0, Resolution.x, Resolution.y);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    Shader our_shader("res/Shader/shader.vert", "res/Shader/shader.frag");

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    last_time = glfwGetTime();

    glEnable(GL_DEPTH_TEST);


    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.0f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        process_input(window);
        countFPS();

        our_shader.use_shader();
        our_shader.set_float("zoom", zoom);
        our_shader.set_vec2("center", center);
        our_shader.set_vec2("Resolution", Resolution);

        glBindVertexArray(VAO);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}