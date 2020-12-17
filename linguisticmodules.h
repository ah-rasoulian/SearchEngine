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
    QStringList persian_superior_signs = {};

    QString remove_punctuations(QString word);
    QString remove_extra_suffixes(QString word);
};

#endif // LINGUISTICMODULES_H
