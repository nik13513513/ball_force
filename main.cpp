#include "widget.h"
#include "view.h"
#include "ball_item.h"

#include <QApplication>
#include <QTime>
#include <QDebug>
#include <QMainWindow>

#include <thread>


void forceAction(GraphicsView* _view)
{
    while (true){
        _view->calculateForces();

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    Widget *widget = new Widget();
    widget->show();

    std::thread forceThread(forceAction,widget->view());
    forceThread.detach();

    return app.exec();
}
