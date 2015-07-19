#ifndef MAIN_H
#define MAIN_H
#include <QtCore>
#include <QDebug>
#include <QApplication>
#include "core/graphshell.h"
//#include "gui/graphshellgui.h"

class WorkerThread : public QThread
{
	Q_OBJECT
public:
	WorkerThread(QObject *parent, graphshell::CircularBuffer *buffer) : QThread(parent) { this->buffer = buffer; }
private:
	void run() Q_DECL_OVERRIDE;
	graphshell::CircularBuffer *buffer;
signals:
};


#endif // MAIN_H

