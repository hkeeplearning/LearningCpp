#include "MySubject.h"
#include "MyObserver.h"


MySubject::MySubject(void)
{
}


MySubject::~MySubject(void)
{
}

void MySubject::Subscribe(int publishType, IObserver *observer)
{
    if (nullptr != observer && publishType >= Article && publishType <= Clothes)
    {
        m_observers[publishType].push_back(observer);
    }
}

void MySubject::UnSubscribe(int publishType, IObserver *observer)
{
    if (nullptr != observer && publishType >= Article && publishType <= Clothes)
    {
        auto &observers = m_observers[publishType];
        for (std::list<IObserver *>::iterator it = observers.begin();
            it != observers.end(); ++it)
        {
            if (*it == observer)
            {
                observers.remove(observer);
                break;
            }
        }
    }
}

void MySubject::Notify(int publishType, const std::string &desp)
{
    if (m_observers.find(static_cast<PublishType>(publishType)) != m_observers.end())
    {
        auto &observers = m_observers[publishType];
        for (std::list<IObserver *>::iterator it = observers.begin();
            it != observers.end(); ++it)
        {
            (*it)->OnContentPublish(desp);
        }
    }
}
