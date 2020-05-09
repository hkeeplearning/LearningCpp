#pragma once

#include "IObserver.h"

class MyObserver : public IObserver
{
public:
    MyObserver(const std::string &name);
    ~MyObserver();
    void OnContentPublish(const std::string &content);

private:
    std::string m_name;
};
