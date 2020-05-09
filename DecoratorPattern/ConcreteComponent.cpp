#include "ConcreteComponent.h"
#include <iostream>

ConcreteComponent::ConcreteComponent(void)
{
}


ConcreteComponent::~ConcreteComponent(void)
{
}

void ConcreteComponent::Operation()
{
    std::cout << "ConcreteComponent is created.\n";
}
