#pragma once
#include <memory>

template <typename T>
class Singleton {
public:
	//static T& getInstance() {
	//	static T instance;
	//	return instance;
	//}
	template<typename... Args>
	static T& getInstance(Args&&... args) {
		static T instance(std::forward<Args>(args)...);
		return instance;
	}
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
	Singleton(const Singleton&&) = delete;
	Singleton& operator=(const Singleton&&) = delete;
private:
	Singleton() = default;
	~Singleton() = default;
};