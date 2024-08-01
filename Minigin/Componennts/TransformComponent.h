#ifndef ENGINE2D_TRANSFORMCOMPONENT_H
#define ENGINE2D_TRANSFORMCOMPONENT_H
#include "BaseComponent.h"
#include <glm/glm.hpp>


namespace dae
{
	class TransformComponent final : public BaseComponent
	{
	public:

		TransformComponent(GameObject* Owner);
		~TransformComponent() override;

		// Copy and move
		TransformComponent(const TransformComponent& other) = delete;
		TransformComponent(TransformComponent&& other) = delete;
		TransformComponent& operator=(const TransformComponent& other) = delete;
		TransformComponent& operator=(TransformComponent&& other) = delete;



		const glm::vec2& GetLocalPosition() const;
		const glm::vec2& GetWorldPosition();
		void SetLocalPosition(float x, float y);

		void SwitchDirtyFlag(bool IsFlagged);


	private:

		bool m_IsDirty;
		glm::vec2 m_LocalPosition;
		glm::vec2 m_WorldPosition;


		void SetWorldPosition();
	};
}

#endif // ENGINE2D_TRANSFORMCOMPONENT_H