#ifndef QUERYPROCESSOR_H
#define QUERYPROCESSOR_H

#include <QObject>
#include <databasehandler.h>

class QueryProcessor : public QObject
{
    Q_OBJECT

public:
    QueryProcessor(DatabaseHandler *database);
    void search(QString query);

private:
    DatabaseHandler *database;

signals:
    void show_message(QString message);
};

#endif // QUERYPROCESSOR_H
