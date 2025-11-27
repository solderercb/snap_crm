#include "sclientmodel.h"

SClientModel::SClientModel(int, QObject*)
{
}

SClientModel::~SClientModel()
{
}

void SClientModel::setId(const int)
{
}

void SClientModel::createBalanceObj()
{
}

bool SClientModel::updateBalance(const double, const QString&)
{
    return 1;
}

bool SClientModel::updateBalance(const double, const QString&, const SBalanceLogRecordModel::RoyaltyReason, const int)
{
    return 1;
}


SBalanceLogRecordModel::SBalanceLogRecordModel(QObject*)
{
}

SBalanceLogRecordModel::SBalanceLogRecordModel(int, QObject *parent):
    SBalanceLogRecordModel(parent)
{
}

SBalanceLogRecordModel::~SBalanceLogRecordModel()
{
}
