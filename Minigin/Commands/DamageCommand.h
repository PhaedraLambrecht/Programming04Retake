#ifndef ENGINE2D_DAMAGECOMMAND_H
#define ENGINE2D_DAMAGECOMMAND_H
#include "Command.h"


namespace dae
{
	class HealthComponent;
	class GameObject;

	class DamageCommand : public BaseCommand
	{
	public:

		DamageCommand(GameObject* actor, int damage);
		~DamageCommand() override;

		DamageCommand(const DamageCommand& other) = delete;
		DamageCommand(DamageCommand&& other) = delete;
		DamageCommand& operator=(const DamageCommand& other) = delete;
		DamageCommand& operator=(DamageCommand&& other) = delete;




		void Execute() override;


	private:


		HealthComponent* m_pHealthComponnent;
		int m_Damage;
	};
}


#endif //ENGINE2D_DAMAGECOMMAND_H