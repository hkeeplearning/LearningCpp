#pragma once

#include "ISubject.h"

#include <list>
#include <map>

class MySubject : public ISubject
{
public:
    enum PublishType{
        Article,
        Book,
        Clothes,
    };
    MySubject();
    ~MySubject();
    void Subscribe(int publishType, IObserver *observer);
    void UnSubscribe(int publishType, IObserver *observer);
    void Notify(int publishType, const std::string &desp);

private:
    std::map<int, std::list<IObserver *>> m_observers;
};

