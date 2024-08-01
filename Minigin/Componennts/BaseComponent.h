#ifndef ENGINE2D_BASECOMPONENT_H
#define ENGINE2D_BASECOMPONENT_H
#include <memory>


namespace dae
{
	class GameObject;

	class BaseComponent
	{
	public:

		virtual ~BaseComponent();

		// Copy and move
		BaseComponent(const BaseComponent& other) = delete;
		BaseComponent(BaseComponent&& other) = delete;
		BaseComponent& operator=(const BaseComponent& other) = delete;
		BaseComponent& operator=(BaseComponent&& other) = delete;



		virtual void Render() const;
		virtual void RenderUI();

		virtual void FixedUpdate(const float fixedTimeStep);
		virtual void Update();


	protected:

		BaseComponent(GameObject* Owner);
		GameObject* GetOwner() const;


	private:

		GameObject* m_pOwner; // Can be a smart pointer --> Preferably a shared_ptr
	};
}

#endif // !ENGINE2D_BASECOMPONENT_H
