#pragma once

#include "IDecorator.h"

class DecoratorC : public IDecorator
{
public:
    DecoratorC(IComponent *cmp);
    ~DecoratorC(void);
    void Operation();

private:
    void AddNewOperator();
};

