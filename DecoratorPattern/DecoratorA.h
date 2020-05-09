#pragma once

#include "IDecorator.h"

class DecoratorA : public IDecorator
{
public:
    DecoratorA(IComponent *cmp);
    virtual ~DecoratorA(void);
    void Operation();

private:
    void AddNewOperator();
};

