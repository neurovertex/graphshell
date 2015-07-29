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

    GraphManager &manager = GraphManager::getInstance();
    GraphShell &shell = manager.newGraph("newshell");
    graph = new GraphViewScene(*this, shell);
    graph->setSceneRect(-500, -250, 1000, 500);
    ui->graphicsView->setScene(graph);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNew_Graph_triggered()
{
    qDebug() << "New graph requested";

    if (graph->getShell().getBoxes().size() == 1) {
        TextReaderBox *reader = (TextReaderBox*) graph->getShell().getBoxes()[0];
        QFile *f = new QFile("output.txt");
        if (!f->open(QIODevice::Text | QIODevice::WriteOnly | QIODevice::Truncate))
            qFatal(f->errorString().toStdString().c_str());
        TextPrinterBox *printer = new TextPrinterBox(*f);
        f->setParent(printer);
        printer->setAutoStart(true);
        graph->getShell().addBox(*printer);
        printer->setObjectName("Printer");
        qDebug() << &graph->getShell().connectSockets(*reader->getTextOutput(), *printer->getTextInput());
    } else
        qDebug() << "already called";
}

void MainWindow::on_actionStart_triggered()
{
    qDebug() << "Start requesteed";
    graph->getShell().start();
}

}
}
