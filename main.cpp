#include "mainwindow.h"
#include <iostream>
#include <fstream>
#include <QApplication>
#include <QMessageBox>

void f(){

}

int main(int argc, char *argv[])
{
    std::string file_path = "D:\\QT_projects\\oop_lab3\\words_list.txt";
    QApplication a(argc, argv);
    MainWindow w(file_path);
    w.show();
    return a.exec();
}
