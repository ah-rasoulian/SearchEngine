#include "databasehandler.h"

DatabaseHandler::DatabaseHandler()
{
    number_of_documents = 0;
}

unsigned long DatabaseHandler::get_document_number(QString document_path){
    if(documents_map_path_to_int.contains(document_path))
        return documents_map_path_to_int.value(document_path);
    else{
        documents_map_path_to_int.insert(document_path, number_of_documents);
        documents_map_int_to_path.insert(number_of_documents, document_path);

        return number_of_documents ++;
    }
}

QString DatabaseHandler::get_document_path(unsigned long document_number){
    return documents_map_int_to_path.value(document_number);
}

void DatabaseHandler::indexer(QString word, unsigned long docID, unsigned long position, QString cluster){
    using namespace std;
    if(cluster.compare("") != 0)
        cluster_doc.insert(cluster, docID);


    postings.insert(word, make_pair(docID, position));

    if(docID_bag_of_words.contains(docID)){
        pair<QString, unsigned long> word_freq_tmp;
        foreach(word_freq_tmp, docID_bag_of_words.values(docID)){
            if(word_freq_tmp.first.compare(word) == 0){
                unsigned long new_freq = word_freq_tmp.second + 1;
                docID_bag_of_words.remove(docID, word_freq_tmp);
                docID_bag_of_words.insert(docID, make_pair(word, new_freq));
                break;
            }
        }
    }
    else
        docID_bag_of_words.insert(docID, make_pair(word, 1));
}

QList<std::pair<unsigned long, unsigned long> > DatabaseHandler::get_postings_list(QString word){
    return postings.values(word);
}

bool DatabaseHandler::word_exists(QString word){
    if (postings.contains(word))
        return true;
    else
        return false;
}

void DatabaseHandler::calculate_word_collection_frequency(){
    using namespace std;
    foreach(QString word, postings.uniqueKeys()){
        QSet<unsigned long> documents;
        pair<unsigned long, unsigned long> doc_pos_tmp;
        foreach(doc_pos_tmp, postings.values(word))
            documents.insert(doc_pos_tmp.first);
        word_collection_frequency.insert(word, documents.size());
    }
}

void DatabaseHandler::calculate_document_tf_idf(){
    using namespace std;
    calculate_word_collection_frequency();

    for(unsigned long docID = 0; docID < number_of_documents; docID++){
        pair<QString, unsigned long> word_freq_tmp;
        foreach(word_freq_tmp, docID_bag_of_words.values(docID)){
            double tf_idf = (1 + log10(word_freq_tmp.second)) * log10(number_of_documents / word_collection_frequency.value(word_freq_tmp.first));
            docID_words_tf_idf.insert(docID, make_pair(word_freq_tmp.first, tf_idf));
        }
    }

    for(unsigned long docID = 0; docID < number_of_documents; docID++){
        double sum_square_words_tf_idf = 0;
        pair<QString, double> word_and_tf_idf;
        foreach(word_and_tf_idf, docID_words_tf_idf.values(docID)){
            sum_square_words_tf_idf += pow(word_and_tf_idf.second, 2);
        }
        docID_size_tf_idf.insert(docID, sqrt(sum_square_words_tf_idf));
    }
}

double DatabaseHandler::calculate_query_tf_idf(QString word,unsigned long frequency_in_query){
    if (word_collection_frequency.contains(word))
        return (1 + log10(frequency_in_query)) * log10(number_of_documents / word_collection_frequency.value(word));
    else
        return 0;
}

double DatabaseHandler::get_docID_words_tf_idf(QString word, unsigned long docID){
    std::pair<QString, double> words_tf_idf_tmp;
    foreach(words_tf_idf_tmp, docID_words_tf_idf.values(docID)){
        if(words_tf_idf_tmp.first.compare(word) == 0)
            return words_tf_idf_tmp.second;
    }
    return 0;
}

double DatabaseHandler::get_docID_size_tf_idf(unsigned long docID){
    return docID_size_tf_idf.value(docID);
}

void DatabaseHandler::make_champion_list(){
    using namespace std;
    foreach(QString word, postings.uniqueKeys()){
        QMap<unsigned long, unsigned long> docID_frequency_tmp;
        pair<unsigned long, unsigned long> doc_pos_tmp;
        foreach(doc_pos_tmp, postings.values(word)){
            if(docID_frequency_tmp.contains(doc_pos_tmp.first))
                docID_frequency_tmp.insert(doc_pos_tmp.first, docID_frequency_tmp.value(doc_pos_tmp.first) + 1);
            else
                docID_frequency_tmp.insert(doc_pos_tmp.first, 1);
        }
        QMultiMap<unsigned long, unsigned long> freq_docID_tmp;
        foreach(unsigned long docID, docID_frequency_tmp.keys())
            freq_docID_tmp.insert(docID_frequency_tmp.value(docID), docID);

        word_freq_doc_chamption_list.insert(word, freq_docID_tmp);
    }
}

QMultiMap<unsigned long, unsigned long> DatabaseHandler::get_word_freq_doc_champion_list(QString word){
    return word_freq_doc_chamption_list.value(word);
}

void DatabaseHandler::calculate_clusters_mean(){
    using namespace std;

    foreach(QString cluster, cluster_doc.uniqueKeys() ){

        int cluster_size = cluster_doc.values(cluster).size();
        QMap <QString, double> word_mean_tf_idf;

        foreach(unsigned long docID, cluster_doc.values(cluster)){
            pair<QString, double> word_tf_idf_tmp;
            foreach(word_tf_idf_tmp, docID_words_tf_idf.values(docID)){

                if(word_mean_tf_idf.contains(word_tf_idf_tmp.first))
                    word_mean_tf_idf.insert(word_tf_idf_tmp.first, word_mean_tf_idf.value(word_tf_idf_tmp.first) + word_tf_idf_tmp.second);
                else
                    word_mean_tf_idf.insert(word_tf_idf_tmp.first, word_tf_idf_tmp.second);
            }
        }

        foreach(QString word, word_mean_tf_idf.keys()){
            word_mean_tf_idf.insert(word, word_mean_tf_idf.value(word) / cluster_size);
        }

        cluster_words_mean_tf_idf.insert(cluster, word_mean_tf_idf);
    }

    foreach(QString cluster, cluster_doc.uniqueKeys()){
        double sum_square_words_tf_idf = 0;
        foreach(double word_tf_idf, cluster_words_mean_tf_idf.value(cluster).values()){
            sum_square_words_tf_idf += pow(word_tf_idf, 2);
        }
        cluster_size_tf_idf.insert(cluster, sqrt(sum_square_words_tf_idf));
    }

}

QMultiHash<QString, unsigned long> DatabaseHandler::get_clusters_docID(){
    return cluster_doc;
}

double DatabaseHandler::get_cluster_word_tf_idf(QString cluster, QString word){
    if (cluster_words_mean_tf_idf.value(cluster).contains(word) )
        return cluster_words_mean_tf_idf.value(cluster).value(word);
    else
        return 0;
}

double DatabaseHandler::get_cluster_size_tf_idf(QString cluster){
    return cluster_size_tf_idf.value(cluster);
}

// /home/amirhossein/Data/University/Fall 2020/Information Retreival/docs

