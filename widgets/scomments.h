#ifndef SCOMMENTS_H
#define SCOMMENTS_H

#include <QTableView>
#include <SWidget>

class SCommentsModel;
class QSqlQuery;
class tabCommon;
class QMenu;

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
    friend class TClassTest;
public:
    enum MenuActions{Copy = 0, Edit, Remove};
    explicit SComments(QWidget *parent = nullptr);
    ~SComments();
    void setParentTab(tabCommon*);
    void setMode(const int);
    void load(const int);
private:
    enum OpType {Commit, Delete};
    Ui::SComments *ui;
    SCommentsModel *commentsModel;
    QSqlQuery *m_query;
    tabCommon *m_parentTab = nullptr;
    QMenu *tableMenu = nullptr;
    int m_opType = Commit;
    bool isEditable(const int row);
    tabCommon *findParentTab();
    void checkParentTab();
    void initTableMenu();
    void appendLineTermination();
    void clearInputField();
    void editLastComment();
    int checkInput() override;
    QString queryLogFile() override;
    void commit(const int stage = 0) override;
    void addComment();
    void updateComment();
    void deleteComment();
    void endCommit() override;
private slots:
    void menuRequest(QPoint pos);
    void remove();
    void edit();
    void copyToClipboard(const bool copyTimeStamp = 0);
    void lineEditKeyPressEvent(int);
    void updateTable();
};

#endif // SCOMMENTS_H
