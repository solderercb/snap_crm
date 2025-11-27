#include <QProcess>
#include <QDebug>

int main() {
    QStringList testApps = {
        "../Fields_test/build_type/fields_test",
        "../SSingleRowModel_test/build_type/SSingleRowModel_test",
        "../SInvoiceModel_test/build_type/sinvoicemodel_test",
        "../SRepairSaleItemModel_test/build_type/SRepairSaleItemModel_test",
        "../SRepairStatusLog_test/build_type/SRepairStatusLog_test",
        "../SStoreItemModel_test/build_type/SStoreItemModel_test",
        "../SStoreSaleItemModel_test/build_type/SStoreSaleItemModel_test",
        "../SWorkModel_test/build_type/sworkmodel_test",
        "../SClientModel_test/build_type/SClientModel_test",
        "../SRepairModel_test/build_type/SRepairModel_test",
        "../SPartRequestModel_test/build_type/SPartRequestModel_test",
        "../SComSettings_test/build_type/SComSettings_test",
        "../SSaleTableModel_test/build_type/SSaleTableModel_test",
        "../SComments_widget_test/build_type/SComments_widget_test"
    };

    for (QString testPath : testApps) {
        qDebug() << "Running:" << testPath;
#ifdef QT_DEBUG
        int result = QProcess::execute(testPath.replace("build_type", "debug"), QStringList());
#else
        int result = QProcess::execute(testPath.replace("build_type", "release"), QStringList());
#endif
        if (result != 0) {
            qDebug() << "Test failed:" << testPath;
        }
    }

    return 0;
}
