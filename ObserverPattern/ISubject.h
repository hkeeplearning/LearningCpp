#pragma once

#include <string>

class IObserver;

class ISubject
{
public:
    virtual ~ISubject();
    virtual void Subscribe(int publishType, IObserver *observer) = 0;
    virtual void UnSubscribe(int publishType, IObserver *observer) = 0;
    virtual void Notify(int publishType, const std::string &desp) = 0;

protected:
    ISubject();
};

