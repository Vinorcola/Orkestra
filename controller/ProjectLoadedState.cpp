#include "ProjectLoadedState.hpp"

#include <QAction>
#include <QFileDialog>
#include <QStateMachine>





ProjectLoadedState::ProjectLoadedState(const ProjectManager* projectManager,
                                       EditorWidget* editorWidget,
                                       ProjectFileDock* projectFileDock,
                                       OpenedFileDock* openedFileDock) :
    QState(),
    m_projectManager(projectManager),
    m_currentProject(0),
    m_openFile(new QAction(tr("Open"), this)),
    m_saveFile(new QAction(tr("Save"), this)),
    m_saveAllFiles(new QAction(tr("Save all"), this)),
    m_editorWidget(editorWidget),
    m_projectFileDock(projectFileDock),
    m_openedFileDock(openedFileDock),
    m_connectionFileOpened(),
    m_connectionFileOpeningRequested()
{
    m_openFile->setShortcut(Qt::CTRL + Qt::Key_O);
    m_openFile->setDisabled(true);
    m_saveFile->setShortcut(Qt::CTRL + Qt::Key_S);
    m_saveFile->setDisabled(true);
    m_saveAllFiles->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_S);
    m_saveAllFiles->setDisabled(true);
    
    connect(m_openedFileDock, &OpenedFileDock::fileChanged, m_editorWidget, &EditorWidget::setCurrentFile);
    
    connect(m_openFile, &QAction::triggered, this, &ProjectLoadedState::openFile);
    connect(m_saveFile, &QAction::triggered, m_editorWidget, &EditorWidget::saveCurrentFile);
    connect(m_saveAllFiles, &QAction::triggered, m_editorWidget, &EditorWidget::saveAllFiles);
}





QAction* ProjectLoadedState::getOpenFileAction() const
{
    return m_openFile;
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
    m_openFile->setEnabled(true);
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
        m_connectionFileOpeningRequested = connect(m_projectFileDock, &ProjectFileDock::fileOpeningRequested, m_currentProject, &Project::open);
    }
}





void ProjectLoadedState::onExit(QEvent* /*event*/)
{
    m_openFile->setDisabled(true);
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
        m_currentProject->open(path);
    }
}
