#include "graphshell.h"
#include "sockets/controlsockets.h"

namespace graphshell {
using namespace graphshell::sockets;

/*!
 * \brief Creates a new Box, adds the general input (start, stop) and output (started, stopped) control (signal) sockets.
 * \param typeName Box::typeName
 * \param autostart Box::autostart
 */
Box::Box(QString typeName, bool autostart) :
    QThread()
{
    this->typeName = typeName;
    setObjectName(typeName);
    this->autostart = autostart;
    shell = nullptr;
    InputSignalSocket *start = new InputSignalSocket(DataType::getType("/signal/"), "start");
    InputSignalSocket *stop = new InputSignalSocket(DataType::getType("/signal/"), "stop");
    OutputSignalSocket *started = new OutputSignalSocket(DataType::getType("/signal/void/"), "started", false, new QVariant());
    OutputSignalSocket *stopped = new OutputSignalSocket(DataType::getType("/signal/void/"), "stopped", false, new QVariant());

    connect(start, &InputSignalSocket::valueReceived, this, &Box::startBox);
    connect(stop, &InputSignalSocket::valueReceived, this, &Box::stopBox);

    connect(this, &Box::started, started, &OutputSignalSocket::sendVoid);
    connect(this, &Box::finished, stopped, &OutputSignalSocket::sendVoid);

    addSocket(*start, CONTROL);
    addSocket(*stop, CONTROL);

    addSocket(*started, CONTROL);
    addSocket(*stopped, CONTROL);
}

/*!
 * \brief Sets this Box's parent shell to the given GraphSell. Note that, for threading
 * reasons, this will not call QObject::setParent(). This causes a fatal error if the Box
 * already had a parent.
 * \param shell The new parent.
 */
void Box::setParent(GraphShell &shell)
{
    if (this->shell == nullptr)
        this->shell = &shell;
    else if (this->shell == &shell)
        qWarning() << "Box "<< objectName() <<"'s parent has already been set";
    else
        qFatal("Cannot change a box's parent");
}

Box::~Box()
{
}
void Box::startBox() {
    qDebug() << this << " start requested";
    moveToThread(this);
    this->start();
}

void Box::stopBox() {
    qDebug() << this << " stop requested";
    this->exit();
}

// ############ ADD ##############
void Box::addInputSocket(InputSocket &socket, unsigned int flags)
{
    QHash<QString, InputSocket*> &map = ((flags&Box::DATA) > 0 ? dataInput: controlInput);
    if (map.value(socket.objectName()) == &socket) {
        qWarning() << "Socket "<< &socket <<" has already been added to its parent box";
    } else {
        if (map.contains(socket.objectName())) {
            removeSocket(*map[socket.objectName()], flags);
        }
        map[socket.objectName()] = &socket;
        socket.setParent(*this);

        emit socketAdded(socket, flags);
    }
}


void Box::addOutputSocket(OutputSocket &socket, unsigned int flags)
{
    QHash<QString, OutputSocket*> &map = ((flags&Box::DATA) > 0 ? dataOutput : controlOutput);
    if (map.value(socket.objectName()) == &socket) {
        qWarning() << "Socket "<< &socket <<" has already been added to its parent box";
    } else {
        if (map.contains(socket.objectName())) {
            removeSocket(*map[socket.objectName()], flags);
        }
        map[socket.objectName()] = &socket;
        socket.setParent(*this);
        emit socketAdded(socket, flags);
    }
}

/*!
 * \brief Removes a socket from the box according to flags. The socket is deleted after this.
 * \param sock the Socket to add
 * \param flags sets whether the socket is data (Box::DATA) or control (Box::CONTROL).
 * Box::INPUT and Box::OUTPUT flags are ignored and deduced from Socket::isInput()
 * \return true if the socket was successfully removed (i.e if it was actually present)
 */
bool Box::removeSocket(Socket &sock, unsigned int flags)
{
    if (sock.isInput())
        return removeInputSocket((InputSocket&)sock, (flags | INPUT) & ~OUTPUT);
    else
        return removeOutputSocket((OutputSocket&)sock, (flags | OUTPUT) & ~INPUT);
    delete &sock;
}

bool Box::removeInputSocket(InputSocket &socket, unsigned int flags)
{
    QHash<QString, InputSocket*> &map = ((flags&Box::DATA) > 0 ? dataInput : controlInput);
    if (!map.contains(socket.objectName()))
        qWarning() << "Attempt at removing an absent socket";
    else {
        map.remove(socket.objectName());
        emit socketRemoved(socket);
        return true;
    }
    return false;
}

bool Box::removeOutputSocket(OutputSocket &socket, unsigned int flags)
{
    QHash<QString, InputSocket*> &map = ((flags&Box::DATA) > 0 ? dataInput : controlInput);
    if (!map.contains(socket.objectName()))
        qWarning() << "Attempt at removing an absent socket";
    else {
        map.remove(socket.objectName());
        emit socketRemoved(socket);
        return true;
    }
    return false;
}


}
