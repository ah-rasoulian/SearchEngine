#include "tokenizer.h"
// /home/amirhossein/Data/University/Fall 2020/Information Retreival/sampleDoc
Tokenizer::Tokenizer()
{

}

void Tokenizer::find_files(QDir directory){
    foreach(QString filename, directory.entryList()){
        if (filename.compare(".") == 0 || filename.compare("..") == 0)
            continue;

        QString path(directory.path());
        path.append("/");
        path.append(filename);

       if (filename.contains(".txt")){
           QFile *file = new QFile(path);
           if(file->exists())
               tokenize(file);
           else
               emit show_message("Can not open file!!!");
       }
       else{
           QDir nested_directory(path);
           if (nested_directory.exists())
               find_files(path);
       }
    }
}

void Tokenizer::tokenize(QFile *file){
    file->open(QIODevice::ReadOnly | QIODevice::Text);
    QString content = file->readAll();
    QStringList tokens = content.split(" ");
    file->close();

    foreach(QString word, tokens){
        emit show_message(word);
    }
}

