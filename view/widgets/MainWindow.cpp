#include "MainWindow.hpp"

#include <QCloseEvent>





MainWindow::MainWindow(ProjectManager* projectManager,
                       QAction* quit) :
    QMainWindow(),
    m_editorWidget(new EditorWidget),
    m_projectDock(new ProjectFileDock(projectManager)),
    m_openedFileDock(new OpenedFileDock),
    m_quitAction(quit),
    m_projectManager(projectManager)
{
    setWindowTitle(tr("Orkestra v0.1"));
    setMinimumSize(800, 400);
    
    
    
    // Setup docks.
    addDockWidget(Qt::LeftDockWidgetArea, m_projectDock);
    addDockWidget(Qt::LeftDockWidgetArea, m_openedFileDock);
    
    connect(m_projectDock, &ProjectFileDock::projectChanged, this, &MainWindow::setupCurrentProject);
    connect(m_openedFileDock, &OpenedFileDock::fileChanged, m_editorWidget, &EditorWidget::setCurrentFile);
    
    
    
    // Setup central widget.
    setCentralWidget(m_editorWidget);
}





void MainWindow::setupCurrentProject(const QModelIndex& projectIndex)
{
    Project* project(m_projectManager->getProject(projectIndex));
    m_openedFileDock->setCurrentProject(project);
    m_editorWidget->setCurrentProject(project);
}





void MainWindow::closeEvent(QCloseEvent* event)
{
    /* NOTE: We intercept the close event, and trigger the quit action instead. This way, we pass through the controller
     * to quit the application.
     */
    event->ignore();
    m_quitAction->triggered();
}
