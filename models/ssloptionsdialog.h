#ifndef SSLOPTIONSDIALOG_H
#define SSLOPTIONSDIALOG_H

#include <QWidget>
#include <QFrame>
#include <QWidgetAction>
#include <QStandardItemModel>
#include <QSignalMapper>
#include <QFileDialog>

namespace Ui {
    class SSLOptionsDialog;
}

class SSLOptionsDialog : public QFrame
{
    Q_OBJECT
signals:
    void close();
public:
    explicit SSLOptionsDialog(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~SSLOptionsDialog();
    Ui::SSLOptionsDialog *ui;

private:
    QSignalMapper *signalMapper;
    QWidget *parent;
    QString directory;

private slots:
    void browse(int);
    void cancel();
    void ok();
};

#endif // SSLOPTIONSDIALOG_H
