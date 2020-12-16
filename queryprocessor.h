#ifndef QUERYPROCESSOR_H
#define QUERYPROCESSOR_H

#include <QObject>
#include <databasehandler.h>
#include <linguisticmodules.h>

typedef struct query_words{
    QString word;
    int position;
} query_words ;

class QueryProcessor : public QObject
{
    Q_OBJECT

public:
    QueryProcessor(DatabaseHandler *database, LinguisticModules *linguistic_modules);
    void search(QString query);

private:
    DatabaseHandler *database;
    LinguisticModules *linguistic_modules;

    QList<query_words> query_tokenizer(QString query);

signals:
    void show_message(QString message);
};

#endif // QUERYPROCESSOR_H
