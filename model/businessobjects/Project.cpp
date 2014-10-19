#include "Project.hpp"





Project::Project(const QString& name,
                 const QString& rootPath,
                 const Config& config,
                 QObject* parent) :
    QAbstractListModel(parent),
    m_name(name),
    m_rootPath(rootPath),
    m_config(config),
    m_files()
{
    
}





Project::Project(const Config& config,
                 QObject* parent) :
    QAbstractListModel(parent),
    m_name(),
    m_rootPath(),
    m_config(config),
    m_files()
{
    
}





const QString& Project::getName() const
{
    return m_name;
}





const QString& Project::getRootPath() const
{
    return m_rootPath;
}





bool Project::isOpened(const QString& filePath) const
{
    for (int i(0), iEnd(m_files.size()); i < iEnd; ++i)
    {
        if (m_files.at(i)->getPath() == filePath)
        {
            return true;
        }
    }
    
    return false;
}





File* Project::getFile(const QModelIndex& index) const
{
    if (index.isValid() && index.row() < rowCount())
    {
        return m_files.at(index.row());
    }
    
    return 0;
}





const QList<File*>& Project::getFiles() const
{
    return m_files;
}





void Project::save(QXmlStreamWriter& outputStream) const
{
    outputStream.writeStartElement(QStringLiteral("project"));
    outputStream.writeAttribute(QStringLiteral("name"), m_name);
    outputStream.writeAttribute(QStringLiteral("rootpath"), m_rootPath);
    
    outputStream.writeStartElement(QStringLiteral("files"));
    
    for (int i(0), iEnd(m_files.size()); i < iEnd; ++i)
    {
        m_files.at(i)->save(outputStream);
    }
    
    outputStream.writeEndElement();// files
    
    outputStream.writeEndElement();// project
}





bool Project::load(QXmlStreamReader& inputStream,
                   const Config& config)
{
    if (!inputStream.atEnd() && inputStream.readNextStartElement() && inputStream.name() == "project")
    {
        QXmlStreamAttributes attributes(inputStream.attributes());
        if (attributes.hasAttribute(QStringLiteral("name")))
        {
            m_name = attributes.value(QStringLiteral("name")).toString();
        }
        else
        {
            return false;
        }
        if (attributes.hasAttribute(QStringLiteral("rootpath")))
        {
            m_rootPath = attributes.value(QStringLiteral("rootpath")).toString();
        }
        else
        {
            return false;
        }
        
        // Go to files.
        if (!inputStream.atEnd() && inputStream.readNextStartElement() && inputStream.name() == "files")
        {
            File* file(new File(this));
            while (file->load(inputStream, config))
            {
                add(file);
                file = new File(this);
            }
            // We delete the last file that was not configured.
            delete file;
        }
        
        // We go to the end of the project tag.
        inputStream.readNextStartElement();
        while (!inputStream.atEnd() && inputStream.name() != "project")
        {
            inputStream.readNextStartElement();
        }
        
        return true;
    }
    
    return false;
}





void Project::open(const QString& filePath)
{
    QFileInfo fileInfo(filePath);
    
    if (fileInfo.isFile())
    {
        if (isOpened(filePath))
        {
            /* NOTE: Here, we emit the signal fileOpened even if the file was already opened. Indeed, this signal is
             * used to set the current file in the editor.
             */
            emit fileOpened(getIndexOfFileByPath(filePath));
        }
        else
        {
            add(new File(fileInfo, m_config, this));
        }
    }
}





int Project::rowCount(const QModelIndex& /*parent*/) const
{
    return m_files.size();
}





QVariant Project::data(const QModelIndex& index,
                       int role) const
{
    if (index.isValid() && index.row() < rowCount())
    {
        if (role == Qt::DisplayRole)
        {
            return m_files.at(index.row())->getName();
        }
    }
    
    return QVariant();
}





void Project::add(File* file)
{
    int insertRow(rowCount());
    beginInsertRows(QModelIndex(), insertRow, insertRow);
    m_files.append(file);
    endInsertRows();
    
    emit fileOpened(index(insertRow));
}





QModelIndex Project::getIndexOfFileByPath(const QString& filePath) const
{
    for (int i(0), iEnd(m_files.size()); i < iEnd; ++i)
    {
        if (m_files.at(i)->getPath() == filePath)
        {
            return index(i);
        }
    }
    
    return QModelIndex();
}
