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
        QAction* m_quitAction;
        
        
        
    public:
        MainWindow(QAction* quit,
                   EditorWidget* editorWidget,
                   ProjectFileDock* projectFileDock,
                   OpenedFileDock* openedFileDock);
        
        
        
    protected:
        virtual void closeEvent(QCloseEvent* event);
};

#endif // MAINWINDOW_HPP
