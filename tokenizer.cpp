#include "tokenizer.h"
// /home/amirhossein/Data/University/Fall 2020/Information Retreival/sampleDoc
Tokenizer::Tokenizer()
{
    persian_punctuations << "!" << "،" << "؟" << "." << ":";

    persian_plural_signs << "ها";

    persian_stop_words << "از" << "در" << "با" << "و" << "من" << "تو" << "او" << "ما" << "شما" << "آن" << "آنها" << "برای" << "تا" << "به" << "را" << "این" << "هم" << "درباره" << "که" << "ولی" << "اما";
    persian_stop_words << "بود" << "است" << "شده" << "شد" << "کرد" << "کند" << "دارد" << "داشت" << "چه";

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
    QStringList tokens = content.split(QRegExp("\\s+"));
    file->close();

    foreach(QString word, tokens){
        if (is_stop_word(word))
            continue;
        emit show_message(linguistic_modules(word));
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
