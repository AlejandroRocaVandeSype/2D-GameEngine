#ifndef GAME_ENGINE_CONTROLLER
#define GAME_ENGINE_CONTROLLER
#include <memory>

class Controller final
{
public:
	enum class XboxControllerButton  
	{ 
		// Specific for XBOX 360 CONTROLLER
		DPadUp = 0x0001,
		DPadDown = 0x0002,
		DPadLeft = 0x0004,
		DPadRigth = 0x0008,
		Start = 0x0010,
		Back = 0x0020,
		LeftThumb = 0x0040,
		RightThumb = 0x0080,
		LeftShoulder = 0x0100,
		RightShoulder = 0x0200,
		ButtonA = 0x1000,
		ButtonB = 0x2000,
		ButtonX = 0x4000,
		ButtonY = 0x8000
	};

	explicit Controller(unsigned controllerIdx);
	~Controller();

	void ProcessRequests();

	bool IsDown(XboxControllerButton button) const;
	bool IsUp(XboxControllerButton button) const;
	bool IsPressed(XboxControllerButton button) const;

	bool IsConnected() const;

	const unsigned GetControllerIdx() const;

private:
	// Pimpl pattern --> This way none of the classes will know that XInput is being used
	class ControllerImpl;
	std::unique_ptr<ControllerImpl> pImpl;

};

#endif