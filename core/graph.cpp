#include "graphshell.h"

namespace graphshell {

GraphShell::GraphShell(QObject *parent, QString name) : QObject(parent)
{
    setObjectName(name);
}

void GraphShell::addBox(Box *box)
{
    if (!boxes.contains(box)) {
        boxes << box;
        box->setParent(this);
        emit boxAdded(box);
    } else
        qWarning() << "Tried to add box to graph multiple times";
}

bool GraphShell::removeBox(Box *box)
{
    if (boxes.removeAll(box) > 0) {
        emit boxRemoved(box);
        delete box;
        return true;
    }
    return false;
}

Pipe *GraphShell::connect(InputSocket *in, OutputSocket *out)
{
    Pipe *p = new Pipe(this, in, out);
    pipes << p;
    emit pipeAdded(p);
    return p;
}

bool GraphShell::disconnect(Pipe *pipe)
{
    int removed = pipes.removeAll(pipe);
    if (removed > 0)
        emit pipeRemoved(pipe);
    delete pipe;
    return removed > 0;
}

void GraphShell::start()
{
    for (Box* b : boxes)
        if (b->autostarts())
            b->startBox();
}

void GraphShell::stop()
{
    for (Box* b : boxes)
        b->stopBox();
}

}
