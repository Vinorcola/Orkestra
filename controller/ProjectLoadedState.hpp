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
        ProjectManager* m_projectManager;
        Project* m_currentProject;
        QAction* m_closeProject;
        QAction* m_openFile;
        QAction* m_closeFile;
        QAction* m_saveFile;
        QAction* m_saveAllFiles;
        EditorWidget* m_editorWidget;
        ProjectFileDock* m_projectFileDock;
        OpenedFileDock* m_openedFileDock;
        QMetaObject::Connection m_connectionFileOpened;
        QMetaObject::Connection m_connectionFileOpeningRequested;
        
        
        
    public:
        ProjectLoadedState(ProjectManager* projectManager,
                           EditorWidget* editorWidget,
                           ProjectFileDock* projectFileDock,
                           OpenedFileDock* openedFileDock);
        
        
        
        QAction* getCloseProjectAction() const;
        
        
        
        QAction* getOpenFileAction() const;
        
        
        
        QAction* getCloseFileAction() const;
        
        
        
        QAction* getSaveFileAction() const;
        
        
        
        QAction* getSaveAllFilesAction() const;
        
        
        
    public slots:
        virtual void onEntry(QEvent* event);
        
        
        
        virtual void onExit(QEvent *event);
        
        
        
        void openFile();
        
        
        
        void closeFile();
        
        
        
        void closeProject();
};

#endif // PROJECTLOADEDSTATE_HPP
