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
        std::ifstream in_file;
        in_file.open(file_path, std::ios_base::in);
        if(in_file.is_open()){
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
        else{
            return;
        }
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
        lw->clear();
        QRect lw_pos = textarea->cursorRect();
        lw->move(QPoint(lw_pos.right(), lw_pos.bottom()+10));
        cursor.movePosition(QTextCursor::StartOfWord, QTextCursor::KeepAnchor);
        QString str = cursor.selectedText();
        if(str.length() >= min_char_count){
            std::vector<std::string> hints;
            int pos;
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
                    else if(line.find(str.toStdString()) == 0){
                       // hints.push_back(line);
                        //hint_str.append(line + "\n");
                        lw->addItem(QString::fromUtf8(line));

                    }
                }
            }
            //return hint_str;
        }
        else{
            //return "";
        }
        mutex.unlock();
    }

private:
    QTextEdit* textarea;
    std::map<int, char> list_map;
    std::string file_path;
};

#endif // TEXTCOMPLETER_H
