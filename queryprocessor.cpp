#include "queryprocessor.h"

QueryProcessor::QueryProcessor(DatabaseHandler *database, LinguisticModules *linguistic_modules)
{
    this->database = database;
    this->linguistic_modules = linguistic_modules;
}

void QueryProcessor::search(QString query){
    QList<query_words> final_query = query_tokenizer(query);

    foreach(query_words word, final_query){
        QString message(word.word);
        message.append(" : ");
        message.append(QString::number(word.position));
        emit show_message(message);
    }
}

QList<query_words> QueryProcessor::query_tokenizer(QString query){
    QStringList tokens = query.split(QRegExp("\\s+"));
    QList<query_words> final_query;
    int position = -1;
    foreach(QString word, tokens){
        position ++;
        if(!linguistic_modules->is_stop_word(word)){
            query_words new_word;
            new_word.word = linguistic_modules->word_normalizer(word);
            new_word.position = position;
            final_query.append(new_word);
        }
    }
    return final_query;
}
