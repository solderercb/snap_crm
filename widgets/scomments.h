#ifndef SCOMMENTS_H
#define SCOMMENTS_H

#include <QScrollArea>
#include "tabcommon.h"
#include "widgets/swidget.h"
#include "widgets/splaintextedit.h"
#include "../models/scommentmodel.h"
#include "../models/scommentsmodel.h"
#ifdef QT_DEBUG
#include <QTest>
#endif

namespace Ui {
class SComments;
}

class commentsTable : public QTableView
{
    Q_OBJECT
signals:
    void copyText();
public:
    explicit commentsTable(QWidget *parent = nullptr);
    ~commentsTable();
    void resizeEvent(QResizeEvent*) override;
    void keyPressEvent(QKeyEvent *event) override;
    QModelIndexList selectedIndexes() const override;
private:
    QFontMetrics *fontMetrics;
};

class SComments : public SWidget
{
    Q_OBJECT

public:
    enum MenuActions{Copy = 0, Edit, Remove};
    explicit SComments(QWidget *parent = nullptr);
    ~SComments();
    void setParentTab(tabCommon*);
    void setMode(const int);
    void load(const int);
    private:
    Ui::SComments *ui;
    SCommentsModel *commentsModel;
    QSqlQuery *m_query;
    tabCommon *m_parentTab = nullptr;
    bool isEditable(const int row);
    tabCommon *findParentTab();
    void checkParentTab();
private:
    QMenu *tableMenu = nullptr;
    void initTableMenu();
    void appendLineTermination();
    void clearInputField();
    void commit();
    void editLastComment();
private slots:
    void menuRequest(QPoint pos);
    void remove();
    void edit();
    void copyToClipboard(const bool copyTimeStamp = 0);
    void lineEditKeyPressEvent(int);
    void updateTable();
};

#endif // SCOMMENTS_H
