#ifndef SCARTRIDGECARDMODEL_H
#define SCARTRIDGECARDMODEL_H

#include <SWorkModel>

class Material
{
public:
    Material(){};
    ~Material(){};
    double salarySumm(){return 123.45;};
};

class SCartridgeCardModel
{
public:
    SCartridgeCardModel(){m_material = new Material();};
    ~SCartridgeCardModel(){delete m_material;};
    Material* material(SWorkModel::Type){return m_material;};
private:
    Material *m_material;
};

#endif // SCARTRIDGECARDMODEL_H
