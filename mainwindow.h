#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "textcompleter.h"
#include <QMainWindow>
#include <QtConcurrent>
#include <QFuture>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow( std::string file_path, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_main_edit_textChanged();

private:
    Ui::MainWindow *ui;
    TextCompleter completer;
};
#endif // MAINWINDOW_H
