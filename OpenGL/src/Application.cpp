#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

int main(void)
{
    GLFWwindow* window;
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Not OK";
    }

    std::cout << glGetString(GL_VERSION) << std::endl;
    float positions[16] = {
        -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f,0.5f, 0.0f, 1.0f
    };
    unsigned int indices[6] =
    {
        0,1,2,
        2,3,0
    };

    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    IndexBuffer ib(indices, 6);

    VertexArray va;
    VertexBuffer vb(positions, 4 * 4 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);
    


    Shader shader("res/shaders/Basic.glsl");
    shader.Bind();

    shader.SetUniform4f("u_Color", 1, 1, 0, 1);


    Texture texture("res/textures/icon.jpg");
    texture.Bind();
    //shader.SetUniform1i("u_Texture", 0);
    shader.Unbind();
    vb.Unbind();
    ib.Unbind();

    Renderer renderer;
    float r = 0.0f;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        renderer.Clear();
        shader.Bind();
        shader.SetUniform4f("u_Color", r, r, r, 1);
        r += .05f;
        renderer.Draw(va, ib, shader);
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        if (r >= 1)
            r = 0;

        /* Poll for and process events */
        glfwPollEvents();
    }
    GLCall(glfwTerminate());
    return 0;
}