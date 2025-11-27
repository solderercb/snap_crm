/*
* https://wiki.qt.io/Show_tooltips_for_long_entries_of_your_custom_model
*/

#ifndef QTOOLTIPPER_H_
#define QTOOLTIPPER_H_

#include <QObject>

class QToolTipper : public QObject
{
  Q_OBJECT
public:
  explicit QToolTipper(QObject* parent = NULL);
protected:
  bool eventFilter(QObject* obj, QEvent* event);
};

#endif
