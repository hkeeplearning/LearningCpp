#include "DecoratorC.h"
#include <iostream>

DecoratorC::DecoratorC(IComponent *cmp) :
    IDecorator(cmp)
{
}


DecoratorC::~DecoratorC(void)
{
}

void DecoratorC::Operation()
{
    m_cmp->Operation();
    AddNewOperator();
}

void DecoratorC::AddNewOperator()
{
    std::cout << "DecoratorC::NewOperator\n";
}
