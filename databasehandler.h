#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include <QObject>
#include <QHash>
#include <QMap>
#include <QList>

class DatabaseHandler : public QObject
{
    Q_OBJECT

public:
    DatabaseHandler();

    unsigned long get_document_number(QString document_path);
    QString get_document_path(unsigned long document_number);

    void indexer(QString word, unsigned long docID, unsigned long position);

    QList< std::pair<unsigned long, unsigned long> > get_postings_list(QString word);
    bool word_exists(QString word);

private:
    unsigned long number_of_documents;
    QHash<unsigned long, QString> documents_map_int_to_path;
    QHash<QString, unsigned long> documents_map_path_to_int;

    QMultiMap <QString, std::pair<unsigned long, unsigned long> > postings;

signals:
    void show_message(QString message);
};

#endif // DATABASEHANDLER_H
