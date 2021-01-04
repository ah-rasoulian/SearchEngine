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

void DatabaseHandler::indexer(QString word, unsigned long docID, unsigned long position){
    postings.insert(word, std::make_pair(docID, position));
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

double DatabaseHandler::calculate_tf_idf(QString word, long long docID){
    using namespace std;
    QSet<long long> documents;
    long frequency_in_document = 0;

    pair<long long, long long> doc_pos_tmp;
    foreach(doc_pos_tmp, postings.values(word)){
        documents.insert(doc_pos_tmp.first);
        if(doc_pos_tmp.first == docID)
            frequency_in_document ++;
    }

    return (1 + log10(frequency_in_document)) * log10(number_of_documents / documents.size());
}
