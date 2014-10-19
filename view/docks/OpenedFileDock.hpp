#ifndef OPENEDFILEDOCK_HPP
#define OPENEDFILEDOCK_HPP

#include <QDockWidget>
#include <QListView>

#include "model/businessobjects/Project.hpp"





class OpenedFileDock : public QDockWidget
{
        Q_OBJECT
        
        
        
    private:
        QListView* m_view;
        QMetaObject::Connection m_lastConnectionFromView;
        
        
        
    public:
        OpenedFileDock();
        
        
        
        void setCurrentProject(Project* project);
        
        
        
    signals:
        void fileChanged(const QModelIndex& fileIndex);
        
        
        
    public slots:
        void setCurrentFile(const QModelIndex& fileIndex);
};

#endif // OPENEDFILEDOCK_HPP
