#include "graphshell.h"
#include "sockets/controlsockets.h"

namespace graphshell {
using namespace graphshell::sockets;
/*!
 * \class Box
 * \brief Base class for the nodes of the execution graph
 * \ingroup Core
 * \ingroup Boxes
 *
 * This class is the basis of all processing in GraphShell. It has I/O sockets for
 * exchanging information with other boxes. Sockets are divided into 2 categories :
 * "Data" Input/Output sockets are for the process of the box to consume/process/produce
 * "Control" Input/Output sockets are "meta" to the box : they control its execution
 * and give information about it to other boxes. Although this is a purely conceptual
 * difference, in practice boxes may  be redefined to use either as desired.
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

    addSocket(start, CONTROL);
    addSocket(stop, CONTROL);

    addSocket(started, CONTROL);
    addSocket(stopped, CONTROL);
}

void Box::setParent(GraphShell *shell)
{
    if (this->shell == nullptr)
        this->shell = shell;
    else if (this->shell == shell)
        qWarning() << "Box "<< objectName() <<"'s parent has already been set";
    else
        qFatal("Cannot change a box's parent");
}

Box::~Box()
{
}

void Box::startBox() {
    this->start();
}

void Box::stopBox() {
    this->exit();
}


// ############ ADD ##############
void Box::addInputSocket(InputSocket *socket, int flags)
{
    QHash<QString, InputSocket*> &map = ((flags&Box::DATA) > 0 ? dataInput: controlInput);
    if (map.value(socket->objectName()) == socket) {
        qWarning() << "Socket "<< socket <<" has already been added to its parent box";
    } else {
        if (map.contains(socket->objectName())) {
            removeSocket(map[socket->objectName()], flags);
        }
        map[socket->objectName()] = socket;
        socket->setParent(this);
        emit socketAdded(socket, flags);
    }
}


void Box::addOutputSocket(OutputSocket *socket, int flags)
{
    QHash<QString, OutputSocket*> &map = ((flags&Box::DATA) > 0 ? dataOutput : controlOutput);
    if (map.value(socket->objectName()) == socket) {
        qWarning() << "Socket "<< socket <<" has already been added to its parent box";
    } else {
        if (map.contains(socket->objectName())) {
            removeSocket(map[socket->objectName()], flags);
        }
        map[socket->objectName()] = socket;
        socket->setParent(this);
        emit socketAdded(socket, flags);
    }
}

bool Box::removeSocket(InputSocket *socket, int flags)
{
    if ((flags & Box::OUTPUT) > 0)
        qFatal("Tried to remove InputSocket as Output");
    QHash<QString, InputSocket*> &map = ((flags&Box::DATA) > 0 ? dataInput : controlInput);
    if (!map.contains(socket->objectName()))
        qWarning() << "Attempt at removing an absent socket";
    else {
        map.remove(socket->objectName());
        emit socketRemoved(socket);
        return true;
    }
    return false;
}

bool Box::removeSocket(OutputSocket *socket, int flags)
{
    if ((flags & Box::OUTPUT) > 0)
        qFatal("Tried to remove InputSocket as Output");
    QHash<QString, InputSocket*> &map = ((flags&Box::DATA) > 0 ? dataInput : controlInput);
    if (!map.contains(socket->objectName()))
        qWarning() << "Attempt at removing an absent socket";
    else {
        map.remove(socket->objectName());
        emit socketRemoved(socket);
        return true;
    }
    return false;
}


}
