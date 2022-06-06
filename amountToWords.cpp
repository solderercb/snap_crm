// исходный код найден http://www.prog.org.ru/index.php?PHPSESSID=o85kfvo1uojris9soq8gug2642&topic=7629.msg41913#msg41913. Спасибо автору.
// "ЗЫ: Писалось под Украину, на библиотеке Qt3 for PlamOS, поэтому будут необходимы незначительные изменения в коде."
// Исправлен числительные, удалены  или заменены упраздённые функции; проверен с QT6
#include "amountToWords.h"

QString amountToWords( double n )
{
	static QString cap[4][10] =
	{
		{
			"",
			"один ",
			"два ",
			"три ",
			"чотири ",
			"п'ять ",
			"шість ",
			"сім ",
			"вісім ",
			"дев'ять "
        },
		{
			"",
			"одна ",
			"дві ",
			"три ",
			"чотири ",
			"п'ять ",
			"шість ",
			"сім ",
			"вісім ",
			"дев'ять "
		},
		{
			"",
			"",
			"двадцять ",
			"тридцять ",
			"сорок ",
			"п'ятдесят ",
			"шістдесят ",
			"сімдесят ",
			"вісімдесят ",
			"дев'яносто "
		},
		{
			"",
			"сто ",
			"двісти ",
			"триста ",
			"чотириста ",
			"п'ятсот ",
			"шістсот ",
			"сімсот ",
			"вісімсот ",
			"дев'ятсот "
		}
	};
	
	static QString cap2[10] =
	{
		"десять ",
		"одинадцять ",
		"дванадцять ",
		"тринадцять ",
		"чотирнадцять ",
		"п'ятнадцять ",
		"шістнадцять ",
		"сімнадцять ",
		"вісімнадцять ",
		"дев'ятнадцять "
	};

	static QString cap3[5][3] =
	{
		{
            "коп.",
            "коп.",
            "коп."
		},
		{
			"гривня, ",
			"гривні, ",
			"гривень, "
		},
		{
			"тисяча ",
			"тисячі ",
			"тисяч "
		},
		{
            "мільйон ",
            "мильйони ",
            "мільйонів "
		},
		{
            "мільярд ",
			"мільярди ",
			"мільярдів "
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
            s = QString::number(r[1]) + QString::number(r[0]) + capt;
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
