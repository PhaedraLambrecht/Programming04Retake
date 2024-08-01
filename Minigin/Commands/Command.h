#ifndef ENGINE2D_BASECOMMAND_H
#define ENGINE2D_BASECOMMAND_H

namespace dae
{
	class BaseCommand
	{
	public:

		explicit BaseCommand() = default;
	//	virtual ~BaseCommand() = default;
		virtual ~BaseCommand();

		BaseCommand(const BaseCommand& other) = delete;
		BaseCommand(BaseCommand&& other) = delete;
		BaseCommand& operator=(const BaseCommand& other) = delete;
		BaseCommand& operator=(BaseCommand&& other) = delete;




		virtual void Execute() = 0;
	};

}

#endif //ENGINE2D_BASECOMMAND_H