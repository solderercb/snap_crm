// исходный код найден http://www.prog.org.ru/index.php?PHPSESSID=o85kfvo1uojris9soq8gug2642&topic=7629.msg41913#msg41913. Спасибо автору.
// это исправленный и дополненный код, найденный в том же топике ниже. Его автору тоже спасибо.

#ifndef AMOUNTTOWORDSRU_H
#define AMOUNTTOWORDSRU_H

#include <QString>

inline QString amountToWordsRU(double n)
{
    if(n < 0)
    {
        n = -n;
    }

    if( n > 999999999999.99 )
    {
        return QString::fromUtf8("Очень много денег ...");
    }

	static QString cap[4][10] =
	{
		{
			"",
			QString::fromUtf8("один "),
			QString::fromUtf8("два "),
			QString::fromUtf8("три "),
			QString::fromUtf8("четыре "),
			QString::fromUtf8("пять "),
			QString::fromUtf8("шесть "),
			QString::fromUtf8("семь "),
			QString::fromUtf8("восемь "),
			QString::fromUtf8("девять ")
		},
		{
			"",
			QString::fromUtf8("одна "),
			QString::fromUtf8("две "),
			QString::fromUtf8("три "),
			QString::fromUtf8("четыре "),
			QString::fromUtf8("пять "),
			QString::fromUtf8("шесть "),
			QString::fromUtf8("семь "),
			QString::fromUtf8("восемь "),
			QString::fromUtf8("девять ")
		},
		{
			"",
			"",
			QString::fromUtf8("двадцать "),
			QString::fromUtf8("тридцать "),
			QString::fromUtf8("сорок "),
			QString::fromUtf8("пятьдесят "),
			QString::fromUtf8("шестьдесят "),
			QString::fromUtf8("семьдесят "),
			QString::fromUtf8("восемьдесят "),
			QString::fromUtf8("девяносто ") 
		},
		{
			"",
			QString::fromUtf8("сто "),
			QString::fromUtf8("двести "),
			QString::fromUtf8("триста "), 
			QString::fromUtf8("четыреста "), 
			QString::fromUtf8("пятьсот "),
			QString::fromUtf8("шестьсот "),
			QString::fromUtf8("семьсот "),
			QString::fromUtf8("восемьсот "),
			QString::fromUtf8("девятьсот ")
		}
	};
 
	static QString cap2[10] =
	{
		QString::fromUtf8("десять "), 
		QString::fromUtf8("одиннадцать "), 
		QString::fromUtf8("двенадцать "), 
		QString::fromUtf8("тринадцать "), 
		QString::fromUtf8("четырнадцать "),
		QString::fromUtf8("пятнадцать "),
		QString::fromUtf8("шестнадцать "), 
		QString::fromUtf8("семнадцать "),
		QString::fromUtf8("восемнадцать "),
		QString::fromUtf8("девятнадцать ")
	};
 
	static QString cap3[5][3] =
	{
		{
			QString::fromUtf8("копейка "), 
			QString::fromUtf8("копейки "), 
			QString::fromUtf8("копеек ") 
		},
		{
			QString::fromUtf8("рубль, "), 
			QString::fromUtf8("рубля, "), 
			QString::fromUtf8("рублей, ") 
		},
		{
			QString::fromUtf8("тысяча "), 
			QString::fromUtf8("тысячи "), 
			QString::fromUtf8("тысяч ") 
		},
		{
			QString::fromUtf8("миллион "), 
			QString::fromUtf8("милиона "), 
			QString::fromUtf8("миллионов ") 
		},
		{
			QString::fromUtf8("миллиард "),
			QString::fromUtf8("миллиарда "),
			QString::fromUtf8("миллиардов ")
		}
	};
 
	QString s;
 
	qlonglong nn = ((qlonglong)(n)*10000 + (qlonglong)(n*1000) % 1000 + 5 ) / 10;
 
	qlonglong lo = 0;
	qlonglong hi = nn;
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
			s = QString::fromUtf8("ноль ") + cap3[1][2] + olds;
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
			s = QString::number(r[1]) + QString::number(r[0]) + " " + capt;
			continue;
		}
		if (nPor == 1)
			rod = 0;
 
		s += cap[2+1][r[2]];
		s += ((r[1] == 1) ? cap2[r[0]] : cap[1+1][r[1]]+cap[0+rod][r[0]])+capt;
		s += olds;
 
		if ((hi == 0) && (nPor >= 1))
			break;
	}
	QString up(s[0]);
	s.replace(0,1,up);
	return s;
}

#endif // AMOUNTTOWORDSRU_H
