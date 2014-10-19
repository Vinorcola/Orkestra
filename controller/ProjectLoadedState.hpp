#ifndef PROJECTLOADEDSTATE_HPP
#define PROJECTLOADEDSTATE_HPP

#include <QMetaObject>
#include <QState>

#include "model/businessobjects/ProjectManager.hpp"
#include "view/docks/OpenedFileDock.hpp"
#include "view/docks/ProjectFileDock.hpp"
#include "view/widgets/EditorWidget.hpp"





class ProjectLoadedState : public QState
{
        Q_OBJECT
        
        
        
    private:
        const ProjectManager* m_projectManager;
        Project* m_currentProject;
        EditorWidget* m_editorWidget;
        ProjectFileDock* m_projectFileDock;
        OpenedFileDock* m_openedFileDock;
        QMetaObject::Connection m_connectionFileOpened;
        QMetaObject::Connection m_connectionFileOpeningRequested;
        
        
        
    public:
        ProjectLoadedState(const ProjectManager* projectManager,
                           EditorWidget* editorWidget,
                           ProjectFileDock* projectFileDock,
                           OpenedFileDock* openedFileDock);
        
        
        
    public slots:
        virtual void onEntry(QEvent* event);
        
        
        
        virtual void onExit(QEvent *event);
};

#endif // PROJECTLOADEDSTATE_HPP
