#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <QObject>
#include <QFileDialog>
#include <databasehandler.h>
#include <linguisticmodules.h>

class Tokenizer : public QObject
{
    Q_OBJECT

public:
    Tokenizer(DatabaseHandler *database, LinguisticModules *linguistic_modules);
    void find_files(QDir directory);

private:

    void document_tokenize(QFile *file, unsigned long docID);

    DatabaseHandler *database;
    LinguisticModules *linguistic_modules;

signals:
    void show_message(QString message);
};

#endif // TOKENIZER_H
