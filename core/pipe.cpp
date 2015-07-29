#include "graphshell.h"

namespace graphshell {

/*!
 * \brief Creates a Pipe object to represent the connection between input and output. This will call
 * InputSocket::connectSocket(Pipe*)
 * \param parent The parent GraphShell of the Pipe (for memory management)
 * \param input the data consumer
 * \param output the data producer
 */
Pipe::Pipe(GraphShell &parent, InputSocket &input, OutputSocket &output) :
    QObject(&parent), input(input), output(output)
{
    input.connectSocket(*this);
}

/*!
 * \brief Deletes the Pipe and disconnects the corresponding sockets
 */
Pipe::~Pipe()
{
    input.disconnectSocket();
}
}

