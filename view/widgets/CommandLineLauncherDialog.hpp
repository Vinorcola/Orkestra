#ifndef COMMANDLINELAUNCHERDIALOG_HPP
#define COMMANDLINELAUNCHERDIALOG_HPP

#include <QDialog>
#include <QDialogButtonBox>
#include <QProcess>
#include <QTextEdit>





class CommandLineLauncherDialog : public QDialog
{
        Q_OBJECT
        
        
        
    private:
        QTextEdit* m_console;
        QProcess m_process;
        QDialogButtonBox* m_buttons;
        bool m_processError;
        
        
        
    public:
        CommandLineLauncherDialog(QWidget* parent,
                                  const QString& workingDirectory,
                                  const QString& command,
                                  const QStringList& arguments = QStringList());
        
        
        
        CommandLineLauncherDialog(QWidget* parent,
                                  const QString& command,
                                  const QStringList& arguments = QStringList());
        
        
        
        bool processTerminatedCorrectly() const;
        
        
        
    public slots:
        void updateOutput();
        
        
        
        void processFinish();
        
        
        
        virtual int exec();
        
        
        
        virtual void reject();
};

#endif // COMMANDLINELAUNCHERDIALOG_HPP
