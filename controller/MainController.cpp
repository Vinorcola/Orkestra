#include "MainController.hpp"

#include <QApplication>
#include <QFileDialog>
#include <QFinalState>
#include <QMenu>
#include <QMenuBar>

#include "controller/ProjectLoadedState.hpp"
#include "view/widgets/ComposerConfigDialog.hpp"
#include "view/widgets/ProjectImportator.hpp"
#include "view/widgets/PHPConfigDialog.hpp"





MainController::MainController() :
    QStateMachine(),
    m_config(new Config(this)),
    m_projectManager(new ProjectManager(*m_config, this)),
    m_quit(new QAction(tr("Quit"), this)),
    m_importProject(new QAction(tr("Import a project"), this)),
    m_phpConfig(new QAction(tr("PHP"), this)),
    m_composerConfig(new QAction(tr("Composer"), this)),
    m_editorWidget(new EditorWidget),
    m_projectFileDock(new ProjectFileDock(m_projectManager)),
    m_openedFileDock(new OpenedFileDock),
    m_window(new MainWindow(m_quit, m_editorWidget, m_projectFileDock, m_openedFileDock))
{
    // State configuration.
    QState* noProjectLoaded(new QState);
    addState(noProjectLoaded);
    setInitialState(noProjectLoaded);
    
    ProjectLoadedState* projectLoaded(new ProjectLoadedState(m_projectManager, m_editorWidget, m_projectFileDock, m_openedFileDock));
    addState(projectLoaded);
    
    QFinalState* quit(new QFinalState);
    addState(quit);
    
    
    
    // Transition configuration.
    /* NOTE: Be careful that every transition that enter projectLoaded state cary a QModelIndex to the project as the
     * first argument.
     */
    noProjectLoaded->addTransition(m_quit, SIGNAL(triggered()), quit);
    noProjectLoaded->addTransition(m_projectFileDock, SIGNAL(projectChanged(QModelIndex)), projectLoaded);
    
    projectLoaded->addTransition(m_quit, SIGNAL(triggered()), quit);
    projectLoaded->addTransition(m_projectFileDock, SIGNAL(projectChanged(QModelIndex)), projectLoaded);
    
    
    
    // Entry and exit configuration.
    connect(noProjectLoaded, &QState::entered, this, &MainController::init);
    connect(quit, &QState::entered, this, &MainController::quit);
    
    
    
    // Setup main window menus.
    QMenu* menuApplication(m_window->menuBar()->addMenu(tr("Orkestra")));
    menuApplication->addAction(m_importProject);
    menuApplication->addAction(projectLoaded->getCloseProjectAction());
    menuApplication->addSeparator();
    menuApplication->addAction(projectLoaded->getOpenFileAction());
    menuApplication->addAction(projectLoaded->getCloseFileAction());
    menuApplication->addAction(projectLoaded->getSaveFileAction());
    menuApplication->addAction(projectLoaded->getSaveAllFilesAction());
    menuApplication->addSeparator();
    menuApplication->addAction(m_quit);
    
    QMenu* menuConfiguration(m_window->menuBar()->addMenu(tr("Configuration")));
    menuConfiguration->addAction(m_phpConfig);
    menuConfiguration->addAction(m_composerConfig);
    
    
    
    // Setup actions.
    m_quit->setShortcut(Qt::CTRL + Qt::Key_Q);
    m_importProject->setShortcut(Qt::CTRL + Qt::Key_I);
    
    connect(m_importProject, &QAction::triggered, this, &MainController::displayProjectImportator);
    connect(m_composerConfig, &QAction::triggered, this, &MainController::configureComposer);
    connect(m_phpConfig, &QAction::triggered, this, &MainController::configurePHP);
}





MainController::~MainController()
{
    delete m_window;
}





void MainController::init()
{
    m_config->load(m_projectManager);
    m_window->show();
}





void MainController::displayProjectImportator()
{
    ProjectImportator dialog(m_window);
    if (dialog.exec() == QDialog::Accepted)
    {
        m_projectManager->addProject(dialog.getProjectName(), dialog.getProjectRootPath());
        /* NOTE: We do nothing more here. The project manager object will notify the project file dock that a new
         * project was added. The dock will update the current project and trigger the projectChanged() signal. This
         * signal will push the controller in the projectLoaded state. All the work will be done on project loaded state
         * entry.
         */
    }
}





void MainController::configurePHP()
{
    PHPConfigDialog dialog(m_config->getPHPConfig().getCommand(), m_window);
    if (dialog.exec() == QDialog::Accepted)
    {
        m_config->getPHPConfig().setCommand(dialog.getCommand());
    }
}





void MainController::configureComposer()
{
    ComposerConfigDialog dialog(m_config->getComposerConfig().getCommand(), m_window);
    if (dialog.exec() == QDialog::Accepted)
    {
        m_config->getComposerConfig().setCommand(dialog.getCommand());
    }
}





void MainController::quit()
{
    /* NOTE: Do here whatever needs to be done before quitting the application (e.g. save config)
     */
    m_config->save(m_projectManager);
    
    qApp->quit();
}
