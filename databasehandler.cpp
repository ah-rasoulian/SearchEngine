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
