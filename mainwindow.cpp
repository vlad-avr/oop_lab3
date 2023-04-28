#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QThread>

MainWindow::MainWindow(std::string file_path, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->completer = new TextCompleter(ui->main_edit, file_path);
    init_threads();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init_threads()
{
    completer->hint();
}

