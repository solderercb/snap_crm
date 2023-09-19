#pragma once
#ifndef AMOUNTTOWORDS_H
#define AMOUNTTOWORDS_H

#include "global.h"
#include "models/scomsettings.h"
#include "amountToWordsRu.h"
#include "amountToWordsUa.h"
#include "amountToWordsBg.h"
#include "amountToWordsBy.h"
#include "amountToWordsCn.h"
#include "amountToWordsIl.h"
#include "amountToWordsKz.h"
#include "amountToWordsMd.h"
#include "amountToWordsPl.h"
#include "amountToWordsUs.h"
#include "amountToWordsUz.h"

QString amountToWords( double n );
QString amountToWords(double n, const QString &currencyCode);

class SAmountToWords
{
public:
    explicit SAmountToWords(){};
    static QString (*amountToWords_p)(double);
    static QString currencyCodeCached;
    static void init();
    static QString (*amountToWords(const QString &currencyCode))(double);
    static QString notImpl(double);
};


#endif // AMOUNTTOWORDS_H
