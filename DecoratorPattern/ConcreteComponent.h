#pragma once

#include "IComponent.h"

class ConcreteComponent : public IComponent
{
public:
    ConcreteComponent(void);
    virtual ~ConcreteComponent(void);
    void Operation();
};

