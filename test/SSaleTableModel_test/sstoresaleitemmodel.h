#ifndef SSTORESALEITEMMODEL_H
#define SSTORESALEITEMMODEL_H

class SStoreSaleItemModel
{
public:
    enum State{Active = 0, Cancelled = 1};
    enum Columns{C_id, C_count, C_price, C_sn, C_warranty, C_isRealization, C_returnPercent, C_state, C_itemId, C_inPrice, C_documentId, C_dealer, C_buyer,};
    bool sale(){return 1;};
    bool reserve(){return 1;};
    bool unsale(){return 1;};
    bool free(){return 1;};
    void set_unsaleReason(const QString&){};
};

#endif // SSTORESALEITEMMODEL_H
