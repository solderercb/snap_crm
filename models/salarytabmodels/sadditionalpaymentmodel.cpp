#include "sadditionalpaymentmodel.h"

SAdditionalPaymentModel::SAdditionalPaymentModel(QObject *parent) : SSingleRowJModel(parent)
{
    mapFields();

    i_tableName = "additional_payments";
    i_obligatoryFields << "name" << "payment_date" << "user" << "to_user" << "price";
}

bool SAdditionalPaymentModel::commit()
{
    return SSingleRowJModel::commit();
}

