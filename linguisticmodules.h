#ifndef LINGUISTICMODULES_H
#define LINGUISTICMODULES_H

#include <QObject>

class LinguisticModules : public QObject
{
    Q_OBJECT

public:
    LinguisticModules();
    bool is_stop_word(QString word);
    QString word_normalizer(QString word);

private:
    QStringList persian_punctuations = {};
    QStringList persian_plural_signs = {};
    QStringList persian_stop_words = {};

    QString remove_punctuations(QString word);
    QString remove_plural_signs(QString word);
};

#endif // LINGUISTICMODULES_H
