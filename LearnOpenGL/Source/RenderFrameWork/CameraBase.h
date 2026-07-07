#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class CameraBase
{
public:
    
    CameraBase(glm::vec3 Location);
    ~CameraBase();
    
    void BindToWindow(GLFWwindow* window);
    void Tick(float deltaTime,GLFWwindow *window);
    void MouseInputCallBack(double xpos, double ypos);
    void Render();
    glm::vec3 GetCameraPos() const { return cameraPos; }
    
    glm::mat4 GetViewMatrix() const { return view; }
protected:
    glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
    
    glm::mat4 view = glm::mat4(1.0f);;
    
    float pitch = 0.0f;
    float yaw   = -90.0f;
    
    float speed = 2.5f;
    float sensitivity = 0.05;
    
    double lastX_ = 0.0, lastY_ = 0.0;
    
    bool firstMouse = false;
    
    
    static void MouseCallback(GLFWwindow* window, double xpos, double ypos) {
        auto* cam = static_cast<CameraBase*>(glfwGetWindowUserPointer(window));
        if (cam) cam->MouseInputCallBack(xpos, ypos);
    }
};
