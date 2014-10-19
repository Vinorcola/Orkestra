#ifndef FILESELECTORWIDGET_HPP
#define FILESELECTORWIDGET_HPP

#include <QLineEdit>
#include <QPushButton>





class FileSelectorWidget : public QWidget
{
        Q_OBJECT
        Q_PROPERTY(QString filePath READ getFilePath)
        
        
        
    private:
        QLineEdit* m_input;
        QPushButton* m_button;
        bool m_isValid;
        const bool m_selectDirectory;
        QPalette m_defaultPalette;
        QPalette m_errorPalette;
        
        
        
    public:
        explicit FileSelectorWidget(const bool selectDirectory = false);
        
        
        
        bool isPathValid() const;
        
        
        
        QString getFilePath() const;
        
        
        
    signals:
        void filePathChanged(QString path);
        
        
        
    private slots:
        void browse();
        
        
        
        void analyzePath(const QString& filePath);
};

#endif // FILESELECTORWIDGET_HPP
