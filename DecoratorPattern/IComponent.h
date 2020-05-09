#pragma once

class IComponent
{
public:
    virtual ~IComponent(void);
    virtual void Operation() = 0;

protected:
    IComponent(void);
};