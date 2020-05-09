#include "IDecorator.h"


IDecorator::IDecorator(IComponent *cmp) :
    m_cmp(cmp)
{
}


IDecorator::~IDecorator(void)
{
}

void IDecorator::Operation()
{
    m_cmp->Operation();
}
