#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"


struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filePath)
{
    std::ifstream stream(filePath);
    enum class ShaderType
    {
        None = -1, Vertex = 0, Fragment = 1
    };
    std::string line;
    std::stringstream ss[2];

    ShaderType type = ShaderType::None;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::Vertex;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::Fragment;
            }
        }
            else
            {
                ss[(int)type] << line << "\n";
            }
    }

    return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader( unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result != GL_TRUE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << message << std::endl;
        std::cout << "Failed to compile" << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int ps = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, ps);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(ps);

    return program;
}

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
    float positions[12] = {
        -0.5f, -0.5f,
        0.5f, -0.5f,
        0.5f, 0.5f,
        -0.5f,0.5f,
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

    VertexBuffer vb(positions, 4 * 2 * sizeof(float));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8, 0);

    ShaderProgramSource source = ParseShader("res/shaders/Basic.glsl");
    std::cout << source.VertexSource;
    std::cout << source.FragmentSource;

    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    glUseProgram(shader);
    unsigned int uColorLoc = glGetUniformLocation(shader, "u_Color");

    float r = 0.0f;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        r += .05f;
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        glUniform4f(uColorLoc, r, 0.5f, 1.0f, 1.0f);
        glBindVertexArray(vao);
        ib.Bind();
        GLCall(glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,nullptr));
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        if (r >= 1)
            r = 0;

        /* Poll for and process events */
        glfwPollEvents();
    }
    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}