#ifndef SREPAIRSALEITEMMODEL_H
#define SREPAIRSALEITEMMODEL_H

class SRepairSaleItemModel
{
public:
    enum State {Requested = 0, EngineerBasket = 1, RepairLinked = 2, Sold = 3, Archive = 4, New = 255};
    bool reserve(){return 1;};
    bool commit(){return 1;};
    bool sale(){return 1;};
    bool unsale(){return 1;};
    bool unlinkRepair(){return 1;};
    bool free(){return 1;};
};

#endif // SREPAIRSALEITEMMODEL_H
