#include <QApplication>

#include "controller/MainController.hpp"





int main(int argc,
         char* argv[])
{
    QApplication application(argc, argv);
    
    MainController controller;
    controller.start();
    
    return application.exec();
}
