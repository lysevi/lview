#ifndef LOGLINE_H
#define LOGLINE_H

#include <QObject>
#include <QDateTime>
#include <QString>
#include <QVariant>

class LogLine : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QDateTime timestamp READ timestamp NOTIFY timestampChanged)
    Q_PROPERTY(QVariant message READ message NOTIFY messageChanged)
    Q_PROPERTY(int count READ count NOTIFY countChanged)
public:
    explicit LogLine(QObject *parent = nullptr);
    LogLine(const QDateTime &timestamp, const QStringList &message, QObject *parent=0);

    QDateTime timestamp() const;
    void setTimestamp(const QDateTime &ts);

    QVariant message() const;

    int count()const;
signals:
    void timestampChanged();
    void messageChanged();
     void countChanged();
private:
    QDateTime m_time;
    QStringList m_message;
    QVariant m_variant_message;
};

#endif // LOGLINE_H
