#ifndef SCARTRIDGECARDMODEL_H
#define SCARTRIDGECARDMODEL_H

#include "scomrecord.h"
#include <QObject>
#include "scartridgematerialmodel.h"

class SCartridgeCardModel : public SComRecord
{
    Q_OBJECT
public:
    explicit SCartridgeCardModel(QObject *parent = nullptr);
    ~SCartridgeCardModel();
    int id();
    void load(const int id);
    void initMaterials();
    QString name();
    void setName(const QString&);
    int maker();
    void setMaker(const int);
    double fullWeight();
    void setFullWeight(const double);
    double tonerWeight();
    void setTonerWeight(const double);
    int resource();
    void setResource(const int);
    void setCreated(const int);
    int user();
    void setUser(const int);
    QString notes();
    void setNotes(const QString&);
    int photo();
    void setPhoto(const int);
    int color();
    void setColor(const int);
    bool archive();
    void setArchive(const bool);
    SCartridgeMaterialModel *material(const int type);
private:
    QString m_name;
    int m_maker;
    double m_fullWeight;
    double m_tonerWeight;
    int m_resource;
    int m_created;
    int m_user;
    QString m_notes;
    int m_photo;
    int m_color;
    bool m_archive;
    QMap<int, SCartridgeMaterialModel*> materials;
};

#endif // SCARTRIDGECARDMODEL_H
