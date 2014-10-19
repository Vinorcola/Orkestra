#include "ProjectFileDock.hpp"

#include <QVBoxLayout>





ProjectFileDock::ProjectFileDock(ProjectManager* projectManager) :
    QDockWidget(),
    m_projectFileModel(new QFileSystemModel(this)),
    m_projectFileView(new QTreeView),
    m_projectsModel(projectManager),
    m_projectsView(new QComboBox)
{
    m_projectFileView->setModel(m_projectFileModel);
    m_projectFileView->setColumnHidden(1, true);// Size
    m_projectFileView->setColumnHidden(2, true);// Type
    m_projectFileView->setColumnHidden(3, true);// Last update
    m_projectFileView->setHeaderHidden(true);
    m_projectFileView->setSelectionMode(QAbstractItemView::NoSelection);
    
    m_projectsView->setModel(projectManager);
    
    QWidget* widget(new QWidget);
    QVBoxLayout* layout(new QVBoxLayout);
    layout->addWidget(m_projectsView);
    layout->addWidget(m_projectFileView);
    widget->setLayout(layout);
    
    setWidget(widget);
    
    connect(m_projectsView, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &ProjectFileDock::setupCurrentProject);
    connect(projectManager, &ProjectManager::projectOpened, [=](const QModelIndex& projectIndex)
    {
        m_projectsView->setCurrentIndex(projectIndex.row());
    });
    connect(m_projectFileView, &QTreeView::doubleClicked, this, &ProjectFileDock::handleDoubleClick);
}





void ProjectFileDock::setupCurrentProject(int projectRow)
{
    QModelIndex projectIndex(m_projectsModel->index(projectRow));
    Project* project(m_projectsModel->getProject(projectIndex));
    if (project)
    {
        m_projectFileView->setRootIndex(m_projectFileModel->setRootPath(project->getRootPath()));
        
        emit projectChanged(projectIndex);
    }
}





void ProjectFileDock::handleDoubleClick(const QModelIndex& index)
{
    QString filePath(m_projectFileModel->filePath(index));
    QFileInfo fileInfo(filePath);
    
    if (fileInfo.isFile())
    {
        emit fileOpeningRequested(filePath);
    }
}
