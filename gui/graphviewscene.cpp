#include "graphshellgui.h"

namespace graphshell
{
namespace gui
{


GraphViewScene::GraphViewScene(MainWindow &window, GraphShell &shell) :
    QGraphicsScene(&window), shell(shell)
{
    connect(&shell, &GraphShell::boxAdded,
            this, &GraphViewScene::boxAdded);
    connect(&shell, &GraphShell::boxRemoved,
            this, &GraphViewScene::boxRemoved);
}

void GraphViewScene::boxAdded(Box &box)
{
    if (!boxes.contains(&box)) {
        qDebug() << "Adding box "<< box.objectName() <<"("<< box.getTypeName() <<")";
        BoxGraphicsWidget *graphbox = new BoxGraphicsWidget(box);
        boxes.insert(&box, graphbox);
        addItem(graphbox);
        graphbox->moveBy((qrand()%200) - 100, (qrand()%200) - 100);
    }
}

void GraphViewScene::boxRemoved(Box &box)
{
    if (boxes.contains(&box)) {
        BoxGraphicsWidget *graphbox = boxes.value(&box);
        boxes.remove(&box);
        removeItem(graphbox);
    }
}

void GraphViewScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect)
    const int MINOR_TICK=20;
    const int MAJOR_TICK=100 / MINOR_TICK; //
    const int XMIN = rect.x(), XMAX = rect.x()+rect.width(),
              YMIN = rect.y(), YMAX = rect.y()+rect.height();
    {   // Vertical lines
        int min = qCeil(XMIN/MINOR_TICK), max = qFloor(XMAX/MINOR_TICK);
        for (int i = min; i <= max; i ++) {
            if (i == 0)
                continue;
            else if (i%MAJOR_TICK == 0)
                painter->setPen(Qt::gray);
            else
                painter->setPen(Qt::lightGray);
            painter->drawLine(i*MINOR_TICK, YMIN, i*MINOR_TICK, YMAX);
        }
    }
    {   // Horizontal lines
        int min = qCeil(YMIN/MINOR_TICK), max = qFloor(YMAX/MINOR_TICK);
        for (int i = min; i <= max; i ++) {
            if (i == 0)
                painter->setPen(Qt::black);
            else if (i%MAJOR_TICK == 0)
                painter->setPen(Qt::darkGray);
            else
                painter->setPen(Qt::lightGray);
            painter->drawLine(XMIN, i*MINOR_TICK, XMAX, i*MINOR_TICK);
        }
    }
    if (XMIN <= 0 && XMAX >= 0) {
        painter->setPen(Qt::black);
        painter->drawLine(0, YMIN, 0, YMAX);
    }
}



}
}
