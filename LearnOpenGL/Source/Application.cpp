#include <glad/glad.h> 
#include <GLFW/glfw3.h>


#include"glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <iostream>
#include "RenderFrameWork/VertexBuffer.h"
#include "RenderFrameWork/StaticShader.h"
#include "RenderFrameWork/VertexArray.h"
#include "RenderFrameWork/IndexBuffer.h"
#include "RenderFrameWork/Texture.h"

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

        
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


int main(void)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


    

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    //遍历驱动，存储显卡中opengl的函数指针(与glew一致)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    float vertices[] = {
    0.5f, 0.5f, 0.0f, 1.0f,1.0f,  // 右上角
    0.5f, -0.5f, 0.0f,1.0f,0.0f,  // 右下角
    -0.5f, -0.5f, 0.0f,0.0f,0.0f, // 左下角
    -0.5f, 0.5f, 0.0f,0.0f,1.0f  // 左上角
    };

    unsigned int indices[] = {
        // 注意索引从0开始! 
        // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
        // 这样可以由下标代表顶点组合成矩形

        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
    };


   

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    VertexBuffer VBO = VertexBuffer(vertices,sizeof(vertices), GL_STATIC_DRAW);
    IndexBuffer IBO = IndexBuffer(indices,sizeof(indices));
    
    
    VertexArray VAO = VertexArray();
    VAO.m_vBufferDesc.Push<float>(3);
    VAO.m_vBufferDesc.Push<float>(2);
    VAO.AddBuffer(VBO,IBO);
    


    StaticShader TestShader = StaticShader();
    TestShader.AttachNewShaderResource("Resource/Shader/Testver.shader", GL_VERTEX_SHADER);
    TestShader.AttachNewShaderResource("Resource/Shader/Testfra.shader", GL_FRAGMENT_SHADER);
    TestShader.Linked();
    TestShader.Bind();

    Texture tes = Texture("Resource/block/steep_planks.png");
    tes.Bind();
    TestShader.SetUniformVariablei("u_Texture", 0);

    VAO.UnBind();
    TestShader.UnBind();

    /** imgui setup */
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    
    const char* glsl_version = "#version 130";
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    bool Isline = false;


    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        

        TestShader.Bind();
        VAO.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        {
            static float f = 0.0f;

            ImGui::Begin("GLTest Tool");                          // Create a window called "Hello, world!" and append into it.
            ImGui::Text("Is Show Line？");               // Display some text (you can use a format strings too)
                   // Edit 1 float using a slider from 0.0f to 1.0f
            
            if(ImGui::Checkbox("1",&Isline))
            {
                if(Isline)
                {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                }
                
                else
                {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                }
            }
            

            

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;

}