#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QThread>

MainWindow::MainWindow(std::string file_path, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->completer = TextCompleter(ui->main_edit, file_path);
    QFuture<void> future = QtConcurrent::run(&TextCompleter::getListMap, &this->completer, file_path);
    ui->hint_list->setMaximumHeight(200);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::init_hint(){
    return completer.hint(this->ui->hint_list);
}

void MainWindow::on_main_edit_textChanged()
{
    QFuture<void> future = QtConcurrent::run(&MainWindow::init_hint, this);

    //QMessageBox::warning(this, QString("bruuh"), QString::fromUtf8(res));
    //ui->word_hints->setText(QString::fromUtf8(res));
}

