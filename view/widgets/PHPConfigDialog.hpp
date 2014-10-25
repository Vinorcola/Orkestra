#ifndef PHPCONFIGDIALOG_HPP
#define PHPCONFIGDIALOG_HPP

#include <QDialog>
#include <QLabel>
#include <QLineEdit>





class PHPConfigDialog : public QDialog
{
        Q_OBJECT
        
        
        
    private:
        static const QString NON_TESTED;
        static const QString TEST_FAILED;
        static const QString TEST_SUCCEEDED;
        QLineEdit* m_commandInput;
        QLabel* m_testLabel;
        
        
        
    public:
        PHPConfigDialog(const QString& defaultCommand,
                        QWidget* parent);
        
        
        
        QString getCommand() const;
        
        
        
    public slots:
        void test();
};

#endif // PHPCONFIGDIALOG_HPP
