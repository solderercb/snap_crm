#include "global.h"
#include "getOutDialog.h"
#include "ui_getOutDialog.h"
#include "tabrepair.h"

getOutDialog::getOutDialog(QWidget *p, Qt::WindowFlags f) :
    QWidget(p, f),
    ui(new Ui::getOutDialog),
    parent(p)
{
    QSqlQuery query(QSqlDatabase::database("connMain"));
    float worksAndSparePartsSumm;
    ui->setupUi(this);
    setDefaultStyleSheets();
    connect(ui->checkBoxCreditPayment, SIGNAL(stateChanged(int)), this, SLOT(enableComboBoxPaymentAccount(int)));
    connect(ui->comboBoxRejectReason, SIGNAL(currentTextChanged(QString)), this, SLOT(otheRejectReasonShow(QString)));
    connect(ui->textEditRejectReason, SIGNAL(textChanged()), this, SLOT(textEditTextChanged()));
    tabRepair *parentTab = static_cast<tabRepair*>(parent);
    state = parentTab->repairModel->record(0).value("state").toInt();
    ui->textEditRejectReason->setHidden(true);
    if (state != 7)// TODO:нужен более гибкий способ определения кода статуса
    {
        newState = 8;
        ui->comboBoxRejectReason->setHidden(true);  // если статус "Готово к выдаче", то элементы выбора/ввода причины отказа скрываем
        ui->checkBoxConfirmGetOut->setHidden(true);
    }
    else
    {
        newState = 12;
        ui->comboBoxRejectReason->setModel(rejectReasonModel);
        ui->comboBoxRejectReason->setCurrentIndex(-1);
        ui->checkBoxPaymentCheckout->setHidden(true);
    }
    ui->comboBoxPaymentAccount->setStyleSheet(commonComboBoxStyleSheet);
    if(parentTab->clientModel->record(0).value("balance_enable").toBool() && permissions->contains("64"))
    {
        query.exec(QUERY_SEL_IS_BALANCE_EN(parentTab->repairModel->record(0).value("client").toInt()));
        query.first();
        // проверка суммы чисел из таблицы balance с числом в таблице clients для клиента
        if( query.value(0).toFloat() != parentTab->clientModel->record(0).value("balance").toFloat() )
        {
            ui->pushButtonGetout->setDisabled(true);
        }
        // проверка достаточности средств на балансе клиента
        if( query.value(0).toFloat() >= parentTab->total_sum )
        {
            balanceNotEnough = 0;
        }
    }
    else
        ui->checkBoxCreditPayment->setEnabled(false);  // TODO: Нужен более универсальный способ проверки права выдачи в долг

    ui->lineEditPrepay->setText(sysLocale.toString(parentTab->repairModel->record(0).value("prepaid_summ").toFloat(), 'f', 2));
    ui->lineEditAgreedAmount->setText(sysLocale.toString(parentTab->repairModel->record(0).value("repair_cost").toFloat(), 'f', 2));
    ui->lineEditAlreadyPayed->setText(sysLocale.toString(parentTab->repairModel->record(0).value("prepaid_summ").toFloat(), 'f', 2));  // назначение данного поля не понятно, оно вроде бы, всегда равно полю Предоплата
    ui->lineEditTotalAmount->setText(sysLocale.toString(parentTab->total_sum, 'f', 2));
    ui->lineEditCurrentPaymentAmount->setText(sysLocale.toString(parentTab->total_sum - parentTab->repairModel->record(0).value("prepaid_summ").toFloat(), 'f', 2));
    ui->comboBoxPaymentAccount->setModel(paymentSystemsModel);
    ui->comboBoxPaymentAccount->setCurrentText("Наличные");
    ui->labelIssuedMessage->setText(parentTab->repairModel->record(0).value("issued_msg").toString());
    if(ui->labelIssuedMessage->text() == "")
        ui->labelIssuedMessage->setHidden(true);
    if(parentTab->repairModel->record(0).value("repair_cost").toFloat() > parentTab->total_sum) // если сумма работ и деталей меньше согласованной, отобразится сообщение
        summsNotEq = 1;

    // Деньги это важно, поэтому перепроверяем суммы
    query.exec(QUERY_SEL_REPAIR_PREPAYS(parentTab->repair_id));
    query.first();
    if(query.value(0).toFloat() != parentTab->repairModel->record(0).value("prepaid_summ").toFloat())
        ui->pushButtonGetout->setDisabled(true);
    query.exec(QUERY_SEL_REPAIR_WORKS(parentTab->repair_id));
    query.first();
    worksAndSparePartsSumm = query.value(0).toFloat();
    query.exec(QUERY_SEL_REPAIR_PARTS(parentTab->repair_id));
    query.first();
    worksAndSparePartsSumm += query.value(0).toFloat();
    if(worksAndSparePartsSumm != parentTab->total_sum)
        ui->pushButtonGetout->setDisabled(true);

    if (!ui->pushButtonGetout->isEnabled())
    {
        shortlivedNotification *newPopup = new shortlivedNotification(this, "Ой-ай", "Ошибка данных. Попробуйте закрыть и снова открыть карту ремонта.", QColor("#FFC7AD"), QColor("#FFA477"));
        newPopup->setVisible(true);
    }

#ifdef QT_DEBUG
//    ui->checkBoxConfirmGetOut->setChecked(true);
//    ui->checkBoxPaymentCheckout->setChecked(true);
//    getOut();
#endif
}

getOutDialog::~getOutDialog()
{
    delete ui;
    emit close();
}

void getOutDialog::checkSums()
{

}

void getOutDialog::setDefaultStyleSheets()
{
    ui->comboBoxRejectReason->setStyleSheet(commonComboBoxStyleSheet);
    ui->comboBoxPaymentAccount->setStyleSheet(commonComboBoxStyleSheet);
    ui->textEditRejectReason->setStyleSheet(commonTextEditStyleSheet);
}

void getOutDialog::getOut()
{
    setDefaultStyleSheets();
    if(ui->comboBoxRejectReason->isVisible() && ui->comboBoxRejectReason->currentIndex() == -1)
    {
        ui->comboBoxRejectReason->setStyleSheet(commonComboBoxStyleSheetRed);
        return;
    }
    if(ui->textEditRejectReason->isVisible() && ui->textEditRejectReason->toPlainText() == "")
    {
        ui->textEditRejectReason->setStyleSheet(commonTextEditStyleSheetRed);
        return;
    }

    // TODO: проверка безналичной оплаты

    tabRepair *parentTab = static_cast<tabRepair*>(parent);
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connThird"));
    QSqlQuery *query2 = new QSqlQuery(QSqlDatabase::database("connThird"));
    bool nDBErr = 1, nIntegrityErr = 1;
    int repair_id, user, client, office, wrkshp_issue_id, cash_order_id;
    float currentPaymentAmount;
    QString paymentSystem;

    repair_id = parentTab->repair_id;
    client = parentTab->repairModel->record(0).value("client").toInt();
    user = userData->value("id").toInt();
    office = userData->value("current_office").toInt();
    currentPaymentAmount = sysLocale.toFloat(ui->lineEditCurrentPaymentAmount->text());
    if( ui->checkBoxPaymentCheckout->isVisible() && !ui->checkBoxPaymentCheckout->isChecked())
    {
        shortlivedNotification *newPopup = new shortlivedNotification(this, "Ой-ай", "Подтвердите правильность ввода данных", QColor("#FFC7AD"), QColor("#FFA477"));
        newPopup->setVisible(true);
        return;
    }
    if( ui->checkBoxConfirmGetOut->isVisible() && !ui->checkBoxConfirmGetOut->isChecked())
    {
        // TODO: подумать над заменой уведомления на окраску чекбокса в красный
        shortlivedNotification *newPopup = new shortlivedNotification(this, "Ой-ай", "Подтвердите выдачу", QColor("#FFC7AD"), QColor("#FFA477"));
        newPopup->setVisible(true);
        return;
    }
    if(summsNotEq)
    {
        QMessageBox::StandardButton resBtn = QMessageBox::question( this, "SNAP CRM",
                                                                    tr("Сумма ремонта меньше согласованной. Всё ОК?\n"),
                                                                    QMessageBox::No | QMessageBox::Yes,
                                                                    QMessageBox::No);
        if (resBtn == QMessageBox::No)
        {
            return;
        }
    }
    if(ui->checkBoxCreditPayment->isChecked())
    {
        if(balanceNotEnough)
        {
            QMessageBox::StandardButton resBtn = QMessageBox::question( this, "SNAP CRM",
                                                                        tr("Баланс клиента меньше стоимости ремонта, продолжить?\n"),
                                                                        QMessageBox::No | QMessageBox::Yes,
                                                                        QMessageBox::No);
            if (resBtn == QMessageBox::No)
            {
                return;
            }
        }
    }

    QUERY_EXEC(query,nDBErr)(QUERY_BEGIN);
    QUERY_EXEC(query,nDBErr)(QUERY_INS_WRKSHP_ISSUED(repair_id, user));
    QUERY_LAST_INS_ID(query,nDBErr,wrkshp_issue_id);
    if(parentTab->repairModel->record(0).value("box").toInt())
        QUERY_EXEC(query,nDBErr)(QUERY_INS_LOG("NULL",3,user,office,"NULL",repair_id,"NULL","NULL","NULL",QString("Заказаз-наряд %1 изъят из ячейки %2").arg(repair_id).arg(parentTab->box_name)));
    QUERY_EXEC(query,nDBErr)(QUERY_INS_REPAIR_STATUS_LOG(repair_id,state,user,parentTab->repairModel->record(0).value("manager").toInt(),parentTab->repairModel->record(0).value("master").toInt()));
    if(currentPaymentAmount)
    {
        if(ui->checkBoxCreditPayment->isChecked())   // если выдача в долг
        {
            QUERY_EXEC(query,nDBErr)(QUERY_INS_BALANCE_LOG(client, -currentPaymentAmount, 0, QString("Списание %1 за ремонт №%2").arg(sysLocale.toCurrencyString(currentPaymentAmount)).arg(repair_id), office, user));
            QUERY_EXEC(query,nDBErr)(QUERY_UPDATE_BALANCE(client,-currentPaymentAmount));
            QUERY_EXEC(query,nDBErr)(QUERY_VRFY_BALANCE(client));
            QUERY_EXEC_VRFY(query,nIntegrityErr);
            QUERY_EXEC(query,nDBErr)(QUERY_INS_LOG("NULL",3,user,office,client,repair_id,"NULL","NULL","NULL",QString("С баланса клиента списано %1 за ремонт №%2.").arg(sysLocale.toCurrencyString(currentPaymentAmount)).arg(repair_id)));
        }
        else    // если выдача с оплатой
        {
            QUERY_EXEC(query,nDBErr)(QUERY_INS_CASH(
                                       15,
                                       currentPaymentAmount,
                                       amountToWords(currentPaymentAmount).replace('\'',"\\\'"),
                                       client,
                                       user,
                                       office,
                                       QString("Поступление денег в размере %2 в счёт выполненого ремонта №%1").arg(repair_id).arg(sysLocale.toCurrencyString(currentPaymentAmount)),
                                       repair_id,
                                       "NULL",
                                       paymentSystemsModel->record(ui->comboBoxPaymentAccount->currentIndex()).value("system_id").toInt()
                                       )
                        );
            // финансы — очень важная вещь, поэтому нужно верифицировать записанные данные
            QUERY_EXEC(query,nDBErr)(QUERY_VRFY_CASH(
                                       15,
                                       currentPaymentAmount,
                                       amountToWords(currentPaymentAmount).replace('\'',"\\\'"),
                                       client,
                                       user,
                                       office,
                                       QString("Поступление денег в размере %2 в счёт выполненого ремонта №%1").arg(repair_id).arg(sysLocale.toCurrencyString(currentPaymentAmount)),
                                       repair_id,
                                       "\'NULL\'",
                                       paymentSystemsModel->record(ui->comboBoxPaymentAccount->currentIndex()).value("system_id").toInt()
                                       )
                        );
            QUERY_EXEC_VRFY(query,nIntegrityErr);
            QUERY_LAST_INS_ID(query,nDBErr,cash_order_id);

            // TODO: Признак предмета расчета
            QUERY_EXEC(query,nDBErr)(QUERY_INS_LOG("NULL",3,user,office,"NULL",repair_id,"NULL","NULL","NULL",QString("Поступление %1 за ремонт №%2 (%3).").arg(sysLocale.toCurrencyString(currentPaymentAmount)).arg(repair_id).arg(ui->comboBoxPaymentAccount->currentText())));
        }
    }
    if (!nIntegrityErr)
    {
        QMessageBox msgBox;
        msgBox.setText("Ошибка");
        msgBox.setIcon(QMessageBox::Critical);
//            qDebug() << "Ошибка целостности данных (касса/баланс)!";
        msgBox.setText(QString("Ошибка целостности данных!"));
        msgBox.exec();
        nDBErr = 0;
    }
//        else
//            qDebug() << "Верификация данных: ОК";

    QUERY_EXEC(query,nDBErr)(QUERY_INS_LOG("NULL",3,user,office,"NULL",repair_id,"NULL","NULL","NULL",QString("Заказаз-наряд №%1 выдан клиенту.").arg(repair_id)));

    QUERY_EXEC(query,nDBErr)(QUERY_UPDATE_STORE_INT_RSRV(3,repair_id));

    QUERY_EXEC(query2,nDBErr)(QUERY_SEL_PRE_UPD_STORE_ITEMS(repair_id));    // кол-ва товара, которое должно получиться после обновления
    QUERY_EXEC(query,nDBErr)(QUERY_UPD_STORE_ITEMS(repair_id));
    QUERY_EXEC(query,nDBErr)(QUERY_SEL_PST_UPD_STORE_ITEMS(repair_id));     // кол-ва товара после обновления
    if (query->size() == query2->size())    // проверка одинакового кол-ва строк (чтобы не вылетело приложение)
    {
        while(query->next() && query2->next() && nIntegrityErr)
        {
            for(int i = query->record().count() - 1; i >= 0; i--)   // проверка одинаковости всех полей
            {
//                qDebug() << QString("query->value(%1) =").arg(i) << query->value(i).toInt() << QString("query2->value(%1) =").arg(i) << query2->value(i).toInt();
                if (query->value(i).toInt() != query2->value(i).toInt())
                {
                    nIntegrityErr = 0;
                }
            }
        }
    }
    else
        nIntegrityErr = 0;

    if (!nIntegrityErr)
    {
        QMessageBox msgBox;
        msgBox.setText("Ошибка");
        msgBox.setIcon(QMessageBox::Critical);
//            qDebug() << "Ошибка целостности данных (товары)!";
        msgBox.setText(QString("Ошибка целостности данных!"));
        msgBox.exec();
        nDBErr = 0;
    }
//        else
//            qDebug() << "Верификация данных: ОК";
    // QUERY_INS_LOG(G,T,U,O,C,R,I,D,B,t)
    QUERY_EXEC(query,nDBErr)(QUERY_INS_LOG_PARTS_IN_REPAIR(user,office,client,repair_id));
    if(ui->comboBoxRejectReason->isVisible())
    {
        if(ui->comboBoxRejectReason->currentIndex() != OTHER_REJECT_REASON_ID)
        {
            QUERY_EXEC(query,nDBErr)(QUERY_SET_REJ_REASON(repair_id, ui->comboBoxRejectReason->currentText()));
        }
        else
        {
            QUERY_EXEC(query,nDBErr)(QUERY_SET_REJ_REASON(repair_id, ui->textEditRejectReason->toPlainText()));
        }
    }

    QUERY_EXEC(query,nDBErr)(QUERY_UPD_REPAIR_STATE(repair_id, newState));

//    nDBErr = 0; // это временное для отладки (чтобы сессия всегда завершалась ROLLBACKOM)
    QUERY_COMMIT_ROLLBACK(query,nDBErr);

    if(nDBErr)
    {
        emit getOutOk();
        this->deleteLater();
    }
    delete query;
    delete query2;
}

void getOutDialog::cancel()
{
    emit close();
//    this->deleteLater();
}

void getOutDialog::createLooseDoc()
{

}

void getOutDialog::enableComboBoxPaymentAccount(int state)
{
    ui->comboBoxPaymentAccount->setEnabled(!state);
}

void getOutDialog::otheRejectReasonShow(QString comboBoxText)
{
    ui->comboBoxRejectReason->setStyleSheet(commonComboBoxStyleSheet);
    if(comboBoxText == rejectReasonModel->getDisplayRole(OTHER_REJECT_REASON_ID, 1))
    {
        ui->textEditRejectReason->setHidden(false);
        ui->textEditRejectReason->resize(ui->comboBoxRejectReason->width(), 100);
        ui->textEditRejectReason->setStyleSheet(commonTextEditStyleSheet);
    }
    else
    {
        ui->textEditRejectReason->setHidden(true);
    }
}

void getOutDialog::textEditTextChanged()
{
    ui->textEditRejectReason->setStyleSheet(commonTextEditStyleSheet);
}
