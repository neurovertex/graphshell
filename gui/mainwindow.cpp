#include "graphshellgui.h"
#include "core/boxes/textboxes.h"

namespace graphshell
{
namespace gui
{
using namespace graphshell::boxes;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    GraphManager *manager = GraphManager::getInstance();
    GraphShell *shell = manager->newGraph("newshell");
    QGraphicsScene *scene = new GraphViewScene(this, shell);

    ui->graphicsView->setScene(scene);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNew_Graph_triggered()
{
    qDebug() << "lolilol";
}

}
}
