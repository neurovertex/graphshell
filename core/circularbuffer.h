#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H
#include "sockets.h"
#include <QDebug>

namespace graphshell {


//########## CIRCULAR BUFFER ##########
/*!
 * \brief Implements a semaphore-guarded circular char buffer. Can be used to
 * send a stream of data from a thread to another. Note that while this is
 * supposedly safe to use with a reader and a writer thread, Its safety isn't
 * guaranteed for more than one reader thread or more than one writer thread.
 * Additionally, this class implements Qt's QIODevice interface, thus can be
 * used like a file, sort of. To this purpose, it has an opening/closing mechanism.
 */
class CircularBuffer : public QIODevice
{
    Q_OBJECT
public:
    explicit CircularBuffer(QObject *parent, quint32 buffersize);
    bool waitForReadyRead(int msecs);
    bool waitForBytesWritten(int msecs);
    qint64 readUntil(char to[], int count, char delimiter='\n');
    bool open(OpenMode mode = ReadWrite) Q_DECL_OVERRIDE;
    void close() Q_DECL_OVERRIDE;
    /*! \return How many bytes are currently in the buffer waiting to be read.
     * \note This is the same as CircularBuffer::bytesToWrite(), but they implement different
     * methods from QIODevice. */
    qint64 bytesAvailable() const Q_DECL_OVERRIDE { return usedSize;}
    /*! \return How many bytes are currently in the buffer waiting to be read.
     * \sa CircularBuffer::bytesAvailable() */
    qint64 bytesToWrite() const Q_DECL_OVERRIDE { return usedSize; }
    void setEndOfStream();
    bool atEnd() const Q_DECL_OVERRIDE;
protected:
    qint64 readData(char *data, qint64 maxlen) Q_DECL_OVERRIDE;
    qint64 readLineData(char *data, qint64 maxlen) Q_DECL_OVERRIDE;
    qint64 writeData(const char *data, qint64 maxlen) Q_DECL_OVERRIDE;
    /*! \brief Implements QIODevice::isSequential to let know that this is, indeed, a sequential device
     * \return true */
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
