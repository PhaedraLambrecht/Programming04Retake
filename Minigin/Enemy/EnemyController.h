#ifndef ENGINE2D_ENEMYCONTROLLER_H
#define ENGINE2D_ENEMYCONTROLLER_H
#include "Componennts/BaseComponent.h"


namespace dae
{
	class BaseEnemyComponent;

	class EnemyController final : public BaseComponent
	{
	public:

		EnemyController(GameObject* owner);
		~EnemyController() override = default;

		EnemyController(const EnemyController& other) = delete;
		EnemyController(EnemyController&& other) = delete;
		EnemyController& operator=(const EnemyController& other) = delete;
		EnemyController& operator=(EnemyController&& other) = delete;



		void Update() override;


	private:

		BaseEnemyComponent* m_pEnemyController;

		float m_attackDelay;
		float m_currentAttackDelay;
	};

}

#endif // !ENGINE2D_ENEMYCONTROLLER_H