#include "sclientmodel.h"

SClientModel::SClientModel(int id, QObject *parent)
{
}

SClientModel::~SClientModel()
{
}

void SClientModel::setPrimaryKey(const int id)
{
}

void SClientModel::createBalanceObj()
{
}

bool SClientModel::updateBalance(const double amount, const QString &text)
{
    return 1;
}

bool SClientModel::updateBalance(const double amount, const QString &text, const SBalanceLogRecordModel::RoyaltyReason reason, const int reason_id)
{
    return 1;
}


SBalanceLogRecordModel::SBalanceLogRecordModel(QObject *parent)
{
}

SBalanceLogRecordModel::SBalanceLogRecordModel(int client, QObject *parent):
    SBalanceLogRecordModel(parent)
{
}

SBalanceLogRecordModel::~SBalanceLogRecordModel()
{
}
