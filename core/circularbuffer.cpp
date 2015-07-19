#include "circularbuffer.h"

namespace graphshell {


CircularBuffer::CircularBuffer(QObject *parent, quint32 buffersize) :
    QIODevice(parent)
{
    setOpenMode(NotOpen);
    size = buffersize;
    readHead = writeHead = 0;
    endHead = ULONG_MAX;
    buffer = new char[size];
    freeSize = size;
    usedSize = 0;
}

bool CircularBuffer::waitForReadyRead(int msecs) {
    if (!isOpen())
        return false;
    QMutexLocker locker(&mutex);
    if (usedSize == 0)
        usedCond.wait(&mutex, (msecs > 0 ? (unsigned long) msecs : ULONG_MAX));
    return isOpen() && usedSize > 0;
}

bool CircularBuffer::waitForBytesWritten(int msecs) {
    if (!isOpen())
        return false;
    QMutexLocker locker(&mutex);
    if (usedSize > 0)
        freeCond.wait(&mutex, (msecs > 0 ? (unsigned long) msecs : ULONG_MAX));
    return isOpen() && usedSize == 0;
}

qint64 CircularBuffer::readData(char *to, qint64 count) {
    if (!isOpen())
        return -1;
    QMutexLocker locker(&mutex);
    int total = 0;
    while (isOpen() && !atEnd() && total < count) {
        while (usedSize <= 0 && isOpen() && !atEnd()) {
            usedCond.wait(&mutex);
        }
        if (usedSize <= 0)
            continue;
        qint64 toread = std::min(usedSize, (quint64)(count-total));
        usedSize -= toread;

        for (int i = 0; i < toread; i ++) {
            to[i+total] = buffer[(i+readHead)%size];
        }
        total += toread;
        readHead += toread;
        freeSize += toread;
        freeCond.wakeAll();
        qDebug() << "Read "<< total <<" readHead : "<< readHead << " usedSize : "<< usedSize;
    }
    emit bytesWritten(total);
    return total;
}

qint64 CircularBuffer::readUntil(char *to, int count, char delimiter) {
    if (!isOpen())
        return -1;
    QMutexLocker locker(&mutex);
    int total = 0;
    bool delimit = false;
    while (isOpen() && !atEnd() && total < count && !delimit) {
        while (usedSize <= 0 && isOpen()) {
            usedCond.wait(&mutex);
        }
        qint64 toread = std::min(usedSize, (quint64)(count-total));
        usedSize -= toread;

        int i;
        for (i = 0; i < toread && !delimit;) {
            to[i+total] = buffer[(i+readHead)%size];
            if (to[i+total] == delimiter)
                delimit = true;
            i++;
        }

        usedSize += toread - i; // Restore whatever wasn't actually read
        if (i > 0) { // Only really useful to avoid wakeAll but might as well wrap all of that
            total += i;
            readHead += i;
            freeSize += i;
            freeCond.wakeAll();
        }
    }
    emit bytesWritten(total);
    if (delimit)
        total --; // Pretend we didn't write the delimiter to the buffer
    return total;
}


qint64 CircularBuffer::writeData(const char *from, qint64 count) {
    endHead = size+1;
    if (!isOpen())
        return -1;
    QMutexLocker locker(&mutex);
    int total = 0;
    while (isOpen() && total < count) {
        while (freeSize <= 0 && isOpen()) {
            freeCond.wait(&mutex);
        }
        qint64 towrite = std::min(freeSize, (quint64)(count-total));
        freeSize -= towrite;

        for (int i = 0; i < towrite; i ++) {
            buffer[(i+writeHead)%size] = from[i+total];
        }

        total += towrite;
        writeHead += towrite;
        usedSize += towrite;
        usedCond.wakeAll();
    }
    emit readyRead();
    qDebug() << "Wrote "<< total <<" writeHead : "<< writeHead;
    return total;
}


bool CircularBuffer::open(OpenMode mode)
{
    if (!isOpen())
        setOpenMode(ReadWrite | Text | Unbuffered);
    return true;
}

void CircularBuffer::close()
{
    if (isOpen()) {
        setOpenMode(NotOpen);
        freeCond.wakeAll();
        usedCond.wakeAll();
    }
}

bool CircularBuffer::atEnd() const {
    return endHead == readHead;
}

void CircularBuffer::setEndOfStream()
{
    QMutexLocker locker(&mutex);
    endHead = writeHead;
    usedCond.wakeAll();
    qDebug() << "Setting endHead to "<< endHead <<"("<< writeHead <<")";
}

qint64 CircularBuffer::readLineData(char *data, qint64 maxlen)
{
    return readUntil(data, maxlen);
}

}
