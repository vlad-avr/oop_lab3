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
    ui->hint_list->setVisible(false);
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
    ui->main_edit->blockSignals(true);
    QFuture<void> future = QtConcurrent::run(&MainWindow::init_hint, this);
    future.waitForFinished();
    ui->main_edit->blockSignals(false);
}



void MainWindow::on_hint_list_itemClicked(QListWidgetItem *item)
{
    QTextCursor cursor = ui->main_edit->textCursor();
    cursor.movePosition(QTextCursor::StartOfWord, QTextCursor::KeepAnchor);

    if(ui->hint_list->currentRow() == 0 && item->text()[item->text().length()-1] == '?'){
        std::string word = item->text().toStdString();
        word = word.substr(word.find_first_of('\'')+1, word.find_last_of('\'') - word.find_first_of('\'') - 1);
        QFuture<void> fut = QtConcurrent::run(&TextCompleter::update_pool, &this->completer, word);
        ui->hint_list->clear();
    }
    else{
        cursor.insertText(item->text());
    }

}


void MainWindow::on_read_btn_clicked()
{
    ui->main_edit->blockSignals(true);
    std::string path = ui->path_edit->text().toStdString();
    std::ifstream in_file(path);
    ui->main_edit->clear();
    if(in_file.is_open()){
        std::string word;

        while(std::getline(in_file, word)){
            word.append("\n");
            ui->main_edit->moveCursor(QTextCursor::End);
            ui->main_edit->insertPlainText(QString::fromUtf8(word));
            ui->main_edit->moveCursor(QTextCursor::End);
        }

    }
    else{
        QMessageBox::critical(this, QString::fromUtf8("ERROR"), QString::fromUtf8("File not found"));
    }
    ui->main_edit->blockSignals(false);
}


void MainWindow::on_save_btn_clicked()
{
    std::string path = ui->path_edit->text().toStdString();
    std::ofstream out_file(path);
    if(out_file.is_open()){
        std::string word = ui->main_edit->toPlainText().toStdString();
        out_file << word;
        QMessageBox::information(this, "Saved", "Save was successful");
    }
    else{
        QMessageBox::critical(this, QString::fromUtf8("ERROR"), QString::fromUtf8("File not found"));
    }
}

