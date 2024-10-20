// 在头文件中定义// my_library.hpp
#ifndef MY_LIBRARY_HPP
#define MY_LIBRARY_HPP

#ifdef _WIN32
    #ifdef MY_LIBRARY_EXPORTS
        #define MY_LIBRARY_API __declspec(dllexport)
    #else
        #define MY_LIBRARY_API __declspec(dllimport)
    #endif
#else
    #define MY_LIBRARY_API
#endif

class MY_LIBRARY_API MyClass {
public:
    void doSomething();
};

#endif // MY_LIBRARY_HPP
#ifdef _WIN32
    #ifdef MYLIBRARY_EXPORTS
        #define MYLIBRARY_API __declspec(dllexport)
    #else
        #define MYLIBRARY_API __declspec(dllimport)
    #endif
#else
    #define MYLIBRARY_API __attribute__((visibility("default")))
#endif

