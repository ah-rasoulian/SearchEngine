#include "queryprocessor.h"

QueryProcessor::QueryProcessor(DatabaseHandler *database, LinguisticModules *linguistic_modules)
{
    this->database = database;
    this->linguistic_modules = linguistic_modules;
}

void QueryProcessor::search(QString query, QString mode){
    QList<query_words> final_query = query_tokenizer(query);

    /*
     * printing each query word and its position
     *
    foreach(query_words word, final_query){
        QString message(word.word);
        message.append(" : ");
        message.append(QString::number(word.position));
        emit show_message(message);
    }
    */

    if(mode.compare("simple_binary") == 0)
        simple_binary_search(final_query);
    else if(mode.compare("ordered") == 0)
        ordered_search(final_query);

}

QList<query_words> QueryProcessor::query_tokenizer(QString query){
    QStringList tokens = query.split(QRegExp("\\s+"), Qt::SkipEmptyParts);
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

void QueryProcessor::simple_binary_search(QList<query_words> final_query){
    using namespace std;
    priority_queue< pair<int , unsigned long> > results;
    QMap<unsigned long, int> doc_frequency_in_results;
    QSet<unsigned long> documents;

    foreach(query_words query_word, final_query){
        if (database->word_exists(query_word.word)){
            pair <unsigned long, unsigned long> doc_pos;
            foreach(doc_pos, database->get_postings_list(query_word.word))
                documents.insert(doc_pos.first);

            foreach(unsigned long docID, documents){
                if(doc_frequency_in_results.contains(docID))
                    doc_frequency_in_results.insert(docID, doc_frequency_in_results.value(docID) + 1);
                else
                    doc_frequency_in_results.insert(docID, 1);
            }
        }
    }

    QMapIterator<unsigned long, int> iterator(doc_frequency_in_results);
    while(iterator.hasNext()){
        iterator.next();
        results.push(make_pair(iterator.value(), iterator.key()));
    }

    while (!results.empty()) {
        emit show_message(database->get_document_path(results.top().second));
        results.pop();
    }
}

void QueryProcessor::ordered_search(QList<query_words> final_query){
    QHash<QString, long> query_bag_of_words;
    foreach(query_words word_pos, final_query){
        if(query_bag_of_words.contains(word_pos.word))
            query_bag_of_words.insert(word_pos.word, query_bag_of_words.value(word_pos.word) + 1);
        else
            query_bag_of_words.insert(word_pos.word, 1);
    }

//    foreach(QString word, query_bag_of_words.keys()){
//        double
//    }
}
