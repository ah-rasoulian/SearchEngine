#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <QObject>
#include <QFileDialog>

class Tokenizer : public QObject
{
    Q_OBJECT

public:
    Tokenizer();
    void find_files(QDir directory);

private:

    void tokenize(QFile *file);

    QStringList persian_punctuations = {};
    QStringList persian_plural_signs = {};

    QString linguistic_modules(QString word);
    QString remove_punctuations(QString word);
    QString remove_plural_signs(QString word);

signals:
    void show_message(QString message);
};

#endif // TOKENIZER_H
