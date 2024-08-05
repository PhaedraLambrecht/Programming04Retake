#include "DiamondBlockComponent.h"
#include "events/Event.h"
#include "Commands/AddPointsCommand.h"
#include "GameObject.h"


dae::DiamondBlockComponent::DiamondBlockComponent(GameObject* owner)
	: BlockComponent(owner)
	, m_Health{ 1 }
{

}

void dae::DiamondBlockComponent::Initialize(std::shared_ptr<GameObject> player)
{
	m_pPlayer = player.get();
}

void dae::DiamondBlockComponent::OnHitCallback(const CollisionData& /*collisionOwner*/, const CollisionData& hitObject)
{
	if (hitObject.ownerType != "DiamondAttack")
		return;

	DoDamage();
}

void dae::DiamondBlockComponent::OnDeath(const Event* e)
{
	if (strcmp(e->eventType, "DiamondDeath") != 0)
		return;
}

bool dae::DiamondBlockComponent::DoDamage()
{
	--m_Health;
	if (m_Health > 0)
	{
		return false;
	}


	AddPointsAndNotifyDeath();
	return true;
}

void dae::DiamondBlockComponent::AddPointsAndNotifyDeath()
{
	dae::AddPointsCommand addPointsCommand(m_pPlayer);
	addPointsCommand.Execute();

	Event event;
	event.eventType = "GainDiamond";

	GetOwner()->MarkForDestruction();
}
