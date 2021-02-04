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
        ordered_search(final_query, "simple");
    else if(mode.compare("ordered-champion") == 0)
        ordered_search(final_query, "champion");
    else if(mode.compare("clustered") == 0)
        ordered_search(final_query, "clustered");
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

void QueryProcessor::ordered_search(QList<query_words> final_query, QString mode){
    using namespace std;
    QHash<QString, unsigned long> query_bag_of_words;
    foreach(query_words word_pos, final_query){
        if(query_bag_of_words.contains(word_pos.word))
            query_bag_of_words.insert(word_pos.word, query_bag_of_words.value(word_pos.word) + 1);
        else
            query_bag_of_words.insert(word_pos.word, 1);
    }

    priority_queue< pair<double , unsigned long> > results;
    QSet<unsigned long> documents;

    // for all documents, the size of tf-idf in query is constant. So we don not calculate it.
    foreach(QString query_word, query_bag_of_words.keys()){
        if (database->word_exists(query_word)){
            if(mode.compare("simple") == 0){
                pair <unsigned long, unsigned long> doc_pos;
                foreach(doc_pos, database->get_postings_list(query_word))
                    documents.insert(doc_pos.first);

            }
            else if(mode.compare("champion") == 0){
                QMultiMap<unsigned long, unsigned long> freq_doc_champion_list = database->get_word_freq_doc_champion_list(query_word);
                QListIterator<unsigned long> freq_doc_champion_list_iterator(freq_doc_champion_list.uniqueKeys());
                freq_doc_champion_list_iterator.toBack();
                while (freq_doc_champion_list_iterator.hasPrevious()){
                    unsigned long freq = freq_doc_champion_list_iterator.previous();
                    if(freq < 2)    // here we put threshold to get only documents who has more than 2 of a such word
                        break;
                    foreach(unsigned long docID, freq_doc_champion_list.values(freq))
                        documents.insert(docID);
                }
            }
            else if(mode.compare("clustered") == 0){

                QString best_cluster;
                double best_cluster_tf_idf = -1.0;
                foreach(QString cluster, database->get_clusters_docID().keys()){

                    double sum_of_querry_mean_cluster_tf_idf_multiplication = 0;
                    foreach(QString query_word, query_bag_of_words.keys()){
                        double word_tf_idf_in_query = database->calculate_query_tf_idf(query_word, query_bag_of_words.value(query_word));
                        double word_tf_idf_in_cluster = database->get_cluster_word_tf_idf(query_word, cluster);
                        sum_of_querry_mean_cluster_tf_idf_multiplication += word_tf_idf_in_cluster * word_tf_idf_in_query;
                    }

                    double similarity = sum_of_querry_mean_cluster_tf_idf_multiplication / database->get_cluster_size_tf_idf(cluster);
                    if(similarity > best_cluster_tf_idf){
                        best_cluster = cluster;
                        best_cluster_tf_idf = similarity;
                    }
                }

                foreach(unsigned long docID, database->get_clusters_docID().values(best_cluster))
                    documents.insert(docID);
            }
        }
    }

    foreach(unsigned long docID, documents){
        double sum_of_query_document_tf_idf_multiplication = 0;
        foreach(QString query_word, query_bag_of_words.keys()){
            double word_tf_idf_in_query = database->calculate_query_tf_idf(query_word, query_bag_of_words.value(query_word));
            double word_tf_idf_in_document = database->get_docID_words_tf_idf(query_word, docID);
            sum_of_query_document_tf_idf_multiplication += word_tf_idf_in_document * word_tf_idf_in_query;
        }
        results.push(make_pair(sum_of_query_document_tf_idf_multiplication / database->get_docID_size_tf_idf(docID), docID));
    }

    int k = 20;
    while (!results.empty() && k > 0) {
        emit show_message(database->get_document_path(results.top().second));
        results.pop();
        k --;
    }
}

// /home/amirhossein/Data/University/Fall 2020/Information Retreival/docs
