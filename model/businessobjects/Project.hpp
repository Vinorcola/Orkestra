#ifndef PROJECT_HPP
#define PROJECT_HPP

#include <QAbstractListModel>

#include "model/businessobjects/File.hpp"





/**
 * @brief The Project class manages a project.
 */
class Project : public QAbstractListModel, public ConfigurableInterface
{
        Q_OBJECT
        
        
        
    private:
        QString m_name;/// The project name.
        QString m_rootPath;/// The project root path.
        const Config& m_config;/// The global configuration object.
        QList<File*> m_files;/// The list of the opened files in the project.
        
        
        
    public:
        /**
         * @brief Constructs a new project.
         * 
         * In order to construct a new project, you need to give a name and a root path. You also need to pass the
         * global configuration to get the configured lexer when opening a file in the project.
         * 
         * @param name The project name.
         * @param rootPath The project root path.
         * @param config The global configuration object.
         * @param parent The parent object.
         */
        Project(const QString& name,
                const QString& rootPath,
                const Config& config,
                QObject* parent);
        
        
        
        /**
         * @brief Constructs a new project in order to load it.
         * 
         * @param config The global configuration object.
         * @param parent The parent object.
         */
        Project(const Config& config,
                QObject* parent);
        
        
        
        /**
         * @brief Returns the project name.
         * 
         * @return The project name.
         */
        const QString& getName() const;
        
        
        
        /**
         * @brief Returns the project root path.
         * 
         * @return The project root path.
         */
        const QString& getRootPath() const;
        
        
        
        /**
         * @brief Indicates if a file is open.
         * 
         * @param filePath The path to the file.
         * @return @b @c true if the file is already opened, @b @c false otherwise.
         */
        bool isOpened(const QString& filePath) const;
        
        
        
        /**
         * @brief Returns the opened file located at the index @a index in the model.
         * 
         * @param index The index of the file in the model.
         * @return An opened file if @a index is valid, 0 otherwise.
         */
        File* getFile(const QModelIndex& index) const;
        
        
        
        /**
         * @brief Returns the list of opened files.
         * 
         * @return The list of opened files.
         */
        const QList<File*>& getFiles() const;
        
        
        
        /**
         * @brief Destructs the file if it belongs to the project.
         * 
         * If the file needs to be saved, a dialog will ask the user what to do.
         * 
         * @param fileIndex The index of the file in the project.
         * @param widget The widget of the file to delete.
         * @return @b @c true if the file was deleted, @b @c false otherwise.
         */
        bool close(const QModelIndex& fileIndex,
                   FileWidget*& widget);
        
        
        
        /**
         * @brief Saves the details of the project into the output configuration stream @a outputStream.
         * 
         * @param outputStream The output configuration stream.
         */
        virtual void save(QXmlStreamWriter& outputStream) const;
        
        
        
        /**
         * @brief Loads the details of the project from the input configuration stream @a inputStream.
         * 
         * The project object @b must have been construct with the Project(QObject*) constructor.
         * 
         * @param inputStream The input configuration stream.
         * @param config The global configuration.
         * @return @b @c true if the load succed, @b @c false otherwise.
         */
        virtual bool load(QXmlStreamReader& inputStream,
                          const Config& config);
        
        
        
    signals:
        /**
         * @brief Signal emited when a file is opened in the project.
         * 
         * @param index
         */
        void fileOpened(const QModelIndex& index);
        
        
        
    public slots:
        /**
         * @brief Open a new file in the project.
         * 
         * @param filePath The path to the file to open.
         */
        void open(const QString& filePath);
        
        
        
        virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
        
        
        
        virtual QVariant data(const QModelIndex& index,
                              int role) const;
        
        
        
    private:
        void add(File* file);
        
        
        
        QModelIndex getIndexOfFileByPath(const QString& filePath) const;
};

#endif // PROJECT_HPP
