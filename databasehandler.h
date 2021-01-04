#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include <QObject>
#include <QHash>
#include <QMap>
#include <QList>
#include <QSet>
#include <math.h>


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

    double calculate_query_tf_idf(QString word, long frequency_in_query);
    void calculate_document_tf_idf();


private:
    unsigned long number_of_documents;
    QHash<unsigned long, QString> documents_map_int_to_path;
    QHash<QString, unsigned long> documents_map_path_to_int;

    QMultiMap <QString, std::pair<unsigned long, unsigned long> > postings;

    QMultiMap <unsigned long, std::pair<QString, unsigned long> > docID_bag_of_words;
    QMap <QString, unsigned long> word_collection_frequency;
    QMultiMap <unsigned long, std::pair<QString, double> > docID_words_tf_idf;
    QMap <unsigned long, double> docID_size_tf_idf;

    void calculate_word_collection_frequency();


signals:
    void show_message(QString message);
};

#endif // DATABASEHANDLER_H
