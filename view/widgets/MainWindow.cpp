#include "MainWindow.hpp"

#include <QCloseEvent>





MainWindow::MainWindow(QAction* quit,
                       EditorWidget* editorWidget,
                       ProjectFileDock* projectFileDock,
                       OpenedFileDock* openedFileDock) :
    QMainWindow(),
    m_quitAction(quit)
{
    setWindowTitle(tr("Orkestra v0.1"));
    setMinimumSize(800, 400);
    
    
    
    // Setup docks.
    addDockWidget(Qt::LeftDockWidgetArea, projectFileDock);
    addDockWidget(Qt::LeftDockWidgetArea, openedFileDock);
    
    
    
    // Setup central widget.
    setCentralWidget(editorWidget);
}





void MainWindow::closeEvent(QCloseEvent* event)
{
    /* NOTE: We intercept the close event, and trigger the quit action instead. This way, we pass through the controller
     * to quit the application.
     */
    event->ignore();
    m_quitAction->triggered();
}
