#define WIN32_LEAN_AND_MEAN
#include "Controller.h"
#include <algorithm>
#include <Windows.h>
#include <XInput.h>
#pragma comment(lib, "xinput.lib")
#include <iostream>


namespace dae
{
    class Controller::ControllerImplementation final
    {
    public:

        ControllerImplementation(unsigned int controllerIndex)
            : m_ControllerIndex{ controllerIndex }
            , m_PreviousState{}
            , m_CurrentState{}
            , m_ButtonsPressedThisFrame{}
            , m_ButtonsReleasedThisFrame{}
        {
        }


        void Update()
        {
            m_PreviousState = m_CurrentState;
            XInputGetState(m_ControllerIndex, &m_CurrentState);// Get the new state of the XInput controller


            auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
            // Calculate which buttons were just pressed down and released up
            m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
            m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);
        }


        bool IsButtonDownThisFrame(unsigned int button) const
        {
            return m_ButtonsPressedThisFrame & button;
        }
        bool IsButtonUpThisFrame(unsigned int button) const
        {
            return m_ButtonsReleasedThisFrame & button;
        }
        bool IsButtonPressedThisFrame(unsigned int button) const
        {
            return m_CurrentState.Gamepad.wButtons & button;
        }




    private:

        unsigned int m_ControllerIndex;

        XINPUT_STATE m_PreviousState;
        XINPUT_STATE m_CurrentState;

        unsigned int m_ButtonsPressedThisFrame;
        unsigned int m_ButtonsReleasedThisFrame;
    };



    dae::Controller::Controller(unsigned int controllerIdx)
        :m_pImpl{ std::make_unique<ControllerImplementation>(controllerIdx) }
    {
    }

    dae::Controller::~Controller()
    {
        std::cout << "Controller" << '\n';
    }


    void dae::Controller::Update()
    {
        m_pImpl->Update();
    }

    bool dae::Controller::IsButtonDown(unsigned int button) const
    {
        return m_pImpl->IsButtonDownThisFrame(button);
    }
    bool dae::Controller::IsButtonUp(unsigned int button) const
    {
        return m_pImpl->IsButtonUpThisFrame(button);
    }
    bool dae::Controller::IsButtonPressed(unsigned int button) const
    {
        return m_pImpl->IsButtonPressedThisFrame(button);
    }

}