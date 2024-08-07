#include "DiamondBlockComponent.h"
#include "events/Event.h"
#include "Commands/AddPointsCommand.h"
#include "GameObject.h"


dae::DiamondBlockComponent::DiamondBlockComponent(GameObject* owner)
	: BlockComponent(owner)
	, m_Health{ 1 }
{

}

void dae::DiamondBlockComponent::Initialize(std::vector<dae::GameObject*> players)
{
	m_pPlayer = players;
}

void dae::DiamondBlockComponent::OnHitCallback(const CollisionData& /*collisionOwner*/, const CollisionData& hitObject)
{
	if (hitObject.ownerType != "DiamondAttack")
		return;

	DoDamage(hitObject.owningObject);
}

void dae::DiamondBlockComponent::OnDeath(const Event* e)
{
	if (strcmp(e->eventType, "DiamondDeath") != 0)
		return;

	NotifyOnDeath();
}

bool dae::DiamondBlockComponent::DoDamage(GameObject* player)
{
	--m_Health;

	if (m_Health > 0)
		return false;

	AddPointsToPlayers(player);
	NotifyOnDeath();
	return true;
}

void dae::DiamondBlockComponent::AddPointsToPlayers(GameObject* player)
{
	dae::AddPointsCommand addPointsCommand(player);
	addPointsCommand.Execute();
}

void dae::DiamondBlockComponent::NotifyOnDeath()
{
	Event event;
	event.eventType = "GainDiamond";

	GetOwner()->MarkForDestruction();
}
