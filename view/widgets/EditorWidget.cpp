#include "EditorWidget.hpp"





EditorWidget::EditorWidget() :
    QStackedWidget(),
    m_currentProject(0)
{
    
}





void EditorWidget::setCurrentProject(Project* project)
{
    clear();
    
    const QList<File*>& files(project->getFiles());
    for (int i(0), iEnd(files.size()); i < iEnd; ++i)
    {
        addWidget(files.at(i)->getWidget());
    }
    
    m_currentProject = project;
}





void EditorWidget::clear()
{
    for (int i(0), iEnd(count()); i < iEnd; ++i)
    {
        removeWidget(currentWidget());
    }
}





void EditorWidget::setCurrentFile(const QModelIndex& fileIndex)
{
    QWidget* widget(m_currentProject->getFile(fileIndex)->getWidget());
    if (indexOf(widget) == -1)
    {
        addWidget(widget);
    }
    setCurrentWidget(widget);
}





void EditorWidget::saveCurrentFile()
{
    FileWidget* widget(static_cast<FileWidget*>(currentWidget()));
    if (widget)
    {
        widget->save();
    }
}





void EditorWidget::saveAllFiles()
{
    for (int i(0), iEnd(count()); i < iEnd; ++i)
    {
        static_cast<FileWidget*>(widget(i))->save();
    }
}
