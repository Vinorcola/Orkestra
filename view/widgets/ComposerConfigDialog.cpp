#include "ComposerConfigDialog.hpp"

#include <QDialogButtonBox>
#include <QFormLayout>
#include <QProcess>
#include <QPushButton>

#include "view/widgets/CommandLineLauncherDialog.hpp"





const QString ComposerConfigDialog::NON_TESTED(tr("Non tested"));
const QString ComposerConfigDialog::TEST_FAILED(tr("Unvalid command"));
const QString ComposerConfigDialog::TEST_SUCCEEDED(tr("Valid command"));





ComposerConfigDialog::ComposerConfigDialog(const QString& defaultCommand,
                                           QWidget* parent) :
    QDialog(parent),
    m_commandInput(new QLineEdit(defaultCommand)),
    m_testLabel(new QLabel(NON_TESTED))
{
    setMinimumSize(600, 400);
    
    QPushButton* testButton(new QPushButton(tr("Test")));
    QPushButton* updateButton(new QPushButton(tr("Update Composer")));
    
    QDialogButtonBox* buttons(new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel));
    
    QHBoxLayout* commandLayout(new QHBoxLayout);
    commandLayout->addWidget(m_commandInput);
    commandLayout->addWidget(m_testLabel);
    commandLayout->addWidget(testButton);
    
    QFormLayout* formLayout(new QFormLayout);
    formLayout->addRow(tr("Composer command:"), commandLayout);
    formLayout->addRow(tr("Update Composer:"), updateButton);
    
    QVBoxLayout* layout(new QVBoxLayout);
    layout->addLayout(formLayout);
    layout->addStretch();
    layout->addWidget(buttons);
    
    setLayout(layout);
    
    connect(m_commandInput, &QLineEdit::textChanged,
            [=]()
    {
        m_testLabel->setText(NON_TESTED);
    });
    connect(testButton, &QPushButton::clicked, this, &ComposerConfigDialog::test);
    connect(updateButton, &QPushButton::clicked, this, &ComposerConfigDialog::update);
    connect(buttons, &QDialogButtonBox::accepted, this, &ComposerConfigDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &ComposerConfigDialog::reject);
}





QString ComposerConfigDialog::getCommand() const
{
    return m_commandInput->text();
}





void ComposerConfigDialog::test()
{
    CommandLineLauncherDialog dialog(this, m_commandInput->text());
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





void ComposerConfigDialog::update()
{
    CommandLineLauncherDialog dialog(this, m_commandInput->text(), {"self-update"});
    dialog.exec();
}
