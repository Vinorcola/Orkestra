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
        QAction* m_saveFile;
        MainWindow* m_window;
        
        
        
    public:
        MainController();
        
        
        
        virtual ~MainController();
        
        
        
    public slots:
        void start();
        
        
        
        void displayProjectImportator();
        
        
        
    private slots:
        void quit();
};

#endif // MAINCONTROLLER_HPP
