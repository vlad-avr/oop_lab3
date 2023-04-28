#ifndef TEXTCOMPLETER_H
#define TEXTCOMPLETER_H
#include <QTextEdit>
#include <iostream>
#include <fstream>
#include <string>
#include <QMessageBox>
#include <QFile>
#include <QIODevice>

class TextCompleter
{
public:

    TextCompleter(){}
    TextCompleter(QTextEdit* textarea, std::string file_path){
        this->textarea = textarea;
        this->file_path = file_path;
        getListMap(this->file_path);
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
                qDebug() << prev_pos << line << cur_pos;
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

    std::string hint(unsigned int min_char_count = 2, unsigned int max_hint_size = 7){
        QTextCursor cursor = getCursor();
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
            qDebug()<<in_file.tellg();
            std::string hint_str;
            if(in_file.is_open()){
                std::string line;
                while(std::getline(in_file, line, ' ')){
                    if(line[0] != str[0]){
                        break;
                    }
                    else if(line.find(str.toStdString()) == 0){
                       // hints.push_back(line);
                        hint_str.append(line + "\n");
                    }
                }
            }
            return hint_str;
        }
        else{
            return "";
        }
    }

private:
    QTextEdit* textarea;
    std::map<int, char> list_map;
    std::string file_path;
};

#endif // TEXTCOMPLETER_H
