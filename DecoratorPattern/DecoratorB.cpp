#include "DecoratorB.h"
#include <iostream>


DecoratorB::DecoratorB(IComponent *cmp) :
    IDecorator(cmp)
{
}


DecoratorB::~DecoratorB(void)
{
}

void DecoratorB::Operation()
{
    m_cmp->Operation();
    AddNewOperator();
}

void DecoratorB::AddNewOperator()
{
    std::cout << "DecoratorB::NewOperator\n";
}
