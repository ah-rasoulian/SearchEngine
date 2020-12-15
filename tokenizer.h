#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <QObject>
#include <QFileDialog>

class Tokenizer : public QObject
{
    Q_OBJECT

public:
    Tokenizer();
    void tokenize(QDir directory);

signals:
    void show_message(QString message);
};

#endif // TOKENIZER_H
