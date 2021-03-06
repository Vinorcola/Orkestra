#include "ProjectLoadedState.hpp"

#include <QAction>
#include <QFileDialog>
#include <QStateMachine>





ProjectLoadedState::ProjectLoadedState(ProjectManager* projectManager,
                                       EditorWidget* editorWidget,
                                       ProjectFileDock* projectFileDock,
                                       OpenedFileDock* openedFileDock) :
    QState(),
    m_projectManager(projectManager),
    m_currentProject(0),
    m_closeProject(new QAction(tr("Close project"), this)),
    m_openFile(new QAction(tr("Open"), this)),
    m_closeFile(new QAction(tr("Close"), this)),
    m_saveFile(new QAction(tr("Save"), this)),
    m_saveAllFiles(new QAction(tr("Save all"), this)),
    m_editorWidget(editorWidget),
    m_projectFileDock(projectFileDock),
    m_openedFileDock(openedFileDock),
    m_connectionFileOpened(),
    m_connectionFileOpeningRequested()
{
    // Setup actions.
    m_closeProject->setDisabled(true);
    m_openFile->setShortcut(Qt::CTRL + Qt::Key_O);
    m_openFile->setDisabled(true);
    m_closeFile->setShortcut(Qt::CTRL + Qt::Key_W);
    m_closeFile->setDisabled(true);
    m_saveFile->setShortcut(Qt::CTRL + Qt::Key_S);
    m_saveFile->setDisabled(true);
    m_saveAllFiles->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_S);
    m_saveAllFiles->setDisabled(true);
    
    connect(m_openedFileDock, &OpenedFileDock::fileChanged, m_editorWidget, &EditorWidget::setCurrentFile);
    
    connect(m_closeProject, &QAction::triggered, this, &ProjectLoadedState::closeProject);
    connect(m_openFile, &QAction::triggered, this, &ProjectLoadedState::openFile);
    connect(m_closeFile, &QAction::triggered, this, &ProjectLoadedState::closeFile);
    connect(m_saveFile, &QAction::triggered, m_editorWidget, &EditorWidget::saveCurrentFile);
    connect(m_saveAllFiles, &QAction::triggered, m_editorWidget, &EditorWidget::saveAllFiles);
}





QAction* ProjectLoadedState::getCloseProjectAction() const
{
    return m_closeProject;
}





QAction* ProjectLoadedState::getOpenFileAction() const
{
    return m_openFile;
}





QAction* ProjectLoadedState::getCloseFileAction() const
{
    return m_closeFile;
}





QAction* ProjectLoadedState::getSaveFileAction() const
{
    return m_saveFile;
}





QAction* ProjectLoadedState::getSaveAllFilesAction() const
{
    return m_saveAllFiles;
}





void ProjectLoadedState::onEntry(QEvent* event)
{
    m_closeProject->setEnabled(true);
    m_openFile->setEnabled(true);
    m_closeFile->setEnabled(true);
    m_saveFile->setEnabled(true);
    m_saveAllFiles->setEnabled(true);
    
    if (event->type() == QEvent::StateMachineSignal)
    {
        QStateMachine::SignalEvent* signalEvent(static_cast<QStateMachine::SignalEvent*>(event));
        QModelIndex projectIndex(signalEvent->arguments().first().toModelIndex());
        
        m_currentProject = m_projectManager->getProject(projectIndex);
        
        m_openedFileDock->setCurrentProject(m_currentProject);
        m_editorWidget->setCurrentProject(m_currentProject);
        
        m_connectionFileOpened = connect(m_currentProject, &Project::fileOpened, m_openedFileDock, &OpenedFileDock::setCurrentFile);
        m_connectionFileOpeningRequested = connect(m_projectFileDock, &ProjectFileDock::fileOpeningRequested, m_currentProject, &Project::openFile);
    }
}





void ProjectLoadedState::onExit(QEvent* /*event*/)
{
    m_closeProject->setDisabled(true);
    m_openFile->setDisabled(true);
    m_closeFile->setDisabled(true);
    m_saveFile->setDisabled(true);
    m_saveAllFiles->setDisabled(true);
    
    disconnect(m_connectionFileOpened);
    disconnect(m_connectionFileOpeningRequested);
}





void ProjectLoadedState::openFile()
{
    QString path(QFileDialog::getOpenFileName(m_editorWidget, tr("Select a file"), m_currentProject->getRootPath()));
    if (!path.isEmpty())
    {
        m_currentProject->openFile(path);
    }
}





void ProjectLoadedState::closeFile()
{
    FileWidget* widget(0);
    QModelIndex fileIndex(m_openedFileDock->getCurrentFileIndex());
    if (m_currentProject->closeFile(fileIndex, widget) && widget)
    {
        m_editorWidget->deleteFileWidget(widget);
    }
}





void ProjectLoadedState::closeProject()
{
    m_projectManager->deleteProject(m_currentProject);
}
