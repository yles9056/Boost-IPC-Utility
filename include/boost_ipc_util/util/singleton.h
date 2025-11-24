#pragma once

/*
* Singleton examples:
* https://stackoverflow.com/a/34653512/7006125
* https://stackoverflow.com/a/1008289/7006125
*/

#define SINGLETON(className)\
public:\
    inline static className& getInstance() {\
        static className instance;\
        return instance;\
    }\
    className(className const&) = delete;\
    className(className&&) = delete;\
    className& operator=(className const&) = delete;\
    className& operator=(className&&) = delete;
