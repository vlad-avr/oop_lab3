#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "textcompleter.h"
#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow( std::string file_path, QWidget *parent = nullptr);
    ~MainWindow();
    void init_hint();

private slots:
    void on_main_edit_textChanged();


    void on_hint_list_itemClicked(QListWidgetItem *item);

    void on_read_btn_clicked();

    void on_save_btn_clicked();

private:
    Ui::MainWindow *ui;
    /*!
     * \brief completer for completing text
     */
    TextCompleter completer;
};
#endif // MAINWINDOW_H
