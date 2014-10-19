#include "OpenedFileDock.hpp"





OpenedFileDock::OpenedFileDock() :
    QDockWidget(),
    m_view(new QListView),
    m_lastConnectionFromProject(),
    m_lastConnectionFromView()
{
    setWidget(m_view);
}





void OpenedFileDock::setCurrentProject(Project* project)
{
    /* NOTE: We disconnect the view before changing anything in the model.
     */
    if (m_lastConnectionFromView)
    {
        disconnect(m_lastConnectionFromView);
    }
    
    m_view->setModel(project);
    m_view->setCurrentIndex(project->index(0));// We select the first row.
    
    if (m_lastConnectionFromProject)
    {
        disconnect(m_lastConnectionFromProject);
    }
    m_lastConnectionFromProject = connect(project, &Project::fileOpened, m_view, &QListView::setCurrentIndex);
    m_lastConnectionFromView = connect(m_view->selectionModel(), &QItemSelectionModel::currentChanged, this, &OpenedFileDock::fileChanged);
}
