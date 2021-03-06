#ifndef LOGVIEWER_H
#define LOGVIEWER_H

#include "listboxeditableitem.h"
#include <QProgressDialog>
#include <QWidget>
#include <memory>

namespace Ui {
class LogViewer;
}

class Log;

class LogViewer : public QWidget {
  Q_OBJECT

public:
  explicit LogViewer(const QFont &font, QWidget *parent = 0);
  ~LogViewer();
  void setModel(Log *model_);
  Log *model() const { return m_model; }
  void setAutoScroll(bool value);
  bool eventFilter(QObject *object, QEvent *event) override;
  void selectRow(int row);
  int selectedRow();

signals:
  void selectNewRow();
private slots:
  void onScrollRangeChanged(int min, int max);
  void customContextMenu(const QPoint &);
  void copySelectedSlot();
  void onSelectionChangedSlot();

private:
  Ui::LogViewer *ui;
  Log *m_model;
  ListboxEditableItem m_delegate;
  bool m_autoscroll;
  QFont m_default_font;
  std::shared_ptr<QProgressDialog> m_progress_dlg;
};

#endif // LOGVIEWER_H
