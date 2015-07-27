#ifndef MAIN_H
#define MAIN_H
#include <QtCore>
#include <QDebug>
#include "core/graphshell.h"
#include "gui/graphshellgui.h"

class WorkerThread : public QThread
{
	Q_OBJECT
public:
	WorkerThread(QObject *parent, graphshell::CircularBuffer *buffer) : QThread(parent) { this->buffer = buffer; }
private:
    void run() Q_DECL_OVERRIDE {
        QFile in("./ipsum.txt");
        if (!in.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << "Error !" << in.errorString();
            return;
        }

        char *buf = new char[1025];
        int total = 0;
        while (!in.atEnd()) {
            int cnt = in.read(buf, 1024);
            buffer->write(buf, cnt);
            qDebug() << "written "<< cnt << " to buffer";
            total += cnt;
        }
        in.close();
        buffer->setEndOfStream();
        qDebug() << "Finished writing. Total : "<< total;
    }
	graphshell::CircularBuffer *buffer;
signals:
};


#endif // MAIN_H

