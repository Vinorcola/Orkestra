#-------------------------------------------------
#
# Project created by QtCreator 2014-10-12T13:46:10
#
#-------------------------------------------------

QT += core gui widgets


TARGET = Orkestra1


TEMPLATE = app


CONFIG += c++11 qscintilla2


SOURCES += main.cpp\
    config/Config.cpp \
    controller/MainController.cpp \
    model/businessobjects/File.cpp \
    model/businessobjects/Project.cpp \
    model/businessobjects/ProjectManager.cpp \
    view/docks/OpenedFileDock.cpp \
    view/docks/ProjectFileDock.cpp \
    view/widgets/EditorWidget.cpp \
    view/widgets/FileSelectorWidget.cpp \
    view/widgets/FileWidget.cpp \
    view/widgets/MainWindow.cpp \
    view/widgets/ProjectImportator.cpp \
    controller/ProjectLoadedState.cpp


HEADERS += config/global.hpp \
    config/Config.hpp \
    config/ConfigurableInterface.hpp \
    controller/MainController.hpp \
    model/businessobjects/File.hpp \
    model/businessobjects/Project.hpp \
    model/businessobjects/ProjectManager.hpp \
    view/docks/OpenedFileDock.hpp \
    view/docks/ProjectFileDock.hpp \
    view/widgets/EditorWidget.hpp \
    view/widgets/FileSelectorWidget.hpp \
    view/widgets/FileWidget.hpp \
    view/widgets/MainWindow.hpp \
    view/widgets/ProjectImportator.hpp \
    controller/ProjectLoadedState.hpp
