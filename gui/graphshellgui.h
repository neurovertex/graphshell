#ifndef GRAPHSHELLGUI_H
#define GRAPHSHELLGUI_H
#include <QtGui>
#include <QtWidgets>
#include "ui_mainwindow.h"
#include "../core/graphshell.h"


namespace Ui {
class MainWindow;
}

namespace graphshell
{
namespace gui
{

//########## MAINWINDOW ##########

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionNew_Graph_triggered();

private:
    Ui::MainWindow *ui;
};




//########## GRAPHVIEW ##########
class BoxGraphicsWidget;
class GraphViewScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GraphViewScene(MainWindow *window, GraphShell *shell);

signals:

public slots:
    void boxAdded(Box* box);
    void boxRemoved(Box *box);
private:
	GraphShell *shell;
    QHash<Box*, BoxGraphicsWidget*> boxes;
};



//########## BOXGRAPHICS ##########
class SocketGraphicsWidget;
class BoxGraphicsWidget : public QGraphicsWidget
{
    Q_OBJECT
public:
    BoxGraphicsWidget(Box *box);
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) Q_DECL_OVERRIDE;
private:
    Box *box;
    QHash<Socket*, SocketGraphicsWidget*> sockets;

    QGraphicsLinearLayout *dataInput;
    QGraphicsLinearLayout *dataOutput;
    QGraphicsLinearLayout *controlInput;
    QGraphicsLinearLayout *controlOutput;
public slots:
    void promptRename();
    void socketAdded(Socket *s, int flags);
    void socketRemoved(Socket *socket);
};



//########## SOCKETGRAPHICS ##########
class SocketGraphicsWidget : public QGraphicsWidget
{
    Q_OBJECT
public:
    SocketGraphicsWidget(BoxGraphicsWidget *parentbox, Socket * socket, bool data, bool input);
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) Q_DECL_OVERRIDE;
private:
    BoxGraphicsWidget *parent;
    Socket *socket;
    bool isData;
    bool isInput;
};



}
}


#endif // GRAPHSHELLGUI_H
