#include "queryprocessor.h"

QueryProcessor::QueryProcessor(DatabaseHandler *database)
{
    this->database = database;
}

void QueryProcessor::search(QString query){
   emit show_message(query);
}
