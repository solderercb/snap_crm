#ifndef SFIELDMODEL_H
#define SFIELDMODEL_H

#include <SSingleRowJModel>
#include <QSqlDatabase>

class QLineEdit;
class QComboBox;
class SDateEdit;

#include "ssinglerowmodel_predef.h"     // этот файл нужно подключать после ssinglerowmodel.h и до списка элементов
#define TABLE_FIELDS                                                        \
    TABLE_FIELD(id, id, int, 0)                                             \
    TABLE_FIELD(_f, target, bool, 0)                                        \
    TABLE_FIELD(name, name, QString, 0)                                     \
    TABLE_FIELD(type, type, int, 0)                                         \
    TABLE_FIELD(def_values, defaultValue, QString, 0)                       \
    TABLE_FIELD(required, required, bool, 0)                                \
    TABLE_FIELD(printable, printable, bool, 0)                              \
    TABLE_FIELD(updated_at, updated, QDateTime, 0)                          \
    TABLE_FIELD(devices, devices, QString, 0)                               \
    TABLE_FIELD(categories, categories, QString, 0)                         \
    TABLE_FIELD(archive, archive, bool, 0)                                  \
    TABLE_FIELD(dev_match, deviceMatch, bool, 0)                            \
	TABLE_FIELD(input_mask, inputMask, QString, 0)

class SFieldModel : public SSingleRowJModel
{
    Q_OBJECT
    friend class TClassTest;
    friend class SFieldsModel;
    friend class SFieldValueModel;
signals:
    void textChanged(int, QString);
public:
    enum WidgetType {LineEdit = 1, ComboBox, DateEdit, dummy};
    explicit SFieldModel(QObject *parent = nullptr);
    explicit SFieldModel(const int id, QSqlDatabase database = QSqlDatabase::database(TdConn::main()), QObject *parent = nullptr);
    ~SFieldModel();
#include "ssinglerowmodel_init.h"     // этот файл нужно подключать именно здесь
public:
    QWidget *widget();
    void setDefaultStyleSheet();
    QWidget *createWidget(const QString &value = QString());
    void deleteWidget();
    void setWidgetValue(const QString &value);
    void setWidgetDefaultValue();
    bool validate();
private:
    QWidget *m_widget = nullptr;
    QWidget *createLineEdit();
    QWidget *createComboBox();
    QWidget *createDateTime();
    QWidget *createDummyWidget();
    QComboBox *comboBox();
    QLineEdit *lineEdit();
    SDateEdit *dateEdit();
    QDate stringToDate(const QString &str);
    QString fieldsForSelectQuery() override;
};

#endif // SFIELDMODEL_H
