#include "tokenizer.h"

Tokenizer::Tokenizer(DatabaseHandler *database, LinguisticModules *linguistic_modules)
{
    this->database = database;
    this->linguistic_modules = linguistic_modules;
}

void Tokenizer::find_files(QDir directory, bool nested_function, bool is_clustered_dir){
    if (is_clustered_dir){

        QString cluster_name = directory.absolutePath().split("/")[directory.absolutePath().split("/").size() - 1];
        emit show_message(cluster_name);
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
                   document_tokenize(file, docID, cluster_name);
               else
                   emit show_message("Can not open file!!!");
           }
           else{
               QDir nested_directory(path);
               if (nested_directory.exists())
                   find_files(path, true, true);

           }
        }
    }
    else {
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
                   document_tokenize(file, docID, "");
               else
                   emit show_message("Can not open file!!!");
           }
           else{
               QDir nested_directory(path);
               if (nested_directory.exists())
                   find_files(path, true, false);
           }
        }

    }

    if(!nested_function){
        database->calculate_document_tf_idf();
        database->make_champion_list();
        if (is_clustered_dir)
            database->calculate_clusters_mean();
        emit show_message("Documents has been added successfully.");
    }
}

void Tokenizer::document_tokenize(QFile *file, unsigned long docID, QString cluster){
    file->open(QIODevice::ReadOnly | QIODevice::Text);
    QString content = file->readAll();
    QStringList tokens = content.split(QRegExp("\\s+"), Qt::SkipEmptyParts);
    file->close();
    if(cluster.compare("") != 0){
        unsigned long position = -1;
        foreach(QString word, tokens){
            position ++;
            if (!linguistic_modules->is_stop_word(word))
                database->indexer(linguistic_modules->word_normalizer(word), docID, position, cluster);
        }
    }
    else {
        unsigned long position = -1;
        foreach(QString word, tokens){
            position ++;
            if (!linguistic_modules->is_stop_word(word))
                database->indexer(linguistic_modules->word_normalizer(word), docID, position, "");
        }
    }
}
