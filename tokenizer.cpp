#include "tokenizer.h"
// /home/amirhossein/Data/University/Fall 2020/Information Retreival/sampleDoc
Tokenizer::Tokenizer(DatabaseHandler *database)
{
    Tokenizer::database = database;

    persian_punctuations << "!" << "،" << "؟" << "." << ":";

    persian_plural_signs << "ها";

    persian_stop_words << "از" << "در" << "با" << "و" << "من" << "تو" << "او" << "ما" << "شما" << "آن" << "آنها" << "برای" << "تا" << "به" << "را" << "این" << "هم" << "درباره" << "که" << "ولی" << "اما";

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
               tokenize(file, docID);
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

void Tokenizer::tokenize(QFile *file, unsigned long docID){
    file->open(QIODevice::ReadOnly | QIODevice::Text);
    QString content = file->readAll();
    QStringList tokens = content.split(QRegExp("\\s+"));
    file->close();

    foreach(QString word, tokens){
        if (is_stop_word(word))
            continue;

        // showing the term + doc ID
        QString message(linguistic_modules(word));
        message.append(" , doc ID :");
        message.append(QString::number(docID));
        emit show_message(message);
    }
}

QString Tokenizer::linguistic_modules(QString word){
    word = remove_punctuations(word);
    word = remove_plural_signs(word);
    return word;
}

QString Tokenizer::remove_punctuations(QString word){
    foreach(QString punctuation, persian_punctuations){
        if (word.endsWith(punctuation) || word.startsWith(punctuation))
            word = word.remove(punctuation);
    }
    return word;
}

QString Tokenizer::remove_plural_signs(QString word){
    foreach(QString sign, persian_plural_signs){
        if (word.endsWith(sign))
            word = word.remove(sign);
    }
    return word;
}

bool Tokenizer::is_stop_word(QString word){
    foreach(QString stopword, persian_stop_words){
        if (word.compare(stopword) == 0)
            return true;
    }
    return false;
}
