#ifndef ENGINE2D_SINGLETON_H
#define ENGINE2D_SINGLETON_H
namespace dae
{
	template <typename T>
	class Singleton
	{
	public:
		static T& GetInstance()
		{
			static T instance{};
			return instance;
		}

		virtual ~Singleton() = default;
		Singleton(const Singleton& other) = delete;
		Singleton(Singleton&& other) = delete;
		Singleton& operator=(const Singleton& other) = delete;
		Singleton& operator=(Singleton&& other) = delete;

	protected:
		Singleton() = default;
	};
}
#endif // !ENGINE2D_SINGLETON_H