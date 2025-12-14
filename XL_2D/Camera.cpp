#include "Camera.hpp"

_NAMESPACE_BEGIN

    Camera::Camera(float Aspectio)
        : m_Aspectio(Aspectio), m_cPos{0.0f, 0.0f, 20.0f}, m_Yaw{-90.0f}, m_Pitch{0.0f}, m_Zoom{45.0f}
    {
        Update();
    }
    Camera::~Camera()
    {

    }

    void Camera::Update()
    {
        // calculate camera front
        glm::vec3 cmFront{
            cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch)),
            sin(glm::radians(m_Pitch)),
            sin(glm::radians(m_Yaw) * cos(glm::radians(m_Pitch)))
            };

        m_cFront = glm::normalize(cmFront);
        m_cRight = glm::normalize(glm::cross(m_cFront,m_GlobalUp));
        m_cUp = glm::normalize(glm::cross(m_cRight,m_cFront));

        m_View = glm::lookAt( m_cPos, m_cPos + m_cFront ,m_cUp );

        if (m_ProjectionType == ProjectionType::Perspective)
        {
            m_Projection = glm::perspective(glm::radians(m_Zoom), m_Aspectio, 0.1f, 1000.0f);
        }
        else
        {
            float half = m_OrthoSize * 0.5f;
            m_Projection = glm::ortho(-half * m_Aspectio, half * m_Aspectio, -half, half, -1000.0f, 1000.0f);
        }
    }
    
    void Camera::OnUpdate()
    {
        float move_velocity = m_MoveSpeed * m_ts;
        float rotate_velocity = m_MoveSpeed * m_ts;

        //// Toggle projection type with O key (edge detect)
        //bool oDown = WindowsInput::IsKeyPressed(Key::O);
        //if (oDown && !m_ToggleKeyPrevState)
        //{
        //    m_ProjectionType = (m_ProjectionType == ProjectionType::Perspective) ? ProjectionType::Orthographic : ProjectionType::Perspective;
        //    Update();
        //}
        //m_ToggleKeyPrevState = oDown;
    }

    void Camera::OnKeyDown(uint32_t nChar, uint32_t nRepCnt)
    {
        float move_velocity = m_MoveSpeed * m_ts;
        float rotate_velocity = m_MoveSpeed * m_ts;

        switch (nChar)
        {
            // 前后
        case 'w':
        case 'W':
            m_cPos += move_velocity * m_cFront;
            Update();
            break;

        case 's':
        case 'S':
            m_cPos -= move_velocity * m_cFront;
            Update();
            break;

            // 左右（基于右向量）
        case 'a':
        case 'A':
            m_cPos -= move_velocity * glm::normalize(glm::cross(m_cFront, m_cUp));
            Update();
            break;

        case 'd':
        case 'D':
            m_cPos += move_velocity * glm::normalize(glm::cross(m_cFront, m_cUp));
            Update();
            break;

            // 上下（沿相机上向量）
        case 'q':
        case 'Q':
            m_cPos += move_velocity * m_cUp;
            Update();
            break;

        case 'e':
        case 'E':
            m_cPos -= move_velocity * m_cUp;
            Update();
            break;

        default:
            break;
        }
    }

    void Camera::OnResize(float width, float height)
    {
        m_Aspectio = width / height;
        Update();
    }

    void Camera::OnMouseWheel(int yOffset)
    {
        m_Zoom -= yOffset * m_ts;

        if (m_Zoom < 1.0f)
            m_Zoom = 1.0f;
        if (m_Zoom > 90.0f)
            m_Zoom = 90.0f;

        Update();
    }

    void Camera::OnMouseMove(int xOffset, int yOffset)
    {
        return;
        float xpos = static_cast<float>(xOffset);
        float ypos = static_cast<float>(yOffset);

        if (m_isFirstMouse)
        {
            m_LastX = xpos;
            m_LastY = ypos;
            m_isFirstMouse = false;
        }

        float xoffset = xpos - m_LastX;
        float yoffset = m_LastY - ypos;

        m_LastX = xpos;
        m_LastY = ypos;

        m_Yaw += xoffset * m_Sensitivity;
        m_Pitch += yoffset * m_Sensitivity;

        if (m_Pitch > 89.0f)
            m_Pitch = 89.0f;

        if (m_Pitch < -89.0f)
            m_Pitch = -89.0f;

        Update();
    }

_NAMESPACE_END