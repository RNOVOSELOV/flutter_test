#include <iostream>
// #include "hello.h"
#include "consoleView.h"
#include "model.h"
#include "presenter.h"

int main(int argc, char * argv[])
{

    if (argc <= 1)
    {
        cout << "Arguments error" << endl;
        return 1;
    }

    Presenter* presenter = new Presenter();
    auto isModelInit{ presenter->initModel(argc, argv) };
    if (isModelInit)
    {
        presenter->startAnalyze();
    }
    delete presenter;
    return 0;
}