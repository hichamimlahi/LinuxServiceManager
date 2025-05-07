#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    app.setApplicationName("Linux Service Manager");
    app.setOrganizationName("Group 5");
    
    MainWindow window;
    window.show();
    
    return app.exec();
}
