#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H
#include "sockets.h"
#include <QDebug>

namespace graphshell {


//########## CIRCULAR BUFFER ##########
class CircularBuffer : public QIODevice
{
    Q_OBJECT
public:
    explicit CircularBuffer(QObject *parent, quint32 buffersize);
    bool waitForReadyRead(int msecs);
    bool waitForBytesWritten(int msecs);
    qint64 readUntil(char[], int count, char delimiter='\n');
    bool open(OpenMode mode = ReadWrite) Q_DECL_OVERRIDE;
    void close() Q_DECL_OVERRIDE;
    qint64 bytesAvailable() const Q_DECL_OVERRIDE { return usedSize;}
    qint64 bytesToWrite() const Q_DECL_OVERRIDE { return usedSize; }
    void setEndOfStream();
    bool atEnd() const Q_DECL_OVERRIDE;
protected:
    qint64 readData(char *data, qint64 maxlen) Q_DECL_OVERRIDE;
    qint64 readLineData(char *data, qint64 maxlen) Q_DECL_OVERRIDE;
    qint64 writeData(const char *data, qint64 maxlen) Q_DECL_OVERRIDE;
    bool isSequential() const Q_DECL_OVERRIDE {return true;}
private:
    quint32 size;
    quint64 readHead;
    quint64 writeHead;
    quint64 endHead;
    char *buffer;
    quint64 freeSize;
    QWaitCondition freeCond;
    quint64 usedSize;
    QWaitCondition usedCond;
    QMutex mutex;
public slots:
signals:
};


}

#endif // CIRCULARBUFFER_H
