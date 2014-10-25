#include "CommandLineLauncherDialog.hpp"

#include <QPushButton>
#include <QVBoxLayout>





CommandLineLauncherDialog::CommandLineLauncherDialog(QWidget* parent,
                                                     const QString& command,
                                                     const QStringList& arguments) :
    QDialog(parent),
    m_console(new QTextEdit),
    m_process(),
    m_buttons(new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel)),
    m_processError(false)
{
    setMinimumSize(600, 400);
    
    
    
    // Configure the console.
    m_console->setFont(QFont("Monospace", 9));
    m_console->setStyleSheet(QStringLiteral("background-color: #000; color: #ddd;"));
    
    
    
    // Configure the dialog.
    m_buttons->button(QDialogButtonBox::Ok)->setDisabled(true);
    
    QVBoxLayout* layout(new QVBoxLayout);
    layout->addWidget(m_console);
    layout->addWidget(m_buttons);
    
    setLayout(layout);
    
    connect(m_buttons, &QDialogButtonBox::accepted, this, &CommandLineLauncherDialog::accept);
    connect(m_buttons, &QDialogButtonBox::rejected, this, &CommandLineLauncherDialog::reject);
    
    
    
    // Configure the process.
    m_process.setProgram(command);
    if (!arguments.isEmpty())
    {
        m_process.setArguments(arguments);
    }
    
    connect(&m_process, &QProcess::readyReadStandardOutput, this, &CommandLineLauncherDialog::updateOutput);
    connect(&m_process, static_cast<void (QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished), this, &CommandLineLauncherDialog::processFinish);
    connect(&m_process, static_cast<void (QProcess::*)(QProcess::ProcessError)>(&QProcess::error), [=]()
    {
        m_processError = true;
    });
    connect(&m_process, static_cast<void (QProcess::*)(QProcess::ProcessError)>(&QProcess::error), this, &CommandLineLauncherDialog::processFinish);
}





bool CommandLineLauncherDialog::processTerminatedCorrectly() const
{
    return !m_processError;
}





void CommandLineLauncherDialog::updateOutput()
{
    m_console->append(m_process.readAllStandardOutput());
}





void CommandLineLauncherDialog::processFinish()
{
    m_buttons->button(QDialogButtonBox::Ok)->setEnabled(true);
    m_buttons->button(QDialogButtonBox::Cancel)->setDisabled(true);
}





int CommandLineLauncherDialog::exec()
{
    m_process.start();
    
    return QDialog::exec();
}





void CommandLineLauncherDialog::reject()
{
    if (m_process.state() != QProcess::NotRunning)
    {
        m_process.terminate();
    }
    
    QDialog::reject();
}
