#define WIN32_LEAN_AND_MEAN
#include "Controler.h"
#include <algorithm>
#include <Windows.h>
#include <XInput.h>
#pragma comment(lib, "xinput.lib")


namespace Engine
{
    class Controller::ControllerImplementation
    {
    public:

        ControllerImplementation(unsigned int controllerIndex)
            : m_controllerIndex{ controllerIndex } 
        {
            ZeroMemory(&m_previousState, sizeof(XINPUT_STATE));
            ZeroMemory(&m_currentState, sizeof(XINPUT_STATE));
        }

        void Update() 
        {
            CopyMemory(&m_previousState, &m_currentState, sizeof(XINPUT_STATE));
            ZeroMemory(&m_currentState, sizeof(XINPUT_STATE));

            XInputGetState(m_controllerIndex, &m_currentState);

            auto buttonChanges = m_currentState.Gamepad.wButtons ^ m_previousState.Gamepad.wButtons;
            m_ButtonsPressedThisFrame = buttonChanges & m_currentState.Gamepad.wButtons;
            m_ButtonsReleasedThisFrame = buttonChanges & (~m_previousState.Gamepad.wButtons);
        }


        bool IsButtonDown(unsigned int button) const 
        {
            return m_ButtonsPressedThisFrame & button;
        }
        bool IsButtonUp(unsigned int button) const 
        {
            return m_ButtonsReleasedThisFrame & button;
        }
        bool IsButtonPressed(unsigned int button) const 
        {
            return m_currentState.Gamepad.wButtons & button;
        }



    private:

        unsigned int m_controllerIndex{};

        XINPUT_STATE m_previousState{};
        XINPUT_STATE m_currentState{};

        WORD m_ButtonsPressedThisFrame{};
        WORD m_ButtonsReleasedThisFrame{};
    };
}


Engine::Controller::Controller(unsigned int controllerIdx)
	:m_pImpl{ new ControllerImplementation{ controllerIdx } }
{
}
Engine::Controller::~Controller()
{
}



void Engine::Controller::Update()
{
    m_pImpl->Update();
}

bool Engine::Controller::IsButtonDown(unsigned int button) const
{
    return m_pImpl->IsButtonDown(button);
}
bool Engine::Controller::IsButtonUp(unsigned int button) const
{
    return m_pImpl->IsButtonUp(button);
}
bool Engine::Controller::IsButtonPressed(unsigned int button) const
{
    return m_pImpl->IsButtonPressed(button);
}
