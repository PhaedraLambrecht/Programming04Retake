#ifndef ENGINE2D_CONTROLER_H
#define ENGINE2D_CONTROLER_H

namespace Engine
{
	enum class ControllerButton
	{
		DPadUp = 0x0001,
		DPadDown = 0x0002,
		DPadLeft = 0x0004,
		DPadRight = 0x0008,
		Start = 0x0010,
		Back = 0x0020,
		LeftThumb = 0x0040,
		RightThumb = 0x0080,
		LeftShoulder = 0x0100,
		RightShoulder = 0x0200,
		ButtonA = 0x1000,
		ButtonB = 0x2000,
		ButtonX = 0x4000,
		ButtonY = 0x8000,
	};

	class Controller
	{
		class ControllerImplementation;
		ControllerImplementation* m_pImpl{};

	public:

		Controller(unsigned int controllerIdx);
		~Controller();

		// Copy and move
		Controller(const Controller& other) = delete;
		Controller(Controller&& other) = delete;
		Controller& operator=(const Controller& other) = delete;
		Controller& operator=(Controller&& other) = delete;



		void Update();

		bool IsButtonDown(unsigned int button) const;
		bool IsButtonUp(unsigned int button) const;
		bool IsButtonPressed(unsigned int button) const;

	};
}

#endif // !ENGINE2D_CONTROLER_H