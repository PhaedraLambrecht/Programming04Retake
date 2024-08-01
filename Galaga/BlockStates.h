#pragma once

namespace dae
{
	class BlockStates final
	{
	public:

		BlockStates();
		~BlockStates() = default;

		BlockStates(const BlockStates& other) = delete;
		BlockStates(BlockStates&& other) = delete;
		BlockStates& operator=(const BlockStates& other) = delete;
		BlockStates& operator=(BlockStates&& other) = delete;

		void Update();
		
	};
}

