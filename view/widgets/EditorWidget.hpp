#ifndef EDITORWIDGET_HPP
#define EDITORWIDGET_HPP

#include <QStackedWidget>

#include "model/businessobjects/Project.hpp"





class EditorWidget : public QStackedWidget
{
        Q_OBJECT
        
        
        
    private:
        Project* m_currentProject;
        
        
        
    public:
        EditorWidget();
        
        
        
        void setCurrentProject(Project* project);
        
        
        
        void clear();
        
        
        
    public slots:
        void setCurrentFile(const QModelIndex& fileIndex);
};

#endif // EDITORWIDGET_HPP
