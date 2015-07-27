
#include "graphshellgui.h"

namespace graphshell
{
namespace gui
{


BoxGraphicsWidget::BoxGraphicsWidget(Box *box) :
     QGraphicsWidget()
{
    this->box = box;
    setAutoFillBackground(true);
    setGeometry(0, 0, 150, 100);
    setFlag(QGraphicsItem::ItemIsMovable);
    setToolTip(box->getTypeName());
    setContentsMargins(0, 0, 0, 0);

    QAction *rename = new QAction("&Rename", this);
    connect(rename, &QAction::triggered, this, &BoxGraphicsWidget::promptRename);
    addAction(rename);

    QLabel *objNameLabel = new QLabel(box->objectName());
    connect(box, &QObject::objectNameChanged, objNameLabel, &QLabel::setText);
    QGraphicsProxyWidget *objName = new QGraphicsProxyWidget(this);
    objName->setWidget(objNameLabel);


    QGraphicsGridLayout *grid = new QGraphicsGridLayout(this);
    grid->setContentsMargins(0, 0, 0, 0);

    grid->setColumnFixedWidth(0, 10);
    grid->setColumnFixedWidth(2, 10);
    grid->setRowFixedHeight(0, 10);
    grid->setRowFixedHeight(2, 10);


    dataInput =     new QGraphicsLinearLayout(Qt::Vertical);
    dataOutput =    new QGraphicsLinearLayout(Qt::Vertical);
    controlInput =  new QGraphicsLinearLayout(Qt::Horizontal);
    controlOutput = new QGraphicsLinearLayout(Qt::Horizontal);

    for (Socket *s : box->getDataInput()) {
        socketAdded(s, Box::DATA | Box::INPUT);
    }
    for (Socket *s : box->getDataOutput()) {
        socketAdded(s, Box::DATA | Box::OUTPUT);
    }
    for (Socket *s : box->getControlInput()) {
        socketAdded(s, Box::CONTROL | Box::INPUT);
    }
    for (Socket *s : box->getControlOutput()) {
        socketAdded(s, Box::CONTROL | Box::OUTPUT);
    }

    connect(box, &Box::socketAdded, this, &BoxGraphicsWidget::socketAdded);
    connect(box, &Box::socketRemoved, this, &BoxGraphicsWidget::socketRemoved);

    grid->addItem(dataInput,1, 0);
    grid->addItem(dataOutput, 1, 2);
    grid->addItem(controlInput, 0, 1);
    grid->addItem(controlOutput, 2, 1);
    grid->addItem(objName, 1, 1);

    setLayout(grid);
}

void BoxGraphicsWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->setBrush(palette().light());
    painter->drawRect(boundingRect());
}


void BoxGraphicsWidget::promptRename()
{
    bool ok = false;
    QString text = QInputDialog::getText(dynamic_cast<QMainWindow*>(this->window()), "Rename box", "New name",
                                         QLineEdit::Normal, box->objectName(), &ok);
    if (ok && !text.isEmpty())
        box->setName(text);
}

void BoxGraphicsWidget::socketAdded(Socket *s, int flags)
{
    qDebug() << "added Socket "<< s->objectName();
    SocketGraphicsWidget *sock = new SocketGraphicsWidget(this, s, (flags & Box::DATA) != 0, (flags & Box::INPUT) != 0);
    sockets.insert(s, sock);
    ((flags & Box::DATA) > 0 ? ((flags & Box::INPUT) > 0 ? dataInput : dataOutput) :
                               ((flags & Box::INPUT) > 0 ? controlInput : controlOutput)
        )->addItem(sock);
}

void BoxGraphicsWidget::socketRemoved(Socket *socket)
{
    SocketGraphicsWidget *sock = sockets.value(socket);
    if (socket != nullptr) {
        dynamic_cast<QGraphicsLinearLayout*>(sock->parentLayoutItem())->removeItem(sock);
        sockets.remove(socket);
        delete sock;
    }
}

void BoxGraphicsWidget::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QMenu menu;
    menu.exec(actions(), event->screenPos());
}




}
}
