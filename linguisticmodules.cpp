#include "linguisticmodules.h"

LinguisticModules::LinguisticModules()
{
    persian_punctuations << "!" << "،" << "؟" << "." << ":";

    persian_plural_signs << "ها";

    persian_stop_words << "از" << "در" << "با" << "و" << "من" << "تو" << "او" << "ما" << "شما" << "آن" << "آنها" << "برای" << "تا" << "به" << "را" << "این" << "هم" << "درباره" << "که" << "ولی" << "اما" << "";
}

QString LinguisticModules::word_normalizer(QString word){
    word = remove_punctuations(word);
    word = remove_plural_signs(word);
    return word;
}

QString LinguisticModules::remove_punctuations(QString word){
    foreach(QString punctuation, persian_punctuations){
        if (word.endsWith(punctuation) || word.startsWith(punctuation))
            word = word.remove(punctuation);
    }
    return word;
}

QString LinguisticModules::remove_plural_signs(QString word){
    foreach(QString sign, persian_plural_signs){
        if (word.endsWith(sign))
            word = word.remove(sign);
    }
    return word;
}

bool LinguisticModules::is_stop_word(QString word){
    foreach(QString stopword, persian_stop_words){
        if (word.compare(stopword) == 0)
            return true;
    }
    return false;
}
