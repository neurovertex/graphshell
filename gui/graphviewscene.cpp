#include "graphshellgui.h"

namespace graphshell
{
namespace gui
{


GraphViewScene::GraphViewScene(MainWindow *window, GraphShell *shell) : QGraphicsScene(window)
{
    this->shell = shell;
    connect(shell, &GraphShell::boxAdded, this, &GraphViewScene::boxAdded);
    connect(shell, &GraphShell::boxRemoved, this, &GraphViewScene::boxRemoved);
}

void GraphViewScene::boxAdded(Box *box)
{
    if (!boxes.contains(box)) {
        qDebug() << "Adding box "<< box->objectName() <<"("<< box->getTypeName() <<")";
        BoxGraphicsWidget *graphbox = new BoxGraphicsWidget(box);
        boxes.insert(box, graphbox);
        addItem(graphbox);
    }
}

void GraphViewScene::boxRemoved(Box *box)
{
    if (boxes.contains(box)) {
        BoxGraphicsWidget *graphbox = boxes.value(box);
        boxes.remove(box);
        removeItem(graphbox);
    }
}

}
}
