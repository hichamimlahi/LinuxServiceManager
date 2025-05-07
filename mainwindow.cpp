#include "mainwindow.h"
#include <QHeaderView>
#include <QMenu>
#include <QMenuBar>
#include <QInputDialog>
#include <QApplication>
#include <QScreen>
#include <QRect>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_serviceManager(new ServiceManager(this))
    , m_refreshTimer(new QTimer(this))
{
    setWindowTitle("Linux Service Manager");
    resize(900, 600);
    
    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
    int x = (screenGeometry.width() - width()) / 2;
    int y = (screenGeometry.height() - height()) / 2;
    move(x, y);
    
    createUI();
    setupConnections();
    setupAutoRefresh();
    
    refreshServices();
}

MainWindow::~MainWindow()
{
}

void MainWindow::createUI()
{
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    
    QSplitter *splitter = new QSplitter(Qt::Vertical);
    
    QWidget *topWidget = new QWidget();
    QVBoxLayout *topLayout = new QVBoxLayout(topWidget);
    
    QHBoxLayout *autoRefreshLayout = new QHBoxLayout();
    QLabel *autoRefreshLabel = new QLabel("Auto Refresh:");
    m_autoRefreshCombo = new QComboBox();
    m_autoRefreshCombo->addItem("Off", 0);
    m_autoRefreshCombo->addItem("5 seconds", 5000);
    m_autoRefreshCombo->addItem("10 seconds", 10000);
    m_autoRefreshCombo->addItem("30 seconds", 30000);
    m_autoRefreshCombo->addItem("1 minute", 60000);
    
    autoRefreshLayout->addWidget(autoRefreshLabel);
    autoRefreshLayout->addWidget(m_autoRefreshCombo);
    autoRefreshLayout->addStretch();
    
    m_refreshButton = new QPushButton("Refresh");
    m_refreshButton->setIcon(QIcon::fromTheme("view-refresh"));
    autoRefreshLayout->addWidget(m_refreshButton);
    
    topLayout->addLayout(autoRefreshLayout);
    
    m_servicesTable = new QTableWidget();
    initializeServicesTable();
    topLayout->addWidget(m_servicesTable);
    
    QHBoxLayout *controlLayout = new QHBoxLayout();
    
    m_startButton = new QPushButton("Start");
    m_startButton->setIcon(QIcon::fromTheme("media-playback-start"));
    m_startButton->setEnabled(false);
    
    m_stopButton = new QPushButton("Stop");
    m_stopButton->setIcon(QIcon::fromTheme("media-playback-stop"));
    m_stopButton->setEnabled(false);
    
    m_restartButton = new QPushButton("Restart");
    m_restartButton->setIcon(QIcon::fromTheme("media-playlist-repeat"));
    m_restartButton->setEnabled(false);
    
    m_viewLogsButton = new QPushButton("View Logs");
    m_viewLogsButton->setIcon(QIcon::fromTheme("text-x-log"));
    m_viewLogsButton->setEnabled(false);
    
    controlLayout->addWidget(m_startButton);
    controlLayout->addWidget(m_stopButton);
    controlLayout->addWidget(m_restartButton);
    controlLayout->addWidget(m_viewLogsButton);
    
    topLayout->addLayout(controlLayout);
    
    QWidget *bottomWidget = new QWidget();
    QVBoxLayout *bottomLayout = new QVBoxLayout(bottomWidget);
    
    QLabel *logLabel = new QLabel("Service Logs:");
    bottomLayout->addWidget(logLabel);
    
    m_logViewer = new QTextEdit();
    m_logViewer->setReadOnly(true);
    m_logViewer->setFont(QFont("Monospace"));
    bottomLayout->addWidget(m_logViewer);
    
    splitter->addWidget(topWidget);
    splitter->addWidget(bottomWidget);
    splitter->setStretchFactor(0, 2);
    splitter->setStretchFactor(1, 1);
    
    mainLayout->addWidget(splitter);
    
    setCentralWidget(centralWidget);
    
    m_statusLabel = new QLabel("Ready");
    statusBar()->addWidget(m_statusLabel);
    
    QMenu *fileMenu = menuBar()->addMenu("&File");
    QAction *exitAction = fileMenu->addAction("E&xit");
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
    
    QMenu *serviceMenu = menuBar()->addMenu("&Service");
    QAction *refreshAction = serviceMenu->addAction("&Refresh");
    refreshAction->setShortcut(QKeySequence::Refresh);
    connect(refreshAction, &QAction::triggered, this, &MainWindow::refreshServices);
    
    QMenu *helpMenu = menuBar()->addMenu("&Help");
    QAction *aboutAction = helpMenu->addAction("&About");
    connect(aboutAction, &QAction::triggered, [this]() {
        QMessageBox::about(this, "About Linux Service Manager", 
                          "Linux Service Manager\n\n"
                          "A Qt application for managing Linux services using systemctl.\n"
                          "Developed by Group 5\n\n"
                          "Features:\n"
                          "- List active services\n"
                          "- Start, stop, restart services\n"
                          "- View service logs");
    });
}

void MainWindow::initializeServicesTable()
{
    m_servicesTable->setColumnCount(5);
    QStringList headers;
    headers << "Service" << "Load" << "Active" << "Sub" << "Description";
    m_servicesTable->setHorizontalHeaderLabels(headers);
    
    m_servicesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_servicesTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_servicesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_servicesTable->setAlternatingRowColors(true);
    m_servicesTable->setSortingEnabled(true);
    
    m_servicesTable->horizontalHeader()->setStretchLastSection(true);
    m_servicesTable->verticalHeader()->setVisible(false);
}

void MainWindow::updateServicesTable()
{
    m_servicesTable->setSortingEnabled(false);
    m_servicesTable->clearContents();
    
    QList<ServiceManager::ServiceInfo> services = m_serviceManager->getServices();
    m_servicesTable->setRowCount(services.size());
    
    for (int i = 0; i < services.size(); ++i) {
        const auto &service = services[i];
        
        QTableWidgetItem *nameItem = new QTableWidgetItem(service.name);
        QTableWidgetItem *loadItem = new QTableWidgetItem(service.loadState);
        QTableWidgetItem *activeItem = new QTableWidgetItem(service.activeState);
        QTableWidgetItem *subItem = new QTableWidgetItem(service.subState);
        QTableWidgetItem *descItem = new QTableWidgetItem(service.description);
        
        if (service.activeState == "active") {
            activeItem->setBackground(Qt::green);
        } else if (service.activeState == "failed") {
            activeItem->setBackground(Qt::red);
        } else {
            activeItem->setBackground(Qt::yellow);
        }
        
        m_servicesTable->setItem(i, 0, nameItem);
        m_servicesTable->setItem(i, 1, loadItem);
        m_servicesTable->setItem(i, 2, activeItem);
        m_servicesTable->setItem(i, 3, subItem);
        m_servicesTable->setItem(i, 4, descItem);
    }
    
    m_servicesTable->setSortingEnabled(true);
    m_servicesTable->sortByColumn(0, Qt::AscendingOrder);
    
    m_servicesTable->resizeColumnsToContents();
    m_servicesTable->horizontalHeader()->setStretchLastSection(true);
    
    m_statusLabel->setText("Services refreshed: " + QString::number(services.size()) + " services found");
}

void MainWindow::setupConnections()
{
    
    connect(m_serviceManager, &ServiceManager::servicesUpdated, this, &MainWindow::updateServicesTable);
    connect(m_serviceManager, &ServiceManager::serviceOperationCompleted, 
            this, &MainWindow::onServiceOperationCompleted);
    
    
    connect(m_refreshButton, &QPushButton::clicked, this, &MainWindow::refreshServices);
    connect(m_startButton, &QPushButton::clicked, this, &MainWindow::startSelectedService);
    connect(m_stopButton, &QPushButton::clicked, this, &MainWindow::stopSelectedService);
    connect(m_restartButton, &QPushButton::clicked, this, &MainWindow::restartSelectedService);
    connect(m_viewLogsButton, &QPushButton::clicked, this, &MainWindow::viewServiceLogs);
    
    connect(m_servicesTable, &QTableWidget::itemSelectionChanged, [this]() {
        QList<QTableWidgetItem*> selectedItems = m_servicesTable->selectedItems();
        if (!selectedItems.isEmpty()) {
            int row = selectedItems.first()->row();
            m_selectedService = m_servicesTable->item(row, 0)->text();
            updateUIState();
        } else {
            m_selectedService.clear();
            updateUIState();
        }
    });
    
    connect(m_autoRefreshCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int index) {
        int interval = m_autoRefreshCombo->currentData().toInt();
        if (interval > 0) {
            m_refreshTimer->start(interval);
        } else {
            m_refreshTimer->stop();
        }
    });
    
    
    connect(m_refreshTimer, &QTimer::timeout, this, &MainWindow::autoRefreshServices);
}

void MainWindow::setupAutoRefresh()
{
    m_refreshTimer->setInterval(5000); 
    m_autoRefreshCombo->setCurrentIndex(0); 
}

void MainWindow::refreshServices()
{
    m_statusLabel->setText("Refreshing services...");
    if (!m_serviceManager->refreshServiceList()) {
        QMessageBox::warning(this, "Error", 
                            "Failed to refresh services: " + m_serviceManager->lastError());
        m_statusLabel->setText("Error: Failed to refresh services");
    }
}

void MainWindow::serviceSelected(QTableWidgetItem *current, QTableWidgetItem *previous)
{
    Q_UNUSED(previous);
    
    if (current) {
        int row = current->row();
        m_selectedService = m_servicesTable->item(row, 0)->text();
        updateUIState();
    } else {
        m_selectedService.clear();
        updateUIState();
    }
}

void MainWindow::updateUIState()
{
    bool hasSelection = !m_selectedService.isEmpty();
    
    m_startButton->setEnabled(hasSelection);
    m_stopButton->setEnabled(hasSelection);
    m_restartButton->setEnabled(hasSelection);
    m_viewLogsButton->setEnabled(hasSelection);
    
    if (hasSelection) {
        m_statusLabel->setText("Selected service: " + m_selectedService);
    } else {
        m_statusLabel->setText("No service selected");
    }
}

void MainWindow::startSelectedService()
{
    if (m_selectedService.isEmpty()) return;
    
    m_statusLabel->setText("Starting service: " + m_selectedService + "...");
    
    if (!m_serviceManager->startService(m_selectedService)) {
        QMessageBox::warning(this, "Error", 
                           "Failed to start service: " + m_serviceManager->lastError());
    }
}

void MainWindow::stopSelectedService()
{
    if (m_selectedService.isEmpty()) return;
    
    
    QMessageBox::StandardButton confirm = QMessageBox::question(
        this, "Confirm Stop", 
        "Are you sure you want to stop the service " + m_selectedService + "?",
        QMessageBox::Yes | QMessageBox::No
    );
    
    if (confirm != QMessageBox::Yes) return;
    
    m_statusLabel->setText("Stopping service: " + m_selectedService + "...");
    
    if (!m_serviceManager->stopService(m_selectedService)) {
        QMessageBox::warning(this, "Error", 
                           "Failed to stop service: " + m_serviceManager->lastError());
    }
}

void MainWindow::restartSelectedService()
{
    if (m_selectedService.isEmpty()) return;
    
    
    QMessageBox::StandardButton confirm = QMessageBox::question(
        this, "Confirm Restart", 
        "Are you sure you want to restart the service " + m_selectedService + "?",
        QMessageBox::Yes | QMessageBox::No
    );
    
    if (confirm != QMessageBox::Yes) return;
    
    m_statusLabel->setText("Restarting service: " + m_selectedService + "...");
    
    if (!m_serviceManager->restartService(m_selectedService)) {
        QMessageBox::warning(this, "Error", 
                           "Failed to restart service: " + m_serviceManager->lastError());
    }
}

void MainWindow::viewServiceLogs()
{
    if (m_selectedService.isEmpty()) return;
    
    m_statusLabel->setText("Fetching logs for service: " + m_selectedService + "...");
    
    
    bool ok;
    int lines = QInputDialog::getInt(this, "Log Lines", 
                                   "Number of log lines to display:", 
                                   50, 10, 1000, 10, &ok);
    
    if (!ok) return;
    
    QString logs = m_serviceManager->getServiceLogs(m_selectedService, lines);
    
    m_logViewer->setText(logs);
    m_logViewer->moveCursor(QTextCursor::End);
    
    m_statusLabel->setText("Logs displayed for service: " + m_selectedService);
}

void MainWindow::onServiceOperationCompleted(const QString &serviceName, const QString &operation, bool success)
{
    if (success) {
        m_statusLabel->setText(operation + " completed successfully for service: " + serviceName);
        
        
        refreshServices();
    } else {
        m_statusLabel->setText(operation + " failed for service: " + serviceName);
    }
}

void MainWindow::autoRefreshServices()
{
    
    if (m_refreshTimer->isActive()) {
        refreshServices();
    }
}
