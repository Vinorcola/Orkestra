#ifndef COMPOSERCONFIGDIALOG_HPP
#define COMPOSERCONFIGDIALOG_HPP

#include <QDialog>
#include <QLabel>
#include <QLineEdit>

#include "model/config/ComposerConfig.hpp"





class ComposerConfigDialog : public QDialog
{
        Q_OBJECT
        
        
        
    private:
        static const QString NON_TESTED;
        static const QString TEST_FAILED;
        static const QString TEST_SUCCEEDED;
        QLineEdit* m_commandInput;
        QLabel* m_testLabel;
        
        
        
    public:
        ComposerConfigDialog(const QString& defaultCommand,
                             QWidget* parent);
        
        
        
        QString getCommand() const;
        
        
        
    public slots:
        void test();
        
        
        
        void update();
};

#endif // COMPOSERCONFIGDIALOG_HPP
