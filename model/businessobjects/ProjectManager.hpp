#ifndef PROJECTMANAGER_HPP
#define PROJECTMANAGER_HPP

#include "model/businessobjects/Project.hpp"





/**
 * @brief The ProjectManager class manages all the known projects.
 */
class ProjectManager : public QAbstractListModel, public ConfigurableInterface
{
        Q_OBJECT
        
        
        
    private:
        QList<Project*> m_projects;/// The list of known projects.
        const Config& m_config;/// The global configuration object.
        
        
        
    public:
        ProjectManager(const Config& config,
                       QObject* parent);
        
        
        
        Project* getProject(const QModelIndex& index) const;
        
        
        
        const QList<Project*>& getProjects() const;
        
        
        
        void add(const QString& name,
                 const QString& rootPath);
        
        
        
        virtual void save(QXmlStreamWriter& outputStream) const;
        
        
        
        virtual bool load(QXmlStreamReader& inputStream,
                          const Config& config);
        
        
        
    signals:
        void projectOpened(const QModelIndex& index);
        
        
        
    public slots:
        virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
        
        
        
        virtual QVariant data(const QModelIndex& index,
                              int role) const;
        
        
        
    private:
        void add(Project* project);
};

#endif // PROJECTMANAGER_HPP
