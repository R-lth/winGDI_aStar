#pragma once

template <typename T>
class Singleton 
{
protected:
    Singleton() = default;
    ~Singleton() = default;

public:
    static T& Get()
    {
        static T instance; 
        return instance; 
    }

public:
    // 복사 생성자 · 대입 방지
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    // 이동 생성자 · 대입 방지
    Singleton(Singleton&&) = delete;
    Singleton& operator=(Singleton&&) = delete;
};
