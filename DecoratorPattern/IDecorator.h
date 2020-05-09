#pragma once

#include "IComponent.h"

class IDecorator : public IComponent
{
public:
    IDecorator(IComponent *cmp);
    virtual ~IDecorator(void);
    void Operation();

protected:
    IComponent *m_cmp;
};

