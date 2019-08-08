#include "ffmpegplayer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ffmpegPlayer w;
    w.show();

    return a.exec();
}
