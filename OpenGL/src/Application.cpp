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
#include "Camera.h"
#include "Model.h"
class Foo
{
public:
    static void  mouse_callback(GLFWwindow* window, double xpos, double ypos)
    {
        std::cout << "it works?";
    }
};

glm::vec3 RayNDS;
static bool mouseClick = false;

glm::vec3 cameraPos = glm::vec3(0, 0, 10);
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraForward = glm::vec3(0, 0, -1);
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float yaw = -90.0f;
float pitch = -3.f;
bool RightMouseClick;
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

        mouseClick = true;

    }
}


void processInput(GLFWwindow* window, Camera *camera)
{
    const float cameraSpeed = 10.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->MoveForward(cameraSpeed);
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->MoveForward(-cameraSpeed);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->Strafe(-cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->Strafe(cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera->MoveUp(cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera->MoveUp(-cameraSpeed);
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        RightMouseClick = true;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    }
    else
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        RightMouseClick = false;
    }
   
}
float lastX = 400;
float lastY = 300;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;
    if(RightMouseClick)
    {

        float sensitivity = 0.1f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        //std::cout << "Yaw: " << yaw << " Pitch: " << pitch << std::endl;
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
    glfwSwapInterval(1);
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Not OK";
    }

    glfwSetMouseButtonCallback(window, mouse_button_callback);
    Foo foo;

    glfwSetCursorPosCallback(window, mouse_callback);
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
    float positions[40] = {
       -0.5f,-0.5f, 0.5f, 0.0f, 0.0f, 
        0.5f,-0.5f, 0.5f, 1.0f, 0.0f,  
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,   
       -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,   
       -0.5f,-0.5f,-0.5f, 0.0f, 0.0f,
        0.5f,-0.5f,-0.5f, 1.0f, 0.0f,
        0.5f, 0.5f,-0.5f, 1.0f, 1.0f,
       -0.5f, 0.5f,-0.5f, 0.0f, 1.0f,
    };

    float position2[20] = {
    -0.4f, -0.4f, 0.0f, 0.0f,
    0.4f, -0.4f, 1.0f, 0.0f, 
    0.4f, 0.4f, 1.0f, 1.0f, 
    -0.4f,0.4f, 0.0f, 1.0f
    };
    unsigned int indices[36] =
    {
        //front
        0,3,2,
        0,2,1,
        //back
        4,7,6,
        4,6,5,
        //top
        3,7,6,
        3,6,2,
        //bottom
        0,4,5,
        0,5,1,
        //left
        0,4,3,
        4,7,3,
        //right
        1,2,5,
        5,2,6
    };

    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };


    glm::vec3 Vert[8] = { 
        glm::vec3(-0.5f,-0.5f, 0.5f),
        glm::vec3(0.5f,-0.5f, 0.5f),
        glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(-0.5f, 0.5f, 0.5f),
        glm::vec3(-0.5f,-0.5f,-0.5f),
        glm::vec3(0.5f,-0.5f,-0.5f),
        glm::vec3(0.5f, 0.5f,-0.5f),
        glm::vec3(-0.5f, 0.5f, -0.5f)
    };            
    std::vector<glm::vec3> verts(Vert, Vert + sizeof(Vert)/sizeof(Vert[0]));
    BoundingBox bBox = Bounds::GetBounds(verts);



    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    //unsigned int vao;
    //glGenVertexArrays(1, &vao);
    //glBindVertexArray(vao);

    IndexBuffer ib(indices, 36);
    Camera camera;
    camera.CreateProj(45.0f, 640.0f / 480.0f, 1.0f, 1000.0f);

    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
    glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
    glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

    glm::mat4 proj = camera.GetProj();//glm::perspective(45.0f, 640.0f / 480.0f, 1.0f, 1000.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -10));
    //view = glm::lookAt(cameraPos, cameraPos + cameraForward, up);
    VertexArray va;
    VertexArray va2;
    VertexArray vaLightBox;
    VertexBuffer vb(positions, 8 * 5 * sizeof(float));
    VertexBuffer vb2(position2, 4 * 5 * sizeof(float));
    VertexBuffer vbLightBox(vertices, 288 * sizeof(float));
    VertexBufferLayout layoutLightBox;
    layoutLightBox.Push<float>(3);
    layoutLightBox.Push<float>(3);
    layoutLightBox.Push<float>(2);
    VertexBufferLayout layout;
    layout.Push<float>(3);
    layout.Push<float>(2);
    //va.AddBuffer(vb, layout);
    //va2.AddBuffer(vb2, layout);
    //vaLightBox.AddBuffer(vbLightBox, layoutLightBox);

    std::string path = "res/models/backpack.obj";
    Model model(path);

    

    Shader shader("res/shaders/Basic.shader");
    shader.Bind();

    shader.SetUniform4f("u_Color", 1, 1, 0, 1);


    Texture texture("res/textures/icon.png");
    Texture texture2("res/textures/blue.png");
    Texture container("res/textures/1001_albedo.jpg");
    Texture container2("res/textures/container2_specular.png");
    texture.Bind();
    shader.SetUniform1i("u_Texture", 0);
    shader.Unbind();
    Shader LightShader("res/shaders/Light.shader");
    LightShader.Bind();
    //container.Bind();
    container2.Bind(1);
    //LightShader.SetUniform1i("material.diffuse", 0);
    LightShader.SetUniform1i("material.specular", 1);

    LightShader.SetUniform3f("material.ambient", 0.5f, 0.0f, 0.0f);
    //LightShader.SetUniform3f("material.diffuse", 1.0f, 0.8f, 0.0f);
    //LightShader.SetUniform1i("material.specular", 1);

    LightShader.SetUniform3f("dirLight.ambient", 0.2f, 0.2f, 0.2f);
    LightShader.SetUniform3f("dirLight.diffuse", 0.8f, 0.8f, 0.8f);
    LightShader.SetUniform3f("dirLight.specular", 1.0f, 1.0f, 1.0f);
    LightShader.SetUniform3f("dirLight.direction", 0.0f, -0.3f, 1.0f);

    LightShader.SetUniform3f("pointLights[0].position", 10.0f, 5.0f, 0.0f);
    LightShader.SetUniform3f("pointLights[0].ambient", 0.2f, 0.2f, 0.2f);
    LightShader.SetUniform3f("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
    LightShader.SetUniform3f("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    LightShader.SetUniform1f("pointLights[0].constant", 1.0f);
    LightShader.SetUniform1f("pointLights[0].linear", 0.02f);
    LightShader.SetUniform1f("pointLights[0].quadratic", 0.001f);

    vb.Unbind();
    ib.Unbind();
    glm::vec3 lightDir(0, -1.0, 0);
    glm::vec3 cubePos[5] = { glm::vec3(0,5,0),
    glm::vec3(0,-5,0),
    glm::vec3(10,0,0),
    glm::vec3(-10,0,0),
    glm::vec3(0,0,5), };

    Renderer renderer;
    glm::vec3 translation(0, 0, 0);
    glm::vec3 translation2(500, 200, 1);
    float r = 0.0f;
    float g = 1.0f;
    float b = 0.0f;
    glm::vec4 ray_clip(RayNDS.x, RayNDS.y, -1.0f, 1.0f);
    glm::vec4 ray_eye = glm::inverse(proj) * ray_clip;
    ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0);
    glm::vec3 ray_wor = (glm::inverse(view) * ray_eye);
    ray_wor = glm::normalize(ray_wor);

    glm::mat4 world = glm::translate(glm::mat4(1.0f), glm::vec3(0, -5, 0)) *glm::scale(glm::mat4(1.0f), glm::vec3(2, 2, 2));
    glm::mat4 world2 = glm::mat4(1.0f);
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        cameraDirection.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraDirection.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraDirection.y = sin(glm::radians(pitch));

        cameraForward = glm::normalize(cameraDirection);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        const float radius = 10.0f;
        float camX = sin(glfwGetTime()) * radius;
        float camZ = cos(glfwGetTime()) * radius;
        processInput(window, &camera);
        view = glm::lookAt(cameraPos, cameraPos + cameraForward, up);
        camera.RotateCamera(pitch, yaw);
        camera.Update();
        view = camera.GetView();
        //view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -10));
        renderer.Clear();
        //world = glm::rotate(world, 0.00001f, glm::vec3(0, 0, 1));
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        //world = glm::rotate(world, 5.0f * deltaTime, glm::vec3(1, 1, 0));
        glm::vec4 ray_clip(RayNDS.x, RayNDS.y, -1.0f, 1.0f);
        glm::vec4 ray_eye = glm::inverse(proj) * ray_clip;
        ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0);
        glm::vec4 ray_wor = (glm::inverse(view) * ray_eye);
        glm::vec3 ray_mod = (glm::inverse(world) * ray_wor);
        ray_mod = glm::normalize(ray_mod);
        //ray_wor = glm::normalize(ray_wor);
        ray_wor = glm::normalize(ray_wor);
        glm::vec3 rayModelSpace = (glm::inverse(world) * glm::vec4(ray_wor.x, ray_wor.y, ray_wor.z,1.0f));
        if (mouseClick)
        {
            
            glm::vec4 rayOrigin(0, 0, 0,1);
            rayOrigin = glm::inverse(view) * rayOrigin;
            std::cout << "Ray Origin World at (" << rayOrigin.x << " : " << rayOrigin.y << " : " << rayOrigin.z << std::endl;
            
            rayOrigin = (glm::inverse(world)* rayOrigin);
            std::cout << "Ray Origin Obj Space at (" << rayOrigin.x << " : " << rayOrigin.y << " : " << rayOrigin.z << std::endl;
            rayModelSpace = glm::normalize(rayModelSpace);
            Ray ray(rayOrigin, ray_mod);
            std::cout << "Ray Direction at (" << ray_wor.x << " : " << ray_wor.y << " : " << ray_wor.z << std::endl;
            std::cout << "Ray in Obj Space at (" << ray_mod.x << " : " << ray_mod.y << " : " << ray_mod.z << std::endl;
            std::cout << "Ray Lenght OBJ space: " << glm::length(rayModelSpace) << std::endl;
            std::cout << Bounds::intersect(ray, bBox) << std::endl;
            mouseClick = false;
        }
        shader.Bind();
        //glm::mat4 world = glm::translate(glm::mat4(1.0f), glm::vec3(ray_eye)) * glm::scale(glm::mat4(1.0f), glm::vec3(100, 100, 1));
        shader.SetUniformMat4f("u_proj", proj);
        world = glm::translate(glm::mat4(1.0f), glm::vec3(0, 5, 0));

        shader.SetUniformMat4f("u_model", world);
        shader.SetUniformMat4f("u_view", view);


        glm::mat4 mvp = proj * view * world;

        shader.Bind();
        shader.SetUniform4f("u_Color", r, g, b, 1);
        texture.Bind();
        shader.SetUniformMat4f("u_MVP", mvp);
        renderer.Draw(va, ib, shader);
        //world = glm::translate(glm::mat4(1.0f), translation2) * glm::scale(glm::mat4(1.0f), glm::vec3(100, 100, 1));
        mvp = proj * view * world;
        shader.SetUniformMat4f("u_MVP", mvp);
        renderer.Draw(va, ib, shader);
        world = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
        LightShader.Bind();
        container.Bind();
        LightShader.SetUniformMat4f("u_proj", proj);
        LightShader.SetUniformMat4f("u_view", view);
        //world2 = glm::rotate(world2, 2 * deltaTime, glm::vec3(0, 1, 0));
        //LightShader.SetUniformMat4f("u_model", world2);
        //LightShader.SetUniform3f("light.direction", view * glm::vec4(lightDir, 0));
        //std::cout << camera.GetCameraPosition().x << " " << camera.GetCameraPosition().y << " " << camera.GetCameraPosition().z << std::endl;
        LightShader.SetUniform3f("u_ViewPos", camera.GetCameraPosition());
        //LightShader.SetUniform3f("u_LightPos", camera.GetCameraPosition());
        //LightShader.SetUniform3f("light.direction", camera.GetCameraForward());


        model.Draw(LightShader);
        //renderer.DrawTriangles(vaLightBox, LightShader);

        for (int i = 0; i < 5; i++)
        {
            glm::mat4 cubWorld= glm::translate(glm::mat4(1.0f), cubePos[i]);
            LightShader.SetUniformMat4f("u_model", cubWorld);

            renderer.DrawTriangles(vaLightBox, LightShader);
        }
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