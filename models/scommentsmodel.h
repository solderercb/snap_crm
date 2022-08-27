#ifndef SCOMMENTSMODEL_H
#define SCOMMENTSMODEL_H

#include <QObject>
#include "scommentmodel.h"

class SCommentsModel : public QObject
{
    Q_OBJECT
public:
    explicit SCommentsModel(QObject *parent = nullptr);
    ~SCommentsModel();

signals:

};

#endif // SCOMMENTSMODEL_H
