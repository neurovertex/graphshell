
#include "graphshellgui.h"

namespace graphshell
{
namespace gui
{


SocketGraphicsWidget::SocketGraphicsWidget(BoxGraphicsWidget &parentbox, Socket &socket, bool data, bool input) :
    QGraphicsWidget(&parentbox), parent(parentbox), socket(socket), isData(data), isInput(input)
{
    setMaximumSize(10, 10);
    QString type = socket.metaObject()->className();
    type = type.replace("graphshell::sockets::", "");
    setToolTip(socket.objectName() +" ("+ type +")");
}

void SocketGraphicsWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (!isData) {
        QTransform rotate;
        rotate.translate(boundingRect().width()/2, boundingRect().height()/2);
        rotate.rotate(90);
        rotate.translate(-boundingRect().width()/2, -boundingRect().height()/2);
        painter->setTransform(rotate, true);
    }

    QPolygon triangle;
    triangle << QPoint(0, 0) << QPoint(0, boundingRect().height()) << QPoint(boundingRect().width(), boundingRect().height()/2);
    painter->setBrush(palette().dark());
    painter->drawConvexPolygon(triangle);
}

void SocketGraphicsWidget::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    Q_UNUSED(event)
    // TODO
}


}
}
