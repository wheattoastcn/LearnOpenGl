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
#include "RenderFrameWork/CameraBase.h"


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

float CountDeltaTime(float& lastFrame)
{
    float currentFrame = static_cast<float>(glfwGetTime());
    float deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    
    return deltaTime;
}


int main(void)
{
    
    //  ==== Init GLFW And GLAD ====//
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

    //open depth test
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    //==== one render target ====//
    
float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f
};

    unsigned int indices[] = {
        0, 1, 2, 3, 4, 5,
        6, 7, 8, 9, 10, 11,
        12, 13, 14, 15, 16, 17,
        18, 19, 20, 21, 22, 23,
        24, 25, 26, 27, 28, 29,
        30, 31, 32, 33, 34, 35
    };

    

    VertexBuffer VBO = VertexBuffer(vertices,sizeof(vertices), GL_STATIC_DRAW);
    IndexBuffer IBO = IndexBuffer(indices, sizeof(indices));
    
    
    VertexArray VAO = VertexArray();
    VAO.m_vBufferDesc.Push<float>(3);
    VAO.m_vBufferDesc.Push<float>(2);
    VAO.m_vBufferDesc.Push<float>(3);
    VAO.AddBuffer(VBO,IBO);
    


    StaticShader TestShader = StaticShader();
    TestShader.AttachNewShaderResource("Resource/Shader/Testver.shader", GL_VERTEX_SHADER);
    TestShader.AttachNewShaderResource("Resource/Shader/Blendlightfra.shader", GL_FRAGMENT_SHADER);
    
    
    TestShader.Linked();
    TestShader.Bind();

   
    Texture tes = Texture("Resource/container2.png");
    Texture tes_specular_map = Texture("Resource/container2_specular.png");
    tes.Bind(0);
    TestShader.SetUniformVariablei("material.Texture", 0);
    tes_specular_map.Bind(1);
    TestShader.SetUniformVariablei("material.Specular", 1);

    
    
    VAO.UnBind();
    TestShader.UnBind();
    
    VertexArray VAO_light = VertexArray();
    VAO_light.m_vBufferDesc.Push<float>(3);
    VAO_light.m_vBufferDesc.Push<float>(2);
    VAO_light.m_vBufferDesc.Push<float>(3);
    VAO_light.AddBuffer(VBO,IBO);
    
    StaticShader LightShader = StaticShader();
    LightShader.AttachNewShaderResource("Resource/Shader/basever.shader", GL_VERTEX_SHADER);
    LightShader.AttachNewShaderResource("Resource/Shader/baselightfra.shader", GL_FRAGMENT_SHADER);
    LightShader.Linked();
    LightShader.Bind();
    
    VAO_light.UnBind();
    LightShader.UnBind();
    

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

    
    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

    // 10 个正方体的位置，全部放在光源的负 X 侧（光源左侧）
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3(-1.5f,  0.5f, -1.0f),
        glm::vec3(-3.0f, -0.5f,  1.0f),
        glm::vec3(-4.5f,  1.0f, -1.5f),
        glm::vec3(-6.0f,  0.0f,  0.5f),
        glm::vec3(-7.5f, -1.0f, -1.0f),
        glm::vec3(-1.0f, -1.5f,  2.0f),
        glm::vec3(-2.5f,  1.5f,  2.5f),
        glm::vec3(-4.0f, -2.0f,  3.0f),
        glm::vec3(-5.5f,  2.0f,  3.5f)
    };
    
    glm::mat4 model_light = glm::mat4(1.0f);
    model_light = glm::translate(model_light, lightPos);
    model_light = glm::scale(model_light, glm::vec3(0.2f));
    
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), 800.f/ 600.f, 0.1f, 100.0f);
    
    CameraBase* Test = new CameraBase(glm::vec3(0,0,3));
   Test->BindToWindow(window);
    
    float lastFrame = 0.0f;
    
   
    
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        
        //相机移动
        Test->Tick(CountDeltaTime(lastFrame),window);
        

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
        
        //物体旋转
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        
        
        TestShader.Bind();
        TestShader.SetUniformMatrix4f("v",Test->GetViewMatrix());
        TestShader.SetUniformMatrix4f("p",projection);
        TestShader.SetUniformMatrix4f("transform",transform); 

        // 材质参数
        TestShader.SetUniformVariable1f("material.diffuseStrength",0.8f);
        TestShader.SetUniformVariable1f("material.specularStrength",0.8f);
        TestShader.SetUniformVariable1f("material.shininess",128.f);

        // 平行光（偏蓝的冷色调，营造阴森氛围）
        TestShader.SetUniformVariable3f("dirLight.direction",-0.2f, -1.0f, -0.3f);
        TestShader.SetUniformVariable3f("dirLight.color",0.2f, 0.4f, 0.8f);
        TestShader.SetUniformVariable1f("dirLight.intensity",0.6f);
        TestShader.SetUniformVariable1f("dirLight.ambientStrength",0.1f);

        // 点光源（暖色，模拟火把/蜡烛）
        TestShader.SetUniformVariable3f("pointLight.color",1.0f, 0.5f, 0.2f);
        TestShader.SetUniformVariable3f("pointLight.position",lightPos);
        TestShader.SetUniformVariable1f("pointLight.constant",1.0f);
        TestShader.SetUniformVariable1f("pointLight.linear",0.09f);
        TestShader.SetUniformVariable1f("pointLight.quadratic",0.032f);
        TestShader.SetUniformVariable1f("pointLight.intensity",1.0f);
        TestShader.SetUniformVariable1f("pointLight.ambientStrength",0.1f);

        // 聚光灯（手电筒，无环境光）
        TestShader.SetUniformVariable3f("spotLight.color",1.0f, 1.0f, 1.0f);
        TestShader.SetUniformVariable3f("spotLight.position",Test->GetCameraPos());
        TestShader.SetUniformVariable3f("spotLight.direction",Test->GetCameraFront());
        TestShader.SetUniformVariable1f("spotLight.cutOff",glm::cos(glm::radians(12.5f)));
        TestShader.SetUniformVariable1f("spotLight.outerCutOff",glm::cos(glm::radians(17.5f)));
        TestShader.SetUniformVariable1f("spotLight.intensity",1.0f);

        TestShader.SetUniformVariable3f("viewPos",Test->GetCameraPos());
        

        VAO.Bind();
        // 循环绘制 10 个正方体，通过 model 矩阵修改各自位置
        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 cubeModel = glm::mat4(1.0f);
            cubeModel = glm::translate(cubeModel, cubePositions[i]);
            cubeModel = glm::rotate(cubeModel, (float)glfwGetTime() + i, glm::vec3(0.0f, 1.0f, 0.0f));
            TestShader.SetUniformMatrix4f("m", cubeModel);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        
        
        LightShader.Bind();
        LightShader.SetUniformMatrix4f("m",model_light);
        LightShader.SetUniformMatrix4f("v",Test->GetViewMatrix());
        LightShader.SetUniformMatrix4f("p",projection);
        
        VAO_light.Bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);
        

        {
            static float f = 0.0f;

            ImGui::Begin("GLTest Tool");                          // Create a window called "Hello, world!" and append into it.
            ImGui::Text("Is Show Line?");               // Display some text (you can use a format strings too)
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

    delete Test;
    glfwTerminate();
    return 0;

}
