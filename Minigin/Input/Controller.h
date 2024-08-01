#ifndef ENGINE2D_CONTROLLER_H
#define ENGINE2D_CONTROLLER_H
#include <memory>

namespace dae
{
	enum ControlerButton
	{
		DPAD_UP = 0x0001,
		DPAD_DOWN = 0x0002,
		DPAD_LEFT = 0x0004,
		DPAD_RIGHT = 0x0008,
		START = 0x0010,
		BACK = 0x0020,
		LEFT_THUMB = 0x0040,
		RIGHT_THUMB = 0x0080,
		LEFT_SHOULDER = 0x0100,
		RIGHT_SHOULDER = 0x0200,
		LEFT_TRIGGER = 0x0400,
		RIGHT_TRIGGER = 0x0800,
		A = 0x1000,
		B = 0x2000,
		X = 0x4000,
		Y = 0x8000
	};



	class Controller final
	{
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



	private:
		
		class ControllerImplementation;
		std::unique_ptr<ControllerImplementation> m_pImpl{};

	};
}

#endif // !ENGINE2D_CONTROLLER_H