#ifndef TEXTCOMPLETER_H
#define TEXTCOMPLETER_H
#include <QTextEdit>
#include <iostream>
#include <fstream>
#include <string>

class TextCompleter
{
public:
    TextCompleter(QTextEdit* textarea, std::string file_path){
        this->textarea = textarea;
        this->file_path = file_path;
        getListMap(this->file_path);
    }

    void getListMap(const std::string file_path){
        std::ifstream in_file(file_path);
        if(in_file.is_open()){
            char cur_char = ' ';
            std::string line;
            std::streampos prev_pos = 0;
            while(std::getline(in_file, line)){
                std::streampos cur_pos = in_file.tellg();
                if(line[0] != cur_char){
                    cur_char = line[0];
                    list_map.insert({prev_pos, cur_char});
                }
                prev_pos = cur_pos;
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

    void hint(int min_char_count = 2, int max_hint_size = 7){
        std::string deb;
        /*for(int i = 0; i < list_map.size(); i++){
            deb.append(std::to_string(list_map[i]));
            deb.append("\t");
        }*/
        deb.append(std::to_string(list_map.size()));
        textarea->setText(QString::fromUtf8(deb));
    }

private:
    QTextEdit* textarea;
    std::map<std::streampos, char> list_map;
    std::string file_path;
};

#endif // TEXTCOMPLETER_H
