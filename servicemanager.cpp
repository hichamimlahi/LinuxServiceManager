#include "servicemanager.h"
#include <QDebug>
#include <QRegularExpression>

ServiceManager::ServiceManager(QObject *parent) : QObject(parent)
{
}

ServiceManager::~ServiceManager()
{
}

bool ServiceManager::refreshServiceList()
{
    QString output;
    int exitCode = executeSystemctl({"list-units", "--type=service", "--all", "--no-pager", "--no-legend"}, &output);
    
    if (exitCode != 0) {
        m_lastError = "Failed to execute systemctl command. Exit code: " + QString::number(exitCode);
        return false;
    }
    
    parseServiceList(output);
    emit servicesUpdated();
    return true;
}

QList<ServiceManager::ServiceInfo> ServiceManager::getServices() const
{
    return m_services;
}

bool ServiceManager::startService(const QString &serviceName)
{
    QString output;
    int exitCode = executeSystemctl({"start", serviceName}, &output);
    
    bool success = (exitCode == 0);
    if (!success) {
        m_lastError = "Failed to start service " + serviceName + ". " + output;
    }
    
    emit serviceOperationCompleted(serviceName, "start", success);
    return success;
}

bool ServiceManager::stopService(const QString &serviceName)
{
    QString output;
    int exitCode = executeSystemctl({"stop", serviceName}, &output);
    
    bool success = (exitCode == 0);
    if (!success) {
        m_lastError = "Failed to stop service " + serviceName + ". " + output;
    }
    
    emit serviceOperationCompleted(serviceName, "stop", success);
    return success;
}

bool ServiceManager::restartService(const QString &serviceName)
{
    QString output;
    int exitCode = executeSystemctl({"restart", serviceName}, &output);
    
    bool success = (exitCode == 0);
    if (!success) {
        m_lastError = "Failed to restart service " + serviceName + ". " + output;
    }
    
    emit serviceOperationCompleted(serviceName, "restart", success);
    return success;
}

QString ServiceManager::getServiceLogs(const QString &serviceName, int lines)
{
    QString output;
    QStringList args = {"journalctl", "-u", serviceName, "-n", QString::number(lines), "--no-pager"};
    
    QProcess process;
    process.start("sudo", args);
    
    if (!process.waitForStarted(5000)) {
        m_lastError = "Failed to start journalctl process: " + process.errorString();
        return "Error: " + m_lastError;
    }
    
    if (!process.waitForFinished(10000)) {
        m_lastError = "journalctl process timed out: " + process.errorString();
        return "Error: " + m_lastError;
    }
    
    if (process.exitCode() != 0) {
        m_lastError = "journalctl process failed with exit code " + QString::number(process.exitCode());
        return "Error: " + m_lastError + "\n" + process.readAllStandardError();
    }
    
    return process.readAllStandardOutput();
}

QString ServiceManager::lastError() const
{
    return m_lastError;
}

int ServiceManager::executeSystemctl(const QStringList &arguments, QString *output)
{
    QProcess process;
    QStringList args = {"systemctl"};
    args.append(arguments);
    
    process.start("sudo", args);
    
    if (!process.waitForStarted(5000)) {
        m_lastError = "Failed to start systemctl process: " + process.errorString();
        return -1;
    }
    
    if (!process.waitForFinished(10000)) {
        m_lastError = "systemctl process timed out: " + process.errorString();
        return -1;
    }
    
    if (output) {
        *output = process.readAllStandardOutput();
    }
    
    return process.exitCode();
}

void ServiceManager::parseServiceList(const QString &output)
{
    m_services.clear();
    
    QStringList lines = output.split('\n', Qt::SkipEmptyParts);
    
    for (const QString &line : lines) {
        QStringList parts = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        
        if (parts.size() >= 4) {
            ServiceInfo service;
            service.name = parts[0];
            service.loadState = parts[1];
            service.activeState = parts[2];
            service.subState = parts[3];
            
            if (parts.size() > 4) {
                service.description = parts.mid(4).join(" ");
            }
            
            m_services.append(service);
        }
    }
}