#include "MainController.hpp"

#include <QApplication>
#include <QFileDialog>
#include <QFinalState>
#include <QMenu>
#include <QMenuBar>

#include "controller/ProjectLoadedState.hpp"
#include "view/widgets/ProjectImportator.hpp"





#include <QDebug>
MainController::MainController() :
    QStateMachine(),
    m_config(new Config(this)),
    m_projectManager(new ProjectManager(*m_config, this)),
    m_quit(new QAction(tr("Quit"), this)),
    m_importProject(new QAction(tr("Import a project"), this)),
    m_saveFile(new QAction(tr("Save"), this)),
    m_saveAllFiles(new QAction(tr("Save all"), this)),
    m_editorWidget(new EditorWidget),
    m_projectFileDock(new ProjectFileDock(m_projectManager)),
    m_openedFileDock(new OpenedFileDock),
    m_window(new MainWindow(m_quit, m_editorWidget, m_projectFileDock, m_openedFileDock))
{
    // Setup widgets.
    connect(m_openedFileDock, &OpenedFileDock::fileChanged, m_editorWidget, &EditorWidget::setCurrentFile);
    
    connect(m_saveFile, &QAction::triggered, m_editorWidget, &EditorWidget::saveCurrentFile);
    connect(m_saveAllFiles, &QAction::triggered, m_editorWidget, &EditorWidget::saveAllFiles);
    
    
    
    // Setup main window menus.
    QMenu* menuApplication(m_window->menuBar()->addMenu(tr("Orkestra")));
    menuApplication->addAction(m_importProject);
    menuApplication->addSeparator();
    menuApplication->addAction(m_saveFile);
    menuApplication->addAction(m_saveAllFiles);
    menuApplication->addSeparator();
    menuApplication->addAction(m_quit);
    
    
    
    // Setup actions.
    m_quit->setShortcut(Qt::CTRL + Qt::Key_Q);
    m_importProject->setShortcut(Qt::CTRL + Qt::Key_I);
    m_saveFile->setShortcut(Qt::CTRL + Qt::Key_S);
    m_saveAllFiles->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_S);
    
    connect(m_importProject, &QAction::triggered, this, &MainController::displayProjectImportator);
    
    
    
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
    
    connect(noProjectLoaded, &QState::entered, this, &MainController::init);
    connect(quit, &QState::entered, this, &MainController::quit);
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
        m_projectManager->add(dialog.getProjectName(), dialog.getProjectRootPath());
        /* NOTE: We do nothing more here. The project manager object will notify the project file dock that a new
         * project was added. The dock will update the current project and trigger the projectChanged() signal. This
         * signal will push the controller in the projectLoaded state. All the work will be done on project loaded state
         * entry.
         */
    }
}





void MainController::quit()
{
    /* NOTE: Do here whatever needs to be done before quitting the application (e.g. save config)
     */
    m_config->save(m_projectManager);
    
    qApp->quit();
}
