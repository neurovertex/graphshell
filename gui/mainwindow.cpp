#include "graphshellgui.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNew_Graph_triggered()
{
    GraphViewWidget* widget = new GraphViewWidget(this);
    ui->tabWidget->addTab(widget, "New tab");

}
