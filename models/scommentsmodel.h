#ifndef SCOMMENTSMODEL_H
#define SCOMMENTSMODEL_H

#include <QObject>
#include "sstandarditemmodel.h"
#include "scommentmodel.h"

class SCommentsModel : public SStandardItemModel
{
    Q_OBJECT
public:
    explicit SCommentsModel(QObject *parent = nullptr);
    ~SCommentsModel();

signals:

};

#endif // SCOMMENTSMODEL_H
