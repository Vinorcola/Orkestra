#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QAction>
#include <QMainWindow>

#include "view/docks/OpenedFileDock.hpp"
#include "view/docks/ProjectFileDock.hpp"
#include "view/widgets/EditorWidget.hpp"





class MainWindow : public QMainWindow
{
        Q_OBJECT
        
        
        
    private:
        EditorWidget* m_editorWidget;
        ProjectFileDock* m_projectDock;
        OpenedFileDock* m_openedFileDock;
        QAction* m_quitAction;
        ProjectManager* m_projectManager;
        
        
        
    public:
        MainWindow(ProjectManager* projectManager,
                   QAction* quit,
                   QAction* saveFile);
        
        
        
    public slots:
        void setupCurrentProject(const QModelIndex& projectIndex);
        
        
        
    protected:
        virtual void closeEvent(QCloseEvent* event);
};

#endif // MAINWINDOW_HPP
