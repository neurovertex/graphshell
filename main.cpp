#include "main.h"

using namespace graphshell;

void WorkerThread::run() {
    QFile in("./ipsum.txt");
    if (!in.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error !" << in.errorString();
        return;
    }

    char *buf = new char[1025];
    char delimiter = '\n';
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


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    /*MainWindow w;
    w.show();*/

    /*GraphManager *manager = new GraphManager(&a);
    qDebug() << manager->newGraph("YourMom");
    foreach (GraphShell *shell, manager->getGraphs()) {
        qDebug() << shell;
    }

    delete manager;*/

    CircularBuffer buffer(&a, 512);
    buffer.open();
    WorkerThread *thread = new WorkerThread(&a, &buffer);
    char *buf = new char[129];
    qint64 total = 0;
    QFile out("./ipsum_out.txt");
    out.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
    thread->start();

    while (!buffer.atEnd()) {
        if (!buffer.waitForReadyRead(-1))
            continue;
        QByteArray line = buffer.readLine();
        qDebug() << "Read "<< line.length() <<" chars";
        out.write(line);
        out.putChar('\n');
        total += line.length()+1;
    }

    buffer.close();

    qDebug() << "Total read : "<< total;

    a.quit();
    return 0;
    //return a.exec();
}
