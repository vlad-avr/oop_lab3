#ifndef TEXTCOMPLETER_H
#define TEXTCOMPLETER_H
#include <QTextEdit>
#include <iostream>
#include <fstream>
#include <string>
#include <QMessageBox>
#include <QListWidget>
#include <QtConcurrent>
#include <QFuture>

class TextCompleter
{
public:

    TextCompleter(){}
    TextCompleter(QTextEdit* textarea, std::string file_path){
        this->textarea = textarea;
        this->file_path = file_path;
        //getListMap(file_path);
        //future.waitForFinished();
    }

    void getListMap(const std::string file_path){
        QMutex mutex;
        while(!mutex.try_lock()){
            continue;
        }
        std::ifstream in_file;
        in_file.open(file_path, std::ios_base::in);
        if(in_file.is_open()){
            list_map.clear();
            char cur_char = '1';
            std::string line;
            list_map.insert({0,'1'});
            int cur_pos = 0;
            int prev_pos = 0;
            while(std::getline(in_file, line, ' ')){
                prev_pos = cur_pos;
                cur_pos = in_file.tellg();
                //qDebug() << prev_pos << line << cur_pos;
                if(line[0] != cur_char){
                    cur_char = line[0];
                    list_map.insert({prev_pos, cur_char});
                }
            }
            in_file.close();
        }
        mutex.unlock();
    }

    QTextCursor getCursor(){
        return textarea->textCursor();
    }

    void hint(QListWidget* lw, unsigned int min_char_count = 2/*, unsigned int max_hint_size = 7*/){

        QMutex mutex;
        while(!mutex.try_lock()){
            continue;
        }
        QTextCursor cursor = getCursor();
        lw->setVisible(false);
        lw->clear();

        QRect lw_pos = textarea->cursorRect();
        lw->move(QPoint(lw_pos.right(), lw_pos.bottom()+10));
        cursor.movePosition(QTextCursor::StartOfWord, QTextCursor::KeepAnchor);
        QString str = cursor.selectedText();
        if(str.length() >= min_char_count){
            int pos;
            bool is_new = true;
            for(auto &item : list_map){
                if(item.second == str[0]){
                    pos = item.first;
                    break;
                }
            }
            std::ifstream in_file(file_path, std::ios_base::in);
            in_file.seekg(pos, in_file.beg);
            //qDebug()<<in_file.tellg();
            //std::string hint_str;
            if(in_file.is_open()){
                std::string line;
                while(std::getline(in_file, line, ' ')){
                    if(line[0] != str[0]){
                        break;
                    }
                    else if(line == str.toStdString()){
                        is_new = false;
                    }
                    else if(line.find(str.toStdString()) == 0){
                       // hints.push_back(line);
                        //hint_str.append(line + "\n");
                        lw->addItem(QString::fromUtf8(line));
                        if(!lw->isVisible()){
                            lw->setVisible(true);
                        }
                    }
                }
                if(is_new){
                    lw->setVisible(true);
                    lw->insertItem(0, QString::fromUtf8("Add '" + str.toStdString() + "' to pool?"));
                }
            }
        }
        mutex.unlock();
    }

    bool str_comp(std::string* str1, std::string* str2) {

        int len = (*str1).length();
        if(len > (*str2).length()){
            len = (*str2).length();
        }
        for (int i = 0; i < len; i++) {
            if ((*str1)[i] < (*str2)[i]) {
                return true;
            }
            else if ((*str1)[i] > (*str2)[i]) {
                return false;
            }
            else {
                continue;
            }
        }
        return true;

    }

    void update_pool(std::string word){
        QMutex mutex;
        while(!mutex.try_lock()){
            continue;
        }
        std::vector<std::string> buffer;
        std::ifstream in_file(file_path);
        if(in_file.is_open()){
            std::string line;
            while(std::getline(in_file, line, ' ')){
                if(str_comp(&word, &line) && word != ""){
                    buffer.push_back(word);
                    word = "";
                }
                buffer.push_back(line);
            }
            in_file.close();
        }
        std::ofstream out_file(file_path);
        if(out_file.is_open()){
            for(int i = 0; i < buffer.size(); i++){
                out_file << buffer[i] << ' ';
            }
            out_file.close();
        }
        mutex.unlock();
        getListMap(file_path);
    }

private:
    QTextEdit* textarea;
    std::map<int, char> list_map;
    std::string file_path;
};

#endif // TEXTCOMPLETER_H
