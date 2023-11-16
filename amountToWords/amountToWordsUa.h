// исходный код найден http://www.prog.org.ru/index.php?PHPSESSID=o85kfvo1uojris9soq8gug2642&topic=7629.msg41913#msg41913. Спасибо автору.
#ifndef AMOUNTTOWORDSUA_H
#define AMOUNTTOWORDSUA_H

#include <QString>

inline QString amountToWordsUA(double n)
{
    if(n < 0)
    {
        n = -n;
    }

    if( n > 999999999999.99 )
    {
        return QString::fromUtf8("Дуже багато грошей ...");
    }

    static QString cap[4][10] =
    {
        {
            "",
            QString::fromUtf8("один "),
            QString::fromUtf8("два "),
            QString::fromUtf8("три "),
            QString::fromUtf8("чотири "),
            QString::fromUtf8("п'ять "),
            QString::fromUtf8("шість "),
            QString::fromUtf8("сім "),
            QString::fromUtf8("вісім "),
            QString::fromUtf8("дев'ять ")
        },
        {
            "",
            QString::fromUtf8("одна "),
            QString::fromUtf8("дві "),
            QString::fromUtf8("три "),
            QString::fromUtf8("чотири "),
            QString::fromUtf8("п'ять "),
            QString::fromUtf8("шість "),
            QString::fromUtf8("сім "),
            QString::fromUtf8("вісім "),
            QString::fromUtf8("дев'ять ")
        },
        {
            "",
            "",
            QString::fromUtf8("двадцять "),
            QString::fromUtf8("тридцять "),
            QString::fromUtf8("сорок "),
            QString::fromUtf8("п'ятдесят "),
            QString::fromUtf8("шістдесят "),
            QString::fromUtf8("сімдесят "),
            QString::fromUtf8("вісімдесят "),
            QString::fromUtf8("дев'яносто ")
        },
        {
            "",
            QString::fromUtf8("сто "),
            QString::fromUtf8("двісті "),
            QString::fromUtf8("триста "),
            QString::fromUtf8("чотириста "),
            QString::fromUtf8("п'ятсот "),
            QString::fromUtf8("шістсот "),
            QString::fromUtf8("сімсот "),
            QString::fromUtf8("вісімсот "),
            QString::fromUtf8("дев'ятсот ")
        }
    };

    static QString cap2[10] =
    {
        QString::fromUtf8("десять "),
        QString::fromUtf8("одинадцять "),
        QString::fromUtf8("дванадцять "),
        QString::fromUtf8("тринадцять "),
        QString::fromUtf8("чотирнадцять "),
        QString::fromUtf8("п'ятнадцять "),
        QString::fromUtf8("шістнадцять "),
        QString::fromUtf8("сімнадцять "),
        QString::fromUtf8("вісімнадцять "),
        QString::fromUtf8("дев'ятнадцять ")
    };

    static QString cap3[5][3] =
    {
        {
            QString::fromUtf8("коп."),
            QString::fromUtf8("коп."),
            QString::fromUtf8("коп.")
        },
        {
            QString::fromUtf8("гривня, "),
            QString::fromUtf8("гривні, "),
            QString::fromUtf8("гривень, ")
        },
        {
            QString::fromUtf8("тисяча "),
            QString::fromUtf8("тисячі "),
            QString::fromUtf8("тисяч ")
        },
        {
            QString::fromUtf8("мільйон "),
            QString::fromUtf8("мильйони "),
            QString::fromUtf8("мільйонів ")
        },
        {
            QString::fromUtf8("мільярд "),
            QString::fromUtf8("мільярди "),
            QString::fromUtf8("мільярдів ")
        }
    };

    QString s;

    long nn = ((int)(n)*10000 + (int)(n*1000) % 1000 + 5 ) / 10;

    long lo = 0;
    long hi = nn;
    int r[3];
    int rod;

    for ( int nPor = 0; true; ++nPor )
    {
        QString olds = s;
        s = "";

        lo = hi%1000;
        hi = hi/1000;

        if ((nPor == 1) && (lo == 0) && (hi == 0))
        {
            s = "нуль " + cap3[1][2] + olds;
            break;
        }

        for ( int nRaz = 0; nRaz < 3; ++nRaz )
        {
            r[nRaz] = lo%10;
            lo      = lo/10;
        }

        switch ( nPor )
        {
            case 0:
            case 1:
            case 2:
                rod = 1;
                break;
            default:
                rod = 0;
        }

        QString capt;
        if (r[1] != 1)
        {
            if (r[0] == 1)
                capt = cap3[nPor][0];
            else if ((r[0] > 1 ) && (r[0] < 5))
                capt = cap3[nPor][1];
        }
        if (capt.isEmpty())
            capt = cap3[nPor][2];

        if (nPor == 0)
        {
            s = QString::number(r[1]) + QString::number(r[0]) + 0x00A0 + capt;  // 0x00A0 — неразрывный пробел
            continue;
        }

        s += cap[2+1][r[2]];
        s += ((r[1] == 1) ? cap2[r[0]] : cap[1+1][r[1]]+cap[0+rod][r[0]])+capt;
        s += olds;

        if ((hi == 0) && (nPor >= 1))
            break;
    }

    s.replace(0,1,s[0].toUpper());
    return s;
}

#endif // AMOUNTTOWORDSUA_H
