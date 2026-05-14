#ifndef SINGLETON_H
#define SINGLETON_H

template<typename T>
class Singleton {
public:
    static T& getInstance() {
        static T instance;
        return instance;
    }
protected:
    Singleton() = default;
    Singleton(const Singleton<T> &) = delete;
    Singleton& operator=(const Singleton<T> &) = delete;
};

#endif // SINGLETON_H
