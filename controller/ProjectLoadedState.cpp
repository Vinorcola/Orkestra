#include "ProjectLoadedState.hpp"

#include <QStateMachine>





ProjectLoadedState::ProjectLoadedState(const ProjectManager* projectManager,
                                       EditorWidget* editorWidget,
                                       ProjectFileDock* projectFileDock,
                                       OpenedFileDock* openedFileDock) :
    QState(),
    m_projectManager(projectManager),
    m_currentProject(0),
    m_editorWidget(editorWidget),
    m_projectFileDock(projectFileDock),
    m_openedFileDock(openedFileDock),
    m_connectionFileOpened(),
    m_connectionFileOpeningRequested()
{
    
}





void ProjectLoadedState::onEntry(QEvent* event)
{
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
    disconnect(m_connectionFileOpened);
    disconnect(m_connectionFileOpeningRequested);
}
