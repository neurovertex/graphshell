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
    InputSignalSocket *start = new InputSignalSocket(this, DataType::getType("/signal/"), "start");
    InputSignalSocket *stop = new InputSignalSocket(this, DataType::getType("/signal/"), "stop");
    OutputSignalSocket *started = new OutputSignalSocket(this, DataType::getType("/signal/void/"), "started", false, new QVariant());
    OutputSignalSocket *stopped = new OutputSignalSocket(this, DataType::getType("/signal/void/"), "stopped", false, new QVariant());

    connect(start, &InputSignalSocket::valueReceived, this, &Box::startBox);
    connect(stop, &InputSignalSocket::valueReceived, this, &Box::stopBox);

    connect(this, &Box::started, started, &OutputSignalSocket::sendVoid);
    connect(this, &Box::finished, stopped, &OutputSignalSocket::sendVoid);

    controlInput.insert(start->objectName(), start);
    controlInput.insert(stop->objectName(), stop);

    controlOutput.insert(started->objectName(), started);
    controlOutput.insert(stopped->objectName(), stopped);
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
void Box::addDataInput(InputSocket *socket)
{
    if (dataInput.contains(socket->objectName()))
        removeDataInput(socket);
    dataInput.insert(socket->objectName(), socket);
    emit socketAdded(socket, Box::INPUT | Box::DATA);
}

void Box::addDataOutput(OutputSocket *socket)
{
    if (dataOutput.contains(socket->objectName()))
        removeDataOutput(socket);
    dataOutput.insert(socket->objectName(), socket);
    emit socketAdded(socket, Box::OUTPUT | Box::DATA);
}

void Box::addControlInput(InputSocket *socket)
{
    if (controlInput.contains(socket->objectName()))
        removeControlInput(socket);
    controlInput.insert(socket->objectName(), socket);
    emit socketAdded(socket, Box::INPUT | Box::CONTROL);;
}

void Box::addControlOutput(OutputSocket *socket)
{
    if (controlOutput.contains(socket->objectName()))
        removeControlOutput(socket);
    controlOutput.insert(socket->objectName(), socket);
    emit socketAdded(socket, Box::OUTPUT | Box::CONTROL);
}


// ############ REMOVE ##############
bool Box::removeDataInput(InputSocket *socket)
{
    if (dataInput.remove(socket->objectName()) > 0) {
        emit socketRemoved(socket);
        return true;
    }
    return false;
}

bool Box::removeDataOutput(OutputSocket *socket)
{
    if (dataOutput.remove(socket->objectName()) > 0) {
        emit socketRemoved(socket);
        return true;
    }
    return false;
}

bool Box::removeControlInput(InputSocket *socket)
{
    if (controlInput.remove(socket->objectName()) > 0) {
        emit socketRemoved(socket);
        return true;
    }
    return false;
}

bool Box::removeControlOutput(OutputSocket *socket)
{
    if (controlOutput.remove(socket->objectName()) > 0) {
        emit socketRemoved(socket);
        return true;
    }
    return false;
}


}
