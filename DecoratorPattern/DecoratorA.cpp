#include "DecoratorA.h"
#include <iostream>


DecoratorA::DecoratorA(IComponent *cmp) :
    IDecorator(cmp)
{
}


DecoratorA::~DecoratorA(void)
{
}

void DecoratorA::Operation()
{
    m_cmp->Operation();
    AddNewOperator();
}

void DecoratorA::AddNewOperator()
{
    std::cout << "DecoratorA::NewOperator\n";
}
