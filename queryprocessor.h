#ifndef QUERYPROCESSOR_H
#define QUERYPROCESSOR_H

#include <QObject>
#include <databasehandler.h>
#include <linguisticmodules.h>
#include <queue>
#include <QList>
#include <QSet>

typedef struct query_words{
    QString word;
    int position;
} query_words ;

class QueryProcessor : public QObject
{
    Q_OBJECT

public:
    QueryProcessor(DatabaseHandler *database, LinguisticModules *linguistic_modules);
    void search(QString query, QString mode);

private:
    DatabaseHandler *database;
    LinguisticModules *linguistic_modules;

    QList<query_words> query_tokenizer(QString query);

    void simple_binary_search(QList<query_words> final_query);
    void ordered_search(QList<query_words> final_query);

signals:
    void show_message(QString message);
};

#endif // QUERYPROCESSOR_H
