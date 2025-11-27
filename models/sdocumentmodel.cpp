#include "sdocumentmodel.h"
#include <ProjectGlobals>
#include <SUserSettings>
#include <SStandardItemModel>
#include <SSqlQueryModel>
#include <SLogRecordModel>

SDocumentModel::SDocumentModel(QObject *parent) : SSingleRowJModel(parent)
{
    mapFields();

    i_obligatoryFields << "company" << "user" << "total" << "created" << "office" << "img1" << "img2" << "img3";
    i_tableName = "docs";
    setPrimaryKeyIndex(0);
    i_logRecord->set_type(SLogRecordModel::Doc);
    connect(this, &SSingleRowModel::beginDataChange, this, &SDocumentModel::setDataRework);
}

SDocumentModel::SDocumentModel(int id, QObject *parent) : SDocumentModel(parent)
{
    load(id);
}

SDocumentModel::~SDocumentModel()
{
}

void SDocumentModel::load(int id)
{
    setPrimaryKey(id);
    SSingleRowJModel::load();
}

void SDocumentModel::setOfficeIndex(int index)
{
    set_office(officesModel->databaseIDByRow(index, "id"));
}

void SDocumentModel::unsetClient()
{
    setData(C_client, QVariant());
}

int SDocumentModel::paymentSystemIndex()
{
    return paymentSystemsModel->rowByDatabaseID(paymentSystem(), "system_id");
}

QString SDocumentModel::amountLocal()
{
    return sysLocale.toString(amount(), 'f', 2);
}

QString SDocumentModel::currencyRateLocal()
{
    return sysLocale.toString(currencyRate(), 'f', 4);
}

int SDocumentModel::companyIndex()
{
    return companiesModel->rowByDatabaseID(company(), "id");
}

void SDocumentModel::setCompanyIndex(int index)
{
    set_company(companiesModel->databaseIDByRow(index, "id"));
}

int SDocumentModel::priceOptionIndex()
{
    return priceColModel->rowByDatabaseID(priceOption(), "id");
}

void SDocumentModel::trackingNumberChanged(const QString &track)
{
    auto old = commitedData(C_trackingNumber).value_or(QVariant()).toString();
    if(!old.isEmpty())
    {
        if(track.isEmpty())
            appendLogText(tr("Номер ТТН для РН №%1 удалён").arg(id()));
        else
            appendLogText(tr("Номер ТТН для РН №%1 изменён с %2 на %3").arg(id()).arg(old, track));

    }
    else
        appendLogText(tr("РН №%1 присвоен номер ТТН: %2").arg(id()).arg(track));
}

bool SDocumentModel::commit()
{
    if(!isPrimaryKeyValid())
    {
        initMandatoryField(C_img1, QVariant());
        initMandatoryField(C_img2, QVariant());
        initMandatoryField(C_img3, QVariant());
        initMandatoryField(C_company, userDbData->company());
        initMandatoryField(C_office, userDbData->currentOffice());
        initMandatoryField(C_user, userDbData->id());
        initMandatoryField(C_created, QDateTime::currentDateTime());
    }

    return SSingleRowJModel::commit();
}

QString SDocumentModel::title()
{
    QString ret;
    switch (type())
    {
        case InInvoice: ret = tr("Приходная накладная"); break;
        case OutInvoice: ret = tr("Расходная накладная"); break;
        case WriteOff: ret = tr("Списание товара"); break;
        case MoveInvoice: ret = tr("Накладнае перемещения"); break;
        case Bill: ret = tr("Счёт"); break;
        case ReserveInvoice: ret = tr("Резерв товара"); break;
        case Buyout: ret = tr("Выкуп техники"); break;
    };
    return ret;
}

void SDocumentModel::updateLogAssociatedRecId()
{
    i_logRecord->set_document(id());
}

void SDocumentModel::setDataRework(const int index, QVariant &data)
{
    switch(index)
    {
        case C_client: if(!data.toInt()) {data = QVariant(); i_logRecord->unsetClient();} break;
        default: ;
    }
}

void SDocumentModel::logDataChange(const int index, const QVariant &data)
{
    switch (index)
    {
        case C_trackingNumber: trackingNumberChanged(data.toString()); break;
        default: break;
    }
}
