#include "ProjectManager.hpp"





ProjectManager::ProjectManager(const Config& config,
                               QObject* parent) :
    QAbstractListModel(parent),
    m_projects(),
    m_config(config)
{
    
}





Project* ProjectManager::getProject(const QModelIndex& index) const
{
    if (index.isValid() && index.row() < rowCount())
    {
        return m_projects.at(index.row());
    }
    
    return 0;
}





const QList<Project*>& ProjectManager::getProjects() const
{
    return m_projects;
}





void ProjectManager::add(const QString& name,
                         const QString& rootPath)
{
    add(new Project(name, rootPath, m_config, this));
}





void ProjectManager::save(QXmlStreamWriter& outputStream) const
{
    outputStream.writeStartElement(QStringLiteral("projects"));
    
    for (int i(0), iEnd(m_projects.size()); i < iEnd; ++i)
    {
        m_projects.at(i)->save(outputStream);
    }
    
    outputStream.writeEndElement();// projects
}





bool ProjectManager::load(QXmlStreamReader& inputStream,
                          const Config& config)
{
    // Go to projects.
    if (!inputStream.atEnd() && inputStream.readNextStartElement() && inputStream.name() == "projects")
    {
        Project* project(new Project(config, this));
        while (project->load(inputStream, config))
        {
            add(project);
            project = new Project(config, this);
        }
        /* We delete the last project that was not configured.
         */
        delete project;
    }
    
    return true;
}





int ProjectManager::rowCount(const QModelIndex& /*parent*/) const
{
    return m_projects.size();
}





QVariant ProjectManager::data(const QModelIndex& index,
                              int role) const
{
    if (index.isValid() && index.row() < rowCount())
    {
        if (role == Qt::DisplayRole)
        {
            return m_projects.at(index.row())->getName();
        }
    }
    
    return QVariant();
}





void ProjectManager::add(Project* project)
{
    int insertRow(rowCount());
    beginInsertRows(QModelIndex(), insertRow, insertRow);
    m_projects.append(project);
    endInsertRows();
    
    emit projectOpened(index(insertRow));
}