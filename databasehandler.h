#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include <QObject>
#include <QHash>
#include <QMap>

typedef struct postings_list{
    QMultiMap <unsigned long, unsigned long> doc_position;
    unsigned long frequency;
} postings_list;

class DatabaseHandler : public QObject
{
    Q_OBJECT

public:
    DatabaseHandler();

    unsigned long get_document_number(QString document_path);
    QString get_document_path(unsigned long document_number);

    void indexer(QString word, unsigned long docID, unsigned long position);

private:
    unsigned long number_of_documents;
    QHash<unsigned long, QString> documents_map_int_to_path;
    QHash<QString, unsigned long> documents_map_path_to_int;

    QMap <QString, postings_list> postings;

signals:
    void show_message(QString message);
};

#endif // DATABASEHANDLER_H
