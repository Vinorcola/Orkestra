#include "FileSelectorWidget.hpp"

#include <QFileDialog>
#include <QHBoxLayout>





FileSelectorWidget::FileSelectorWidget(const bool selectDirectory) :
    QWidget(),
    m_input(new QLineEdit),
    m_button(new QPushButton(tr("Browse..."))),
    m_isValid(false),
    m_selectDirectory(selectDirectory),
    m_defaultPalette(m_input->palette()),
    m_errorPalette(m_defaultPalette)
{
    m_errorPalette.setColor(QPalette::Text, Qt::red);
    
    QHBoxLayout* layout(new QHBoxLayout);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_input);
    layout->addWidget(m_button);
    
    setLayout(layout);
    
    connect(m_input, &QLineEdit::textChanged, this, &FileSelectorWidget::analyzePath);
    connect(m_button, &QPushButton::clicked, this, &FileSelectorWidget::browse);
}





bool FileSelectorWidget::isPathValid() const
{
    return m_isValid;
}





QString FileSelectorWidget::getFilePath() const
{
    return m_input->text();
}





void FileSelectorWidget::browse()
{
    QString currentFilePath(getFilePath());
    QString newFilePath(
        m_selectDirectory ?
        QFileDialog::getExistingDirectory(this, tr("Select a directory"), currentFilePath) :
        QFileDialog::getOpenFileName(this, tr("Select a file"), currentFilePath)
    );
    
    if (!newFilePath.isEmpty() && newFilePath != currentFilePath)
    {
        m_input->setText(newFilePath);
    }
}





void FileSelectorWidget::analyzePath(const QString& filePath)
{
    QFileInfo fileInfo(filePath);
    m_isValid = fileInfo.exists() && (!m_selectDirectory || (m_selectDirectory && fileInfo.isDir()));
    
    if (m_isValid)
    {
        m_input->setPalette(m_defaultPalette);
    }
    else
    {
        m_input->setPalette(m_errorPalette);
    }
    
    emit filePathChanged(filePath);
}
