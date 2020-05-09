#pragma once

#include <string>

class IObserver
{
public:
    virtual ~IObserver();
    virtual void OnContentPublish(const std::string &content) = 0;

protected:
    IObserver();
};

