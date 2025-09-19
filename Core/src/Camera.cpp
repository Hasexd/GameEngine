#include "Camera.h"

namespace Core
{
    Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch):
        Front(glm::vec3(0.0f, 0.0f, -1.0f)),
        MovementSpeed(2.5f),
        MouseSensitivity(0.1f),
        FieldOfView(45.0f)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;

        UpdateVectors();
    }

    glm::mat4 Camera::GetViewMatrix() const
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    void Camera::ProcessKeyboard(Direction direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;

        switch (direction)
        {
        case Direction::FORWARD:
            Position += Front * velocity;
            break;
		case Direction::BACKWARD:
            Position -= Front * velocity;
            break;
        case Direction::LEFT:
			Position -= Right * velocity;
            break;
        case Direction::RIGHT:
            Position += Right * velocity;
			break;
        case Direction::UP:
			Position += WorldUp * velocity;
            break;
        case Direction::DOWN:
            Position -= WorldUp * velocity;
            break;
        }
    }

    void Camera::ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch)
    {
        xOffset *= MouseSensitivity;
        yOffset *= MouseSensitivity;

        Yaw += xOffset;
        Pitch += yOffset;

        if (constrainPitch)
        {
            Pitch = std::clamp(Pitch, -89.0f, 89.0f);
        }

		UpdateVectors();
    }

    void Camera::ProcessMouseScroll(float yOffset)
    {
        FieldOfView -= yOffset;
        FieldOfView = std::clamp(FieldOfView, 1.0f, 90.0f);
    }

    void Camera::UpdateVectors()
    {
        glm::vec3 front = {};

        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

        Front = glm::normalize(front);
        Right = glm::normalize(glm::cross(Front, WorldUp));

        Up = glm::normalize(glm::cross(Right, Front));
    }
}