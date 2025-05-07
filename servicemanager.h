#ifndef SERVICEMANAGER_H
#define SERVICEMANAGER_H

#include <QObject>
#include <QProcess>
#include <QStringList>
#include <QMap>


class ServiceManager : public QObject
{
    Q_OBJECT

public:
    explicit ServiceManager(QObject *parent = nullptr);
    ~ServiceManager();

    
    struct ServiceInfo {
        QString name;
        QString loadState;
        QString activeState;
        QString subState;
        QString description;
    };

    
    bool refreshServiceList();

    
    QList<ServiceInfo> getServices() const;

    
    bool startService(const QString &serviceName);

    
    bool stopService(const QString &serviceName);

    
    bool restartService(const QString &serviceName);

    
    QString getServiceLogs(const QString &serviceName, int lines = 50);

    
    QString lastError() const;

signals:
    
    void servicesUpdated();

    
    void serviceOperationCompleted(const QString &serviceName, const QString &operation, bool success);

private:
    QList<ServiceInfo> m_services;
    QString m_lastError;

    
    int executeSystemctl(const QStringList &arguments, QString *output = nullptr);

    
    void parseServiceList(const QString &output);
};

#endif 
