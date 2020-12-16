#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include <QObject>
#include <QHash>

class DatabaseHandler : public QObject
{
    Q_OBJECT
public:
    DatabaseHandler();

    unsigned long get_document_number(QString document_path);
    QString get_document_path(unsigned long document_number);

private:
    unsigned long number_of_documents;
    QHash<unsigned long, QString> documents_map_int_to_path;
    QHash<QString, unsigned long> documents_map_path_to_int;
};

#endif // DATABASEHANDLER_H
