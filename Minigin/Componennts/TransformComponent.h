#ifndef ENGINE2D_TRANSFORMCOMPONENT_H
#define ENGINE2D_TRANSFORMCOMPONENT_H
#include "BaseComponent.h"
#include <glm/glm.hpp>
#include <string>

#include <unordered_map>

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

		void SetLastMovementDirection(const std::string& direction);
		const std::string GetLastMovementDirection() const;



		void BlockDirection(const std::string& direction);
		bool IsDirectionBlocked(const std::string& direction) const;
		void ResetBlockedDirections();


	private:

		bool m_IsDirty;
		glm::vec2 m_LocalPosition;
		glm::vec2 m_WorldPosition;

		std::string m_LastMovementDirection{};


		void SetWorldPosition();



		std::unordered_map<std::string, bool> m_BlockedDirections;

	};
}

#endif // ENGINE2D_TRANSFORMCOMPONENT_H