#include "graphshell.h"

namespace graphshell {
GraphShell::GraphShell(GraphManager &parent, QString name) :
    QObject(&parent)
{
    setObjectName(name);
}

/*!
 * \brief Adds a box to the graph. This should be called immediately after the box
 * creation
 * \param box the Box to add
 */
void GraphShell::addBox(Box &box)
{
    if (!boxes.contains(&box)) {
        boxes << &box;
        box.setParent(*this);
        emit boxAdded(box);
    } else
        qWarning() << "Tried to add box to graph multiple times";
}

/*!
 * \brief This removes the Box from the graph and deletes it.
 * \param box the Box to remove
 * \return true if the Box was removed successfully (false if the box wasn't present in the graph)
 */
bool GraphShell::removeBox(Box &box)
{
    if (boxes.removeAll(&box) > 0) {
        emit boxRemoved(box);
        delete &box;
        return true;
    }
    return false;
}

/*!
 * \brief Connects the given InputSocket and OutputSocket together and create a
 * Pipe for the connection
 * \param out Producer socket
 * \param in Consumer socket
 * \return The Pipe representing the connection between the sockets
 */
Pipe &GraphShell::connectSockets(OutputSocket &out, InputSocket &in)
{
    Pipe *p = new Pipe(*this, in, out);
    pipes << p;
    emit pipeAdded(*p);
    return *p;
}

/*!
 * \brief Disconnects the InputSocket and OutputSocket of the given Pipe.
 * \param pipe Pipe to delete
 * \return true if the pipe was successfully removed (if it was registered with this graph)
 */
bool GraphShell::disconnectSockets(Pipe &pipe)
{
    int removed = pipes.removeAll(&pipe);
    if (removed > 0) {
        emit pipeRemoved(pipe);
        delete &pipe;
    }
    return removed > 0;
}

/*!
 * \brief Starts the execution of the graph. Calls Box::startBox() on all Boxes that have
 * Box::autostart enabled
 */
void GraphShell::start()
{
    for (Box* b : boxes)
        if (b->autostarts())
            b->startBox();
}


/*!
 * \brief Stops gracefully the execution of the graph. Calls Box::stopBox() on all Boxes
 */
void GraphShell::stop()
{
    for (Box* b : boxes)
        b->stopBox();
}

/*!
 * \brief Forcefully terminates the execution of the graph. Calls QThread::terminate() on all boxes
 */
void GraphShell::terminate()
{
    for (Box* b : boxes)
        b->terminate();
}

}
