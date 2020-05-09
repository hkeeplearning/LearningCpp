#include "MyObserver.h"
#include "MySubject.h"

#include <iostream>

int main()
{
    MySubject subj;
    
    MyObserver obs1("abc");
    subj.Subscribe(MySubject::Article, &obs1);
    subj.Subscribe(MySubject::Clothes, &obs1);

    MyObserver obs2("123");
    subj.Subscribe(MySubject::Article, &obs2);
    subj.Subscribe(MySubject::Book, &obs2);
    subj.Subscribe(MySubject::Clothes, &obs2);

    subj.Notify(MySubject::Article, "This is an article push.");
    subj.Notify(MySubject::Book, "This is a book push.");
    subj.Notify(MySubject::Clothes, "This is clothes push.");

    subj.UnSubscribe(MySubject::Clothes, &obs2);
    std::cout << "###\n\n";
    subj.Notify(MySubject::Article, "This is an article push.");
    subj.Notify(MySubject::Book, "This is a book push.");
    subj.Notify(MySubject::Clothes, "This is clothes push.");

    subj.UnSubscribe(MySubject::Article, &obs1);
    subj.UnSubscribe(MySubject::Clothes, &obs1);
    std::cout << "###\n\n";
    subj.Notify(MySubject::Article, "This is an article push.");
    subj.Notify(MySubject::Book, "This is a book push.");
    subj.Notify(MySubject::Clothes, "This is clothes push.");

    return 0;
}