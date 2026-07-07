#include "CameraBase.h"

#include "glm/ext/matrix_transform.hpp"
#include "imgui/imgui.h"

CameraBase::CameraBase(glm::vec3 Location)
    : cameraPos(Location)
{
    
    
    cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
}

CameraBase::~CameraBase()
{
}

void CameraBase::BindToWindow(GLFWwindow* window)
{
    glfwSetWindowUserPointer(window, this);
    glfwSetCursorPosCallback(window, CameraBase::MouseCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void CameraBase::Tick(float deltaTime,GLFWwindow *window)
{
    //if (ImGui::GetIO().WantCaptureMouse) return;
    float cameraSpeed = speed * deltaTime;
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    
    
    Render();
}

void CameraBase::MouseInputCallBack(double xpos, double ypos)
{
    if (firstMouse) {
        lastX_ = xpos;
        lastY_ = ypos;
        firstMouse = false;
        return;
    }

    float xoffset = static_cast<float>(xpos - lastX_);
    float yoffset = static_cast<float>(lastY_ - ypos); // 反向，保证向上为正

    lastX_ = xpos;
    lastY_ = ypos;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void CameraBase::Render()
{
    
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
    
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}
