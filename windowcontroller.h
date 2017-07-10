#ifndef WINDOWCONTROLLER_H
#define WINDOWCONTROLLER_H

#include <QObject>
#include <QDebug>
#include <QList>

#include "log.h"

class WindowController : public QObject
{
    Q_OBJECT
public:
    WindowController(QObject* rootObject, QObject*parent=nullptr);

    void addLogs(const QList<Log*>&logs);
protected:
    QList<Log*> m_logs;
    QObject* m_rootObject;
private:
    void addTab(const QString&title, Log*v);
signals:

public slots:
    void updateAllSlot(const QString &msg);
    void openFileSlot(const QString &fname);
};

#endif // WINDOWCONTROLLER_H
