#include "amountToWords.h"

QString SAmountToWords::currencyCodeCached = QString();
QString (*SAmountToWords::amountToWords_p)(double) = nullptr;

QString amountToWords( double n )
{
    if(!SAmountToWords::amountToWords_p)
    {
        SAmountToWords::init();
    }
    return SAmountToWords::amountToWords_p(n);
}

QString amountToWords(double n, const QString &currencyCode)
{
    return (*SAmountToWords::amountToWords(currencyCode))(n);
}

void SAmountToWords::init()
{
    currencyCodeCached = comSettings->currencyId;
    amountToWords_p = amountToWords(comSettings->currencyId);
}

QString (*SAmountToWords::amountToWords(const QString &currencyCode))(double)
{
    if(currencyCode.compare("RUB") == 0)
    {
        return &amountToWordsRU;
    }
    else if(currencyCode.compare("USD") == 0)
    {
        return &amountToWordsUS;
    }
    else if(currencyCode.compare("UAH") == 0)
    {
        return &amountToWordsUA;
    }
    else if(currencyCode.compare("BYN") == 0)
    {
        return &amountToWordsBY;
    }
    else if(currencyCode.compare("ILS") == 0)
    {
        return &amountToWordsIL;
    }
    else if(currencyCode.compare("MDL") == 0)
    {
        return &amountToWordsMD;
    }
    else if(currencyCode.compare("PLN") == 0)
    {
        return &amountToWordsPL;
    }
    else if(currencyCode.compare("CNY") == 0)
    {
        return &amountToWordsCN;
    }
    else if(currencyCode.compare("BGN") == 0)
    {
        return &amountToWordsBG;
    }
    else if(currencyCode.compare("UZS") == 0)
    {
        return &amountToWordsUZ;
    }
    else if(currencyCode.compare("KZT") == 0)
    {
        return &amountToWordsKZ;
    }

    return &notImpl;
}

QString SAmountToWords::notImpl(double)
{
    return "Converting not implemented";
}
