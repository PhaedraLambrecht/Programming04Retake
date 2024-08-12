#ifndef ENGINE2D_HEALTHCOMPONENT_H
#define ENGINE2D_HEALTHCOMPONENT_H
#include "BaseComponent.h"


namespace dae
{
	class HealthComponent final : public BaseComponent
	{
	public:

		HealthComponent(GameObject* owner);
		~HealthComponent() = default;
		
		HealthComponent(const HealthComponent& other) = delete;
		HealthComponent(HealthComponent&& other) = delete;
		HealthComponent& operator=(const HealthComponent& other) = delete;
		HealthComponent& operator=(HealthComponent&& other) = delete;



		void SetHealth(int health);
		int GeTHealth() const;

		void SetPlayerIndex(unsigned playerIndex);
		void Damaged(int damage);


	private:

		void Death();
	
		int m_Health;
		unsigned m_PlayerIndex;
	};
}


#endif // !ENGINE2D_HEALTHCOMPONENT_H
