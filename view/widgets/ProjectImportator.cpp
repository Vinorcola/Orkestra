#include "ProjectImportator.hpp"

#include <QDialogButtonBox>
#include <QFormLayout>





ProjectImportator::ProjectImportator(QWidget* parent) :
    QDialog(parent),
    m_nameInput(new QLineEdit),
    m_rootPathInput(new FileSelectorWidget(true))
{
    setWindowTitle(tr("Import an existing project"));
    
    QDialogButtonBox* buttons(new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel));
    
    QFormLayout* formLayout(new QFormLayout);
    formLayout->addRow(tr("Name"), m_nameInput);
    formLayout->addRow(tr("Root path"), m_rootPathInput);
    
    QVBoxLayout* layout(new QVBoxLayout);
    layout->addLayout(formLayout);
    layout->addStretch(1);
    layout->addWidget(buttons);
    
    setLayout(layout);
    
    connect(buttons, &QDialogButtonBox::accepted, this, &ProjectImportator::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &ProjectImportator::reject);
}





QString ProjectImportator::getProjectName() const
{
    return m_nameInput->text();
}





QString ProjectImportator::getProjectRootPath() const
{
    return m_rootPathInput->getFilePath();
}
