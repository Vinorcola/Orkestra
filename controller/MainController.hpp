#ifndef MAINCONTROLLER_HPP
#define MAINCONTROLLER_HPP

#include <QStateMachine>

#include "model/businessobjects/ProjectManager.hpp"
#include "view/widgets/MainWindow.hpp"





class MainController :public QStateMachine
{
        Q_OBJECT
        
        
        
    private:
        Config* m_config;
        ProjectManager* m_projectManager;
        QAction* m_quit;
        QAction* m_importProject;
        QAction* m_phpConfig;
        QAction* m_composerConfig;
        EditorWidget* m_editorWidget;
        ProjectFileDock* m_projectFileDock;
        OpenedFileDock* m_openedFileDock;
        MainWindow* m_window;
        
        
        
    public:
        MainController();
        
        
        
        virtual ~MainController();
        
        
        
    public slots:
        void init();
        
        
        
        void displayProjectImportator();
        
        
        
        void configurePHP();
        
        
        
        void configureComposer();
        
        
        
    private slots:
        void quit();
};

#endif // MAINCONTROLLER_HPP
