#pragma once

#include "IDecorator.h"

class DecoratorB : public IDecorator
{
public:
    DecoratorB(IComponent *cmp);
    ~DecoratorB(void);
    void Operation();

private:
    void AddNewOperator();
};

