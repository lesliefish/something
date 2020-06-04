#include "widget.h"
#include <QApplication>
#include <xcb/xcb.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <iostream>
#include <QFile>
#include <string>
#include <QString>
#include <QTimer>

using namespace  std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.setWindowTitle("XXXXXXXXXXXXXXXXXXXXXXX");
    w.show();

    auto display = XOpenDisplay(nullptr);
    auto root_window = DefaultRootWindow(display);

    Window root_return, parent_return;
    Window * child_list = nullptr;
    unsigned int child_num = 0;
    XQueryTree(display, root_window, &root_return, &parent_return, &child_list, &child_num);

    for(unsigned int i = 0; i < child_num; ++i) {
        auto window = child_list[i];
        XWindowAttributes attrs;
        XGetWindowAttributes(display, window, &attrs);

        std::cout << "#" << window <<":" << "(" << attrs.width << ", " << attrs.height << ")"  << attrs.x << "_" << attrs.y << std::endl;
    }



    XReparentWindow(display,w.winId(), 27271061,200,0);

    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&]
    {
        //XGetWindowAttributes(display, window, &attrs);
    });

    XFree(child_list);
    XCloseDisplay(display);

    return a.exec();
}
