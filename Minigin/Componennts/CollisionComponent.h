#ifndef ENGINE2D_COLLISIONCOMPONENT_H
#define ENGINE2D_COLLISIONCOMPONENT_H
#include "BaseComponent.h"
#include "glm/glm.hpp"
#include "Scene/Scene.h"
#include <functional>


// Based on someone elses (i'll try to find their name soon)
namespace dae
{
	struct CollisionData
	{
		std::string ownerType{ "" };
		GameObject* owningObject{};
	};

	class TransformComponent;


	class CollisionComponent final : public BaseComponent
	{
	public:

		CollisionComponent(GameObject* Owner);
		~CollisionComponent() override;

		// Copy and move
		CollisionComponent(const CollisionComponent& other) = delete;
		CollisionComponent(CollisionComponent&& other) = delete;
		CollisionComponent& operator=(const CollisionComponent& other) = delete;
		CollisionComponent& operator=(CollisionComponent&& other) = delete;




		CollisionData GetCollisionData() const;
		void SetCollisionData(CollisionData data);

		void SetCallback(std::function<void(const CollisionData& owner, const CollisionData& hitObject)> callbackFunc);

		void SetActive(bool isActive);
		bool IsActive();



		void SetScene(Scene* scene);
		void IsOverlappingOtherCollision(const std::vector<CollisionComponent*>& collisionsToCheck) const;
		void SetBounds(float width, float height);
		glm::vec2 GetBounds();

		void Render() const override;


	private:

		TransformComponent* m_pTransform;
		float m_width;
		float m_height;

		CollisionData m_CollisionData;

		Scene* m_pScene;

		bool m_IsActive;


		bool CheckOverlapp(CollisionComponent* pOther) const;
		std::function<void(const CollisionData& collisionOwner, const CollisionData& hitObject)> m_OnHitCallback{};
	};
}
#endif // ENGINE2D_COLLISIONCOMPONENT_H