#ifndef ENGINE2D_ADDPOINTSCOMPONENTCOMPONENT_H
#define ENGINE2D_ADDPOINTSCOMPONENTCOMPONENT_H
#include "BaseComponent.h"

namespace dae
{

	class AddPointsComponnent final : public BaseComponent
	{
	public:

		AddPointsComponnent(GameObject* owner);
		~AddPointsComponnent();

		AddPointsComponnent(const AddPointsComponnent& other) = delete;
		AddPointsComponnent(AddPointsComponnent&& other) = delete;
		AddPointsComponnent& operator=(const AddPointsComponnent& other) = delete;
		AddPointsComponnent& operator=(AddPointsComponnent&& other) = delete;




		void SetPlayerIndex(unsigned int playerIndex);
		void AddPoints();


	private:

		unsigned int m_PlayerIndex;
	};
}

#endif // !ENGINE2D_ADDPOINTSCOMPONENTCOMPONENT_H