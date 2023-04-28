#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QThread>

MainWindow::MainWindow(std::string file_path, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->completer = TextCompleter(ui->main_edit, file_path);
}

MainWindow::~MainWindow()
{
    delete ui;
}


std::string init_hit(TextCompleter* completer){
    return completer->hint();
}

void MainWindow::on_main_edit_textChanged()
{
    QFuture<std::string> future = QtConcurrent::run(init_hit, &completer);
    std::string res = future.result();
    //QMessageBox::warning(this, QString("bruuh"), QString::fromUtf8(res));
    ui->word_hints->setText(QString::fromUtf8(res));
}

