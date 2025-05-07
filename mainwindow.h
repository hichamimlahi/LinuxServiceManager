#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QComboBox>
#include <QMessageBox>
#include <QStatusBar>
#include <QToolBar>
#include <QAction>
#include <QTimer>

#include "servicemanager.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void refreshServices();

    void serviceSelected(QTableWidgetItem *current, QTableWidgetItem *previous);

    void startSelectedService();

    void stopSelectedService();

    void restartSelectedService();

    void viewServiceLogs();

    void onServiceOperationCompleted(const QString &serviceName, const QString &operation, bool success);

    void autoRefreshServices();

private:
    QTableWidget *m_servicesTable;
    QPushButton *m_startButton;
    QPushButton *m_stopButton;
    QPushButton *m_restartButton;
    QPushButton *m_viewLogsButton;
    QPushButton *m_refreshButton;
    QTextEdit *m_logViewer;
    QLabel *m_statusLabel;
    QComboBox *m_autoRefreshCombo;
    QTimer *m_refreshTimer;

    ServiceManager *m_serviceManager;
    QString m_selectedService;

    void createUI();

    void initializeServicesTable();

    void updateServicesTable();

    void setupConnections();

    void updateUIState();

    void setupAutoRefresh();
};

#endif
