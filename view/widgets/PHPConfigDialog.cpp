#include "PHPConfigDialog.hpp"

#include <QDialogButtonBox>
#include <QFormLayout>
#include <QPushButton>

#include "view/widgets/CommandLineLauncherDialog.hpp"





const QString PHPConfigDialog::NON_TESTED(tr("Non tested"));
const QString PHPConfigDialog::TEST_FAILED(tr("Unvalid command"));
const QString PHPConfigDialog::TEST_SUCCEEDED(tr("Valid command"));





PHPConfigDialog::PHPConfigDialog(const QString& defaultCommand,
                                 QWidget* parent) :
    QDialog(parent),
    m_commandInput(new QLineEdit(defaultCommand)),
    m_testLabel(new QLabel(NON_TESTED))
{
    setWindowTitle(tr("PHP configuration"));
    setMinimumSize(600, 400);
    
    
    
    QPushButton* testButton(new QPushButton(tr("Test")));
    
    QDialogButtonBox* buttons(new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel));
    
    QHBoxLayout* commandLayout(new QHBoxLayout);
    commandLayout->addWidget(m_commandInput);
    commandLayout->addWidget(m_testLabel);
    commandLayout->addWidget(testButton);
    
    QFormLayout* formLayout(new QFormLayout);
    formLayout->addRow(tr("PHP command:"), commandLayout);
    
    QVBoxLayout* layout(new QVBoxLayout);
    layout->addLayout(formLayout);
    layout->addStretch();
    layout->addWidget(buttons);
    
    setLayout(layout);
    
    connect(m_commandInput, &QLineEdit::textChanged, [=]()
    {
        m_testLabel->setText(NON_TESTED);
    });
    connect(testButton, &QPushButton::clicked, this, &PHPConfigDialog::test);
    connect(buttons, &QDialogButtonBox::accepted, this, &PHPConfigDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &PHPConfigDialog::reject);
}





QString PHPConfigDialog::getCommand() const
{
    return m_commandInput->text();
}





void PHPConfigDialog::test()
{
    CommandLineLauncherDialog dialog(this, m_commandInput->text(), QStringList({"--version"}));
    if (dialog.exec() == QDialog::Accepted)
    {
        if (dialog.processTerminatedCorrectly())
        {
            m_testLabel->setText(TEST_SUCCEEDED);
        }
        else
        {
            m_testLabel->setText(TEST_FAILED);
        }
    }
}
