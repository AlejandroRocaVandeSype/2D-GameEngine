#define WIN32_LEAND_AND_MEAN
#include <Windows.h>
#include <XInput.h>
#include <iostream>
#include "Controller.h"

/* IMPLEMENTATION OF THE CONTROLLER CLASS TO IMPROVE 
COMPILE TIMES  (PIMPL PATTERN) */
class Controller::ControllerImpl
{

public:
	ControllerImpl(unsigned controllerIdx)
		: m_ControllerIndex { controllerIdx }
		, m_IsConnected { true }
		, m_ItWasConnected { true }
	{	
		// Clear states to ensure it only contains valid data
		ZeroMemory(&m_PreviousState, sizeof(XINPUT_STATE));
		ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
	}

	// Updates the state of the Controller being connected and check button changes
	void ProcessRequests()
	{
		// Save the state in order to be able to compare and detect which buttons have been 
		// pressed or released since last frame
		CopyMemory(&m_PreviousState, &m_CurrentState, sizeof(XINPUT_STATE));
		// Ensure it only contains valid data
		ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));	
		// Retrieves the state of the controller
		XInputGetState(m_ControllerIndex, &m_CurrentState);

		// Check if any button was released/pressed this frame
			// wButtons represents the state of each button on the gamepad
		auto buttonChanges{ m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons };
		m_ButtonPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
		m_ButtonReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);

	}

	bool IsDownThisFrame(unsigned int button) const { return m_ButtonPressedThisFrame & button; }
	bool IsUpThisFrame(unsigned int button) const { return m_ButtonReleasedThisFrame & button; }
	bool IsPressed(unsigned int button) const { return m_CurrentState.Gamepad.wButtons & button; } 


	// -----------------------------------------------------------------------------
	//			*Check if the controller is still connected or not*
	// This function is only called when a Controller has been conected to the game
	// Once it is connected, it is called every frame to check if the controller gets
	// connected or disconected. 
	// -----------------------------------------------------------------------------
	bool IsConnected() 
	{
		// TODO: Instead of showing a text in the console the connected/disconnnected should be done with Imgui
		XINPUT_STATE state;
		DWORD result = XInputGetState(m_ControllerIndex, &state);
		if (result == ERROR_SUCCESS)
		{
			if (!m_ItWasConnected)  // Check previous state of the Controller
			{
				// Controller has been reconnected
				std::cout << "Controller " << m_ControllerIndex << " connected. \n";
				m_IsConnected = true;
				m_ItWasConnected = true;
			}
		}
		else
		{
			// Controller disconnected
			if (m_ItWasConnected)
			{
				std::cout << "Controller " << m_ControllerIndex << " disconnected. \n";
				m_IsConnected = false;
				m_ItWasConnected = false;
			}
		}
	
		return m_IsConnected;
		
	}
	
	const unsigned GetControllerIndex() const { return m_ControllerIndex; };

private:
	// These two are used to compare the state of the controller and see which buttons
	// have been pressed or released since the last frame
	XINPUT_STATE m_PreviousState{};
	XINPUT_STATE m_CurrentState{};

	WORD m_ButtonPressedThisFrame;
	WORD m_ButtonReleasedThisFrame;

	unsigned m_ControllerIndex;			// Index of the connected controller
	bool m_IsConnected;					// Automatically true when an instance of this class is created
	bool m_ItWasConnected;				// Previous state of the Controller (in order to know if it has been reconnected)

};



Controller::Controller(unsigned controllerIdx)
{
	pImpl = std::make_unique<ControllerImpl>(controllerIdx);
}

Controller::~Controller()
{
}

void Controller::ProcessRequests()
{
	pImpl->ProcessRequests();
}

bool Controller::IsDown(XboxControllerButton button) const
{
	return pImpl->IsDownThisFrame(static_cast<unsigned int>(button));
}

bool Controller::IsUp(XboxControllerButton button) const
{
	return pImpl->IsUpThisFrame(static_cast<unsigned int>(button));
}

bool Controller::IsPressed(XboxControllerButton button) const
{
	return pImpl->IsPressed(static_cast<unsigned int>(button));
}

bool Controller::IsConnected() const
{
	return pImpl->IsConnected();
}

const unsigned Controller::GetControllerIdx() const
{
	return pImpl->GetControllerIndex();
}
