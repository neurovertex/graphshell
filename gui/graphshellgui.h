#ifndef GRAPHSHELLGUI_H
#define GRAPHSHELLGUI_H
#include <QMainWindow>
#include "ui_mainwindow.h"
#include "../core/graphshell.h"

using namespace graphshell;

class GraphViewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GraphViewWidget(QWidget *parent = 0);

signals:

public slots:
private:
	GraphShell *shell;
};

namespace Ui {
class MainWindow;
}

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

#endif // GRAPHSHELLGUI_H
