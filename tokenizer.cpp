#include "tokenizer.h"
// /home/amirhossein/Data/University/Fall 2020/Information Retreival/sampleDoc
Tokenizer::Tokenizer(DatabaseHandler *database, LinguisticModules *linguistic_modules)
{
    this->database = database;
    this->linguistic_modules = linguistic_modules;
}

void Tokenizer::find_files(QDir directory){
    foreach(QString filename, directory.entryList()){
        if (filename.compare(".") == 0 || filename.compare("..") == 0)
            continue;

        QString path(directory.path());
        path.append("/");
        path.append(filename);

       if (filename.contains(".txt")){
           long int docID = database->get_document_number(path);
           QFile *file = new QFile(path);
           if(file->exists())
               document_tokenize(file, docID);
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

void Tokenizer::document_tokenize(QFile *file, unsigned long docID){
    file->open(QIODevice::ReadOnly | QIODevice::Text);
    QString content = file->readAll();
    QStringList tokens = content.split(QRegExp("\\s+"), Qt::SkipEmptyParts);
    file->close();

    unsigned long position = -1;
    foreach(QString word, tokens){
        position ++;
        if (!linguistic_modules->is_stop_word(word))
            database->indexer(linguistic_modules->word_normalizer(word), docID, position);
    }
}
