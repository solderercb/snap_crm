#ifndef SCARTRIDGECARDMODEL_H
#define SCARTRIDGECARDMODEL_H

#include "scomrecord.h"
#include <QObject>
#include "scartridgematerialmodel.h"
#include "widgets/shortlivednotification.h"

class SCartridgeCardModel : public SComRecord
{
    Q_OBJECT
public:
    explicit SCartridgeCardModel(QObject *parent = nullptr);
    ~SCartridgeCardModel();
    int id();
    void load(const int id);
    void loadError();
    void initMaterials();
    bool commit();
    QString name();
    void setName(const QString&);
    int vendor();
    void setVendor(const int vendor);
    double fullWeight();
    void setFullWeight(const double);
    double tonerWeight();
    void setTonerWeight(const double);
    int resource();
    void setResource(const int);
    void setCreated(const QDateTime);
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
    bool isMaterialSet(const int type);
private:
    QString m_name;
    int m_vendor = 0;
    double m_fullWeight = 0;
    double m_tonerWeight = 0;
    int m_resource = 0;
    QDateTime m_created;
    int m_user = 0;
    QString m_notes;
    int m_photo = 0;
    int m_color = 0;
    bool m_archive = 0;
    QMap<int, SCartridgeMaterialModel*> m_materials;
};

#endif // SCARTRIDGECARDMODEL_H
