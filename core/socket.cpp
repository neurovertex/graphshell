#include "sockets.h"

namespace graphshell {

/*!
  \class Socket
  \brief Represents an I/O endpoint of a Box.
*/

/*!
 * \brief Creates a new Socket
 * \param parent The Box this Socket will belong to
 * \param type the DataType of the Socket
 * \param name The name of the Socket
 */
Socket::Socket(DataType *type, QString name) :
    QObject()
{
    this->box = nullptr;
    this->type = type;
    setObjectName(name);
}

void Socket::setParent(Box *box)
{
    if (this->box == nullptr) {
        this->box = box;
        QObject::setParent(box);
    } else if (this->box == box)
        qWarning() << "Socket "<< objectName() <<"'s parent has already been set";
    else
        qFatal("Cannot change a socket's parent");
}

Socket::~Socket()
{}

/*!
 * \brief Creates a new InputSocket
 * \param box The Box this Socket will belong to
 * \param type The DataType of the Socket
 * \param name The name of the Socket
 */
InputSocket::InputSocket(DataType *type, QString name) :
    Socket(type, name)
{
}

InputSocket::~InputSocket()
{
}

void InputSocket::setParent(Box *box)
{
    Socket::setParent(box);
    connect(this, &InputSocket::socketConnected,
            getBox(), &Box::socketConnected);
    connect(this, &InputSocket::socketDisconnected,
            getBox(), &Box::socketDisconnected);
}

/*!
 * \fn InputSocket::connectSocket(Pipe*)
 * \brief Connects the InputSocket to an OutputSocket via a Pipe. If already connected the socket
 * will disconnect first.
 * \param pipe The connecting pipe
 */

/*!
 * \fn InputSocket::disconnectSocket()
 * \brief Disconnects the InputSocket from its current connectingPipe if any. This should only be
 * callse by Pipe::~Pipe() and will call OutputSocket::disconnectSocket()
 */

/*!
 * \brief Calls the connected OutputSocket's - if any - OutputSocket::connectSocket(Pipe*).
 * This exists because OutputSocket::connectSocket(Pipe*) is protected, and InputSocket is
 * a friend class, but its subclasses aren't. Thus connectOutput() "relays" friendliness
 * to subclasses.
 * \sa InputSocket::disconnectOutput()
 */

void InputSocket::connectOutput() {
    if (connectingPipe != nullptr)
        connectingPipe->getOutput()->connectSocket(connectingPipe);
}

/*!
 * \brief Calls the connected OuputSocket's - if any - OutputSocket::disconnectSocket().
 * \sa InputSocket::connectOutput()
 */
void InputSocket::disconnectOutput() {
    if (connectingPipe != nullptr)
        connectingPipe->getOutput()->disconnectSocket();
}

/*!
 * \brief Creates a new OutputSocket
 * \param box The Box this Socket will belong to
 * \param type The DataType of the Socket
 * \param name The name of the Socket
 */
OutputSocket::OutputSocket(DataType *type, QString name) :
    Socket(type, name)
{
}

OutputSocket::~OutputSocket()
{
}

void OutputSocket::setParent(Box *box)
{
    Socket::setParent(box);
    connect(this, &OutputSocket::socketConnected,
            getBox(), &Box::socketConnected);
    connect(this, &OutputSocket::socketDisconnected,
            getBox(), &Box::socketDisconnected);
}

}
