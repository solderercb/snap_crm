#include "sprintposreport.h"

SPrintPOSReport::SPrintPOSReport()
{

}

SPrintPOSReport::~SPrintPOSReport()
{
    closePrinter();
}

void SPrintPOSReport::setClientModel(SClientModel *client)
{
    m_client = client;
}

// TODO: редактор отчета
void SPrintPOSReport::addPrintJob(SRepairModel *repair)
{
    if(printer)
    {
        SSaleTableModel *worksAndPartsModel = nullptr;
        QModelIndex index;

        switch(repair->state())
        {
            case Global::RepStateIds::Returned:
            case Global::RepStateIds::ReturnedNoRepair:
            case Global::RepStateIds::ReturnedInCredit: worksAndPartsModel = new SSaleTableModel(); worksAndPartsModel->repair_loadTable(repair->id()); break;
            default: ;
        }

        printer->startSpool("job");
        printer->startPage();
        *printer << EscPosQt::EscPosPrinter::init << EscPosQt::EscPosPrinter::EncodingPC866
                 << EscPosQt::EscPosPrinter::PrintModes()
                 << EscPosQt::EscPosPrinter::lineSpacing(24);
        *printer << EscPosQt::EscPosPrinter::PrintModes(EscPosQt::EscPosPrinter::PrintModeDoubleWidth)
          << EscPosQt::EscPosPrinter::JustificationCenter
          << officesModel->value(userDbData->currentOffice, "id", "name").toString()
          << "\n";

        *printer << EscPosQt::EscPosPrinter::PrintModes()
                 << EscPosQt::EscPosPrinter::JustificationCenter
                 << companiesModel->value(userDbData->company, "id", "name").toString()
                 << "\n"
                 << QStringLiteral("ИНН %1").arg(companiesModel->value(userDbData->company, "id", "inn").toString())
                 << "\n"
                 << officesModel->value(userDbData->currentOffice, "id", "address").toString();

        *printer << EscPosQt::EscPosPrinter::feed(1);
        *printer << EscPosQt::EscPosPrinter::PrintModes(EscPosQt::EscPosPrinter::PrintModeDoubleWidth | EscPosQt::EscPosPrinter::PrintModeEmphasized)
                 << EscPosQt::EscPosPrinter::JustificationRight
                 << QString("РЕМОНТ №%1\n").arg(repair->id());
        *printer << EscPosQt::EscPosPrinter::PrintModes();

        *printer << EscPosQt::EscPosPrinter::JustificationLeft
          << EscPosQt::EscPosPrinter::formatAs2CTable("ДАТА ПРИЁМА", repair->inDate());

        *printer << EscPosQt::EscPosPrinter::formatAs2CTable("КЛИЕНТ", m_client->fullShortName())
                 << EscPosQt::EscPosPrinter::formatAs2CTable(" ", repair->title());

        *printer << EscPosQt::EscPosPrinter::PrintModes(EscPosQt::EscPosPrinter::PrintModeDoubleWidth | EscPosQt::EscPosPrinter::PrintModeDoubleHeight | EscPosQt::EscPosPrinter::PrintModeEmphasized)
                 << EscPosQt::EscPosPrinter::JustificationRight
                 << QString("%1").arg(repair->serialNumber())
                 << "\n";
        *printer << EscPosQt::EscPosPrinter::PrintModes();


        if (worksAndPartsModel)
        {
            *printer << EscPosQt::EscPosPrinter::feed(1);
            *printer << EscPosQt::EscPosPrinter::PrintModes(EscPosQt::EscPosPrinter::PrintModeEmphasized)
                 << EscPosQt::EscPosPrinter::JustificationCenter
                 << QStringLiteral("ВЫПОЛНЕННЫЕ РАБОТЫ\n");

            *printer << EscPosQt::EscPosPrinter::PrintModes()
                 << EscPosQt::EscPosPrinter::JustificationLeft;
            for(int i = 0; i < worksAndPartsModel->rowCount(); i++)
            {
                index = worksAndPartsModel->index(i, SStoreItemModel::SaleOpColumns::ColName);
                *printer << EscPosQt::EscPosPrinter::formatAs2CTable(
                                index.data().toString(),
                                QString("x%1  =%2") \
                                    .arg(index.siblingAtColumn(SStoreItemModel::SaleOpColumns::ColCount).data().toInt()) \
                                    .arg(index.siblingAtColumn(SStoreItemModel::SaleOpColumns::ColSumm).data().toString()));
            }

            *printer << EscPosQt::EscPosPrinter::PrintModes(EscPosQt::EscPosPrinter::PrintModeDoubleWidth | EscPosQt::EscPosPrinter::PrintModeEmphasized)
                 << EscPosQt::EscPosPrinter::JustificationLeft
                 << "\n"
                 << QString("ИТОГ:\n");
            *printer << EscPosQt::EscPosPrinter::PrintModes(EscPosQt::EscPosPrinter::PrintModeDoubleWidth | EscPosQt::EscPosPrinter::PrintModeDoubleHeight | EscPosQt::EscPosPrinter::PrintModeEmphasized)
                 << EscPosQt::EscPosPrinter::JustificationRight
                 << "=" + worksAndPartsModel->amountTotalLocale() + "\n";

        }

        *printer << EscPosQt::EscPosPrinter::feed(1);
        *printer << EscPosQt::EscPosPrinter::PrintModes()
                 << EscPosQt::EscPosPrinter::JustificationCenter;

        if (worksAndPartsModel)
        {
            *printer << QString("--------------------------------\n");
            *printer << EscPosQt::EscPosPrinter::PrintModes(EscPosQt::EscPosPrinter::PrintModeDoubleWidth)
                     << QString("НЕФИСКАЛЬНЫЙ\n");
            *printer << EscPosQt::EscPosPrinter::PrintModes();
            *printer << QString("--------------------------------\n");
            *printer << EscPosQt::EscPosPrinter::feed(1);

            delete worksAndPartsModel;
        }

        *printer << QString("СПАСИБО, ЧТО ВЫБРАЛИ НАС !\n")
                 << QString("ТЕЛ. %1\n").arg(officesModel->value(userDbData->currentOffice, "id", "phone").toString())
                 << QString("ТЕЛ. %1\n").arg(officesModel->value(userDbData->currentOffice, "id", "phone2").toString())
                 << companiesModel->value(userDbData->company, "id", "site").toString();
        *printer << EscPosQt::EscPosPrinter::feed(4);
//        *printer << EscPosQt::EscPosPrinter::partialCut;

        printer->endPage();
        printer->endSpool();
    }
}

void SPrintPOSReport::openPrinter(const QString &name)
{
    printer = new EscPosQt::EscPosPrinter(name);
    if(!printer->open())
    {
        qDebug() << "Can't open printing device " << name;
        delete printer;
        printer = nullptr;
    }
}

void SPrintPOSReport::closePrinter()
{
    if(printer)
    {
        printer->close();
        delete printer;
        printer = nullptr;
    }
}
