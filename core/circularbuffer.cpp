#include "circularbuffer.h"

namespace graphshell {

/*!
 * \brief Creates a new CircularBuffer with internal capacity buffersize.
 * \param parent the parent QObject of this buffer
 * \param buffersize Internal buffer size
 */

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

/*!
 * \brief Blocks the current thread at most msecs waiting for data available to read
 * \param msecs The max time in miliseconds to wait for data. Beyond that, the function
 * will timeout and return false.
 * \return True if some data became available before timeout, false otherwise.
 */
bool CircularBuffer::waitForReadyRead(int msecs) {
    if (!isOpen())
        return false;
    QMutexLocker locker(&mutex);
    if (usedSize == 0)
        usedCond.wait(&mutex, (msecs > 0 ? (unsigned long) msecs : ULONG_MAX));
    return isOpen() && usedSize > 0;
}

/*!
 * \brief Blocks the current thread at most msecs waiting for all data in the
 * buffer to be freed
 * \param msecs The max time in miliseconds to wait. Beyond that, the function
 *  will timeout and return false.
 * \return True if all data was written before the timeout, false otherwise.
 */
bool CircularBuffer::waitForBytesWritten(int msecs) {
    if (!isOpen())
        return false;
    QMutexLocker locker(&mutex);
    if (usedSize > 0)
        freeCond.wait(&mutex, (msecs > 0 ? (unsigned long) msecs : ULONG_MAX));
    return isOpen() && usedSize == 0;
}

/*!
 * \brief Implements QIODevice::readData(). Reads as much data as possible until 'count'
 * \param to Buffer to write data to
 * \param count max data to be read.
 * \return Actual number of bytes read. If less than count, no more data was available,
 * which means the buffer was closed.
 */
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

/*!
 * \brief Reads as much data as possible until either 'count' characters is reached,
 * or 'delimiter' is found
 * \param to Buffer to write data to
 * \param count max data to be read.
 * \param delimiter
 * \return Actual number of bytes read. If less than count, that means either delimiter
 * was found or the buffer was closed.
 */
qint64 CircularBuffer::readUntil(char to[], int count, char delimiter) {
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

/*!
 * \brief Implemnets QIODevice::writeData. Write as much data as possible to the buffer
 * \param from Char buffer to get data from
 * \param count Max data to write to the buffer
 * \return Actual number of chars written. If less than count, that means the buffer was closed
 */
qint64 CircularBuffer::writeData(const char *from, qint64 count) {
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


/*!
 * \brief Implmeents QIODevice::open marks the buffer as readable/writable. Also resets
 * the buffer to an empty state.
 * \param mode This parameter is ignored. The buffer is systematically opened with
 * ReadWrite | Text | Unbuffered
 * \return false if already open, true otherwise.
 */
bool CircularBuffer::open(OpenMode mode)
{

    if (isOpen())
        return false;
    Q_UNUSED(mode)
    readHead = writeHead = 0;
    endHead = ULONG_MAX;
    freeSize = size;
    usedSize = 0;
    setOpenMode(ReadWrite | Text | Unbuffered);
    return true;
}

/*!
 * \brief Implements QIODevice::close. Sets the openMode to NotOpen, then wakes up all
 * waiting threads. This should *ONLY* be called from the reading thread. The writing thread
 * should call setEndOfStream() instead, to let the reader read any data left before closing.
 */
void CircularBuffer::close()
{
    if (isOpen()) {
        setOpenMode(NotOpen);
        freeCond.wakeAll();
        usedCond.wakeAll();
    }
}

/*!
 * \brief Implements QIODevice::atEnd. Tests whether we're at the end of the stream.
 * \return True if the readHead is currently at the end of the readable data. See setEndOfStream()
 */
bool CircularBuffer::atEnd() const {
    return endHead <= readHead;
}

/*!
 * \brief Marks the current writeHead as the end of data for this stream. This allows the buffer to
 * act like a file and notify the reading thread when whatever was producing data is done doing so.
 * No data should be written past this.
 */
void CircularBuffer::setEndOfStream()
{
    QMutexLocker locker(&mutex);
    endHead = writeHead;
    usedCond.wakeAll();
    qDebug() << "Setting endHead to "<< endHead <<"("<< writeHead <<")";
}

/*!
 * \brief Implements QIODevice::readLineData. Reads data (at most maxlen) until a \n is found.
 * \param data Buffer to write data to
 * \param maxlen Maximum number of chars to read
 * \return actual number of chars read
 */
qint64 CircularBuffer::readLineData(char *data, qint64 maxlen)
{
    return readUntil(data, maxlen);
}

}
