
#include "graphshellgui.h"

namespace graphshell
{
namespace gui
{


SocketGraphicsWidget::SocketGraphicsWidget(BoxGraphicsWidget *parentbox, Socket *socket, bool data, bool input) :
    QGraphicsWidget(parentbox)
{
    this->parent = parentbox;
    this->socket = socket;
    this->isData = data;
    this->isInput = input;
    setMaximumSize(10, 10);
    setToolTip(socket->metaObject()->className());
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
    // TODO
}


}
}
