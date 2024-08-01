#ifndef ENGINE2D_FPSCOMPONENT_H
#define ENGINE2D_FPSCOMPONENT_H
#include "BaseComponent.h"


namespace dae
{
	class TextComponent;

	class FPSComponent final : public BaseComponent
	{
	public:

		FPSComponent(GameObject* Owner);
		~FPSComponent() override;

		// Copy and move
		FPSComponent(const FPSComponent& other) = delete;
		FPSComponent(FPSComponent&& other) = delete;
		FPSComponent& operator=(const FPSComponent& other) = delete;
		FPSComponent& operator=(FPSComponent&& other) = delete;



		void Update() override;


	private:

		float m_fps;
		TextComponent* m_pText;
	};
}

#endif // !ENGINE2D_FPSCOMPONENT_H