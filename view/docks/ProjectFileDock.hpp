#ifndef PROJECTFILEDOCK_HPP
#define PROJECTFILEDOCK_HPP

#include <QComboBox>
#include <QDockWidget>
#include <QFileSystemModel>
#include <QMetaObject>
#include <QTreeView>

#include "model/businessobjects/ProjectManager.hpp"





class ProjectFileDock : public QDockWidget
{
        Q_OBJECT
        
        
        
    private:
        QFileSystemModel* m_projectFileModel;
        QTreeView* m_projectFileView;
        ProjectManager* m_projectsModel;
        QComboBox* m_projectsView;
        QMetaObject::Connection m_lastConnection;
        
        
        
    public:
        explicit ProjectFileDock(ProjectManager* projectManager);
        
        
        
    signals:
        void projectChanged(const QModelIndex& projectIndex);
        
        
        
        void fileOpeningRequested(const QString& filePath);
        
        
        
    private slots:
        void setupCurrentProject(int projectRow);
        
        
        
        void handleDoubleClick(const QModelIndex& index);
};

#endif // PROJECTFILEDOCK_HPP
