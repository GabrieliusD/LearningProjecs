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
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Bounds.h"

glm::vec3 RayNDS;

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double xpos, ypos;
        //getting cursor position
        glfwGetCursorPos(window, &xpos, &ypos);
        std::cout << "Cursor Position at (" << xpos << " : " << ypos << std::endl;
        
        float x = (2.0f * xpos) / 640.0f - 1.0f;
        float y = 1.0f - (2.0f * ypos) / 480.0f;
        float z = 1.0f;

        glm::vec3 ray_nds(x, y, z);
    
        RayNDS = ray_nds;
    }
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
    glfwSwapInterval(0);
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Not OK";
    }

    glfwSetMouseButtonCallback(window, mouse_button_callback);
    //init imgui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui::StyleColorsDark();
    ImGui_ImplOpenGL3_Init((char*)glGetString(330));

    bool show_demo_window = false;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.55f, 1.0f);




    std::cout << glGetString(GL_VERSION) << std::endl;
    float positions[20] = {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,
        0.5f, -0.5f, 1.0f, 0.0f, 0.5f,
        0.5f, 0.5f, 1.0f, 1.0f, 0.5f,
        -0.5f,0.5f, 0.0f, 1.0f, 0.5f
    };

    float position2[20] = {
    -0.4f, -0.4f, 0.0f, 0.0f, 0.5f,
    0.4f, -0.4f, 1.0f, 0.0f, 0.5f,
    0.4f, 0.4f, 1.0f, 1.0f, 0.5f,
    -0.4f,0.4f, 0.0f, 1.0f, 1.0f
    };
    unsigned int indices[6] =
    {
        0,1,2,
        2,3,0
    };
    glm::vec3 Vert[4] = { 
        glm::vec3(-0.5f, -0.5f,1),
        glm::vec3(0.5f, -0.5f,0),
        glm::vec3(0.5f, 0.5f,0),
        glm::vec3(-0.5f, 0.5f,0) };
    std::vector<glm::vec3> verts(Vert, Vert + sizeof(Vert)/sizeof(Vert[0]));
    Bounds::GetBounds(verts);



    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //unsigned int vao;
    //glGenVertexArrays(1, &vao);
    //glBindVertexArray(vao);

    IndexBuffer ib(indices, 6);

    glm::mat4 proj = glm::ortho(0.0f, 640.0f, 0.0f, 480.0f, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
    

    VertexArray va;
    VertexArray va2;
    VertexBuffer vb(positions, 4 * 5 * sizeof(float));
    VertexBuffer vb2(position2, 4 * 5 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    layout.Push<float>(1);
    va.AddBuffer(vb, layout);
    va2.AddBuffer(vb2, layout);


    Shader shader("res/shaders/Basic.shader");
    shader.Bind();

    shader.SetUniform4f("u_Color", 1, 1, 0, 1);


    Texture texture("res/textures/icon.png");
    Texture texture2("res/textures/blue.png");
    texture.Bind();

    shader.SetUniform1i("u_Texture", 0);
    shader.Unbind();
    vb.Unbind();
    ib.Unbind();

    Renderer renderer;
    glm::vec3 translation(0, 0, 1);
    glm::vec3 translation2(500, 200, 1);
    float r = 0.0f;
    float g = 1.0f;
    float b = 0.0f;
    glm::vec4 ray_clip(RayNDS.x, RayNDS.y, -1.0f, 1.0f);
    glm::vec4 ray_eye = glm::inverse(proj) * ray_clip;
    ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0);
    glm::vec3 ray_wor = (glm::inverse(view) * ray_eye);
    ray_wor = glm::normalize(ray_wor);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        renderer.Clear();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        glm::mat4 world = glm::translate(glm::mat4(1.0f), translation) * glm::scale(glm::mat4(1.0f), glm::vec3(100, 100, 1));
        glm::vec4 ray_clip(RayNDS.x, RayNDS.y, -1.0f, 1.0f);
        glm::vec4 ray_eye = glm::inverse(proj) * ray_clip;
        ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0);
        glm::vec3 ray_wor = (glm::inverse(view) * ray_eye);
        ray_wor = glm::normalize(ray_wor);
        glm::vec4 rayModelSpace = (glm::inverse(world) * glm::vec4(ray_wor.x, ray_wor.y, ray_wor.z,1));
        //glm::mat4 world = glm::translate(glm::mat4(1.0f), glm::vec3(ray_eye)) * glm::scale(glm::mat4(1.0f), glm::vec3(100, 100, 1));
        
        glm::mat4 mvp = proj * view * world;
        shader.Bind();
        shader.SetUniform4f("u_Color", r, g, b, 1);
        texture.Bind();
        shader.SetUniformMat4f("u_MVP", mvp);
        renderer.Draw(va, ib, shader);
        world = glm::translate(glm::mat4(1.0f), translation2) * glm::scale(glm::mat4(1.0f), glm::vec3(100, 100, 1));
        mvp = proj * view * world;
        shader.SetUniformMat4f("u_MVP", mvp);
        renderer.Draw(va, ib, shader);

        /* Swap front and back buffers */

        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);


        ImGui::SliderFloat3("float", &translation.x, 0, 600);
        ImGui::SliderFloat3("float 2", &translation2.x, 0, 600);
        

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        glfwSwapBuffers(window);
        if (r >= 1)
            r = 0;



        /* Poll for and process events */
        glfwPollEvents();


    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}