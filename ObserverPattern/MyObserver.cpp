#include "MyObserver.h"

#include <iostream>

MyObserver::MyObserver(const std::string &name) : m_name(name)
{
}


MyObserver::~MyObserver(void)
{
}

void MyObserver::OnContentPublish(const std::string &content)
{
    std::cout << m_name << " received the update content\n";
    std::cout << content << "\n\n";
}