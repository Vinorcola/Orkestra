#ifndef PROJECTIMPORTATOR_HPP
#define PROJECTIMPORTATOR_HPP

#include <QDialog>

#include "view/widgets/FileSelectorWidget.hpp"





class ProjectImportator : public QDialog
{
        Q_OBJECT
        
        
        
    private:
        QLineEdit* m_nameInput;
        FileSelectorWidget* m_rootPathInput;
        
        
        
    public:
        ProjectImportator(QWidget* parent);
        
        
        
        QString getProjectName() const;
        
        
        
        QString getProjectRootPath() const;
};

#endif // PROJECTIMPORTATOR_HPP
