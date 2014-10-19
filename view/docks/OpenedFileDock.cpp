#include "OpenedFileDock.hpp"





OpenedFileDock::OpenedFileDock() :
    QDockWidget(),
    m_view(new QListView),
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
    
    m_lastConnectionFromView = connect(m_view->selectionModel(), &QItemSelectionModel::currentChanged, this, &OpenedFileDock::fileChanged);
}





void OpenedFileDock::setCurrentFile(const QModelIndex& fileIndex)
{
    m_view->setCurrentIndex(fileIndex);
}
