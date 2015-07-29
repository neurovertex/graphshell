#include "sockets.h"

namespace graphshell {

/*!
 * \brief Creates a new Socket. Note that the owning box of a socket is set upon
 * calling Box::addSocket, which *should* be done immediately after creation of
 * the socket. Trying to add a socket to the same box afterward will produce a
 * warning, a different box will produce a fatal error.
 * \param type the DataType of the Socket
 * \param name The name of the Socket
 */
Socket::Socket(DataType &type, QString name) :
    QObject(), box(nullptr), type(type)
{
    setObjectName(name);
}

/*!
 * \brief Sets the given box as this socket's
 * \param box
 */
void Socket::setParent(Box &box)
{
    if (this->box == nullptr) {
        this->box = &box;
        QObject::setParent(&box);
    } else if (this->box == &box)
        qWarning() << "Socket "<< objectName() <<"'s parent has already been set";
    else
        qFatal("Cannot change a socket's parent");
}

//########## INPUTSOCKET ##########

/*!
 * \brief Creates a new InputSocket
 * \param type The DataType of the Socket
 * \param name The name of the Socket
 */
InputSocket::InputSocket(DataType &type, QString name) :
    Socket(type, name)
{
}

/*!
 * \brief Sets this socket's parent to the given box. This only works if the socker had
 * no parent previously.
 * \param box the new parent
 * \see Socket::setParent(Box&)
 */
void InputSocket::setParent(Box &box)
{
    Socket::setParent(box);
}

/*!
 * \brief Calls the connected OutputSocket's - if any - OutputSocket::connectSocket(Pipe*).
 * This exists because OutputSocket::connectSocket(Pipe*) is protected, and InputSocket is
 * a friend class, but its subclasses aren't. Thus connectOutput() "relays" friendliness
 * to subclasses.
 * \sa InputSocket::disconnectOutput()
 */
void InputSocket::connectOutput() {
    if (connectingPipe != nullptr)
        connectingPipe->getOutput().connectSocket(*connectingPipe);
}

/*!
 * \brief Calls the connected OuputSocket's - if any - OutputSocket::disconnectSocket().
 * \sa InputSocket::connectOutput()
 */
void InputSocket::disconnectOutput() {
    if (connectingPipe != nullptr)
        connectingPipe->getOutput().disconnectSocket();
}

//########## OUTPUTSOCKET ##########
/*!
 * \brief Creates a new OutputSocket
 * \param type The DataType of the Socket
 * \param name The name of the Socket
 */
OutputSocket::OutputSocket(DataType &type, QString name) :
    Socket(type, name)
{
}


/*!
 * \brief Sets this socket's parent to the given box. This only works if the socker had
 * no parent previously.
 * \param box the new parent
 * \see Socket::setParent(Box&)
 */
void OutputSocket::setParent(Box &box)
{
    Socket::setParent(box);
}

}
