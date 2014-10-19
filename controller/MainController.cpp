#include "MainController.hpp"

#include <QApplication>
#include <QFinalState>
#include <QMenu>
#include <QMenuBar>

#include "view/widgets/ProjectImportator.hpp"





#include <QDebug>
MainController::MainController() :
    QStateMachine(),
    m_config(new Config(this)),
    m_projectManager(new ProjectManager(*m_config, this)),
    m_quit(new QAction(tr("Quit"), this)),
    m_importProject(new QAction(tr("Import a project"), this)),
    m_saveFile(new QAction(tr("Save"), this)),
    m_window(new MainWindow(m_projectManager, m_quit, m_saveFile))
{
    // Setup main window menus.
    QMenu* menuApplication(m_window->menuBar()->addMenu(tr("Orkestra")));
    menuApplication->addAction(m_importProject);
    menuApplication->addSeparator();
    menuApplication->addAction(m_saveFile);
    menuApplication->addSeparator();
    menuApplication->addAction(m_quit);
    
    
    
    // Setup actions.
    m_quit->setShortcut(Qt::CTRL + Qt::Key_Q);
    m_importProject->setShortcut(Qt::CTRL + Qt::Key_I);
    m_saveFile->setShortcut(Qt::CTRL + Qt::Key_S);
    
    connect(m_importProject, &QAction::triggered, this, &MainController::displayProjectImportator);
    
    
    
    // State configuration.
    QState* noProjectLoaded(new QState);
    addState(noProjectLoaded);
    setInitialState(noProjectLoaded);
    
    QState* projectLoaded(new QState);
    addState(projectLoaded);
    
    QFinalState* quit(new QFinalState);
    addState(quit);
    
    
    
    // Transition configuration.
    /* NOTE: Be careful that every transition that enter projectLoaded state cary a QModelIndex to the project as the
     * first argument.
     */
    noProjectLoaded->addTransition(m_quit, SIGNAL(triggered()), quit);
    noProjectLoaded->addTransition(m_projectManager, SIGNAL(projectOpened(QModelIndex)), projectLoaded);
    
    projectLoaded->addTransition(m_quit, SIGNAL(triggered()), quit);
    projectLoaded->addTransition(m_projectManager, SIGNAL(projectOpened(QModelIndex)), projectLoaded);
    
    
    
    /*/ Entry and exit configuration.
    connect(noProjectLoaded, &QState::entered, [=]()
    {
        qDebug() << "Enter in noProjectLoaded state.";
    });
    connect(noProjectLoaded, &QState::exited, [=]()
    {
        qDebug() << "Exit noProjectLoaded state.";
    });
    connect(projectLoaded, &QState::entered, [=]()
    {
        qDebug() << "Enter in projectLoaded state.";
    });
    connect(projectLoaded, &QState::exited, [=]()
    {
        qDebug() << "Exit projectLoaded state.";
    });
    connect(quit, &QState::entered, [=]()
    {
        qDebug() << "Enter in quit state.";
    });//*/
    
    connect(quit, &QState::entered, this, &MainController::quit);
}





MainController::~MainController()
{
    delete m_window;
}





void MainController::start()
{
    QStateMachine::start();
    
    m_config->load(m_projectManager);
    m_window->show();
}





void MainController::displayProjectImportator()
{
    ProjectImportator dialog(m_window);
    if (dialog.exec() == QDialog::Accepted)
    {
        m_projectManager->add(dialog.getProjectName(), dialog.getProjectRootPath());
    }
}





void MainController::quit()
{
    /* NOTE: Do here whatever needs to be done before quitting the application (e.g. save config)
     */
    m_config->save(m_projectManager);
    
    qApp->quit();
}
