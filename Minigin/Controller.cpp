#define WIN32_LEAND_AND_MEAN
#include <Windows.h>
#include <XInput.h>

#include "Controller.h"

/* IMPLEMENTATION OF THE CONTROLLER CLASS TO IMPROVE 
COMPILE TIMES  (PIMPL PATTERN) */
class Controller::ControllerImpl
{

public:
	ControllerImpl(unsigned controllerIdx)
		: m_ControllerIndex { controllerIdx }
	{	
		// Clear states to ensure it only contains valid data
		ZeroMemory(&m_PreviousState, sizeof(XINPUT_STATE));
		ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
	}

	// Updates the state of the Controller being connected and check button changes
	void Update()
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

	bool IsConnected() const 
	{

		// Get the state of this controller to check if it is connected
		XINPUT_STATE state;
		DWORD result = XInputGetState(m_ControllerIndex, &state);
		if (result == ERROR_SUCCESS)
		{
			// This controller is Connected
			return true;
		}

		return false;
	}

	const unsigned GetControllerIndex() const { return m_ControllerIndex; };

private:
	// These two are used to compare the state of the controller and see which buttons
	// have been pressed or released since the last frame
	XINPUT_STATE m_PreviousState{};
	XINPUT_STATE m_CurrentState{};

	WORD m_ButtonPressedThisFrame;
	WORD m_ButtonReleasedThisFrame;

	unsigned m_ControllerIndex;					// Index of the connected controller

};



Controller::Controller(unsigned controllerIdx)
{
	pImpl = new ControllerImpl(controllerIdx);
}


Controller::~Controller()
{
	delete pImpl;
}

void Controller::Update()
{
	pImpl->Update();
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

bool  Controller::IsConnected() const
{
	return pImpl->IsConnected();
}

const unsigned Controller::GetControllerIdx() const
{
	return pImpl->GetControllerIndex();
}