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

signals:
    void show_message(QString message);
};

#endif // TOKENIZER_H
