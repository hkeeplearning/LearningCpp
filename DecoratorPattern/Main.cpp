#include "DecoratorA.h"
#include "DecoratorB.h"
#include "DecoratorC.h"
#include "ConcreteComponent.h"

int main()
{
    auto comp = new ConcreteComponent();
    auto compA = new DecoratorA(comp);
    auto compB = new DecoratorB(compA);
    auto compC = new DecoratorC(compB);
    compC->Operation();

    return 0;
}