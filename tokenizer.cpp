#include "tokenizer.h"

Tokenizer::Tokenizer()
{

}

void Tokenizer::tokenize(QDir directory){
    foreach(QString filename, directory.entryList()){
       if (filename.contains(".txt")){
            emit show_message(filename);
            // QFile file(directory.path() + "/" + filename);
       }
    }
}

