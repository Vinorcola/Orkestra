#include "Project.hpp"

#include <QMessageBox>





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





bool Project::closeFile(const QModelIndex& fileIndex,
                        FileWidget*& widget)
{
    File* file(getFile(fileIndex));
    if (file)
    {
        if (file->doesNeedToBeSaved())
        {
            QMessageBox dialog;
            dialog.setText(tr("The file has been modified."));
            dialog.setInformativeText(tr("Do you want to save the changes?"));
            dialog.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
            dialog.setDefaultButton(QMessageBox::Save);
            int result(dialog.exec());
            
            if (result == QMessageBox::Save || result == QMessageBox::Discard)
            {
                /* NOTE: Deleting the file object does not destruct the widget. The widget will be deleted by the editor
                 * widget.
                 */
                beginRemoveRows(QModelIndex(), fileIndex.row(), fileIndex.row());
                File* file(m_files.takeAt(fileIndex.row()));
                endRemoveRows();
                
                if (result == QMessageBox::Save)
                {
                    file->saveContent();
                    /** TODO: When error management will be implemented, if the file save failed, just return false
                     * here.
                     */
                }
                widget = file->getWidget();
                delete file;
                
                return true;
            }
        }
        else
        {
            /* NOTE: Deleting the file object does not destruct the widget. The widget will be deleted by the editor
             * widget.
             */
            beginRemoveRows(QModelIndex(), fileIndex.row(), fileIndex.row());
            File* file(m_files.takeAt(fileIndex.row()));
            endRemoveRows();
            
            widget = file->getWidget();
            delete file;
            
            return true;
        }
    }
    
    return false;
}





bool Project::prepareClose()
{
    QList<File*> fileToSave;
    for (int i(0), iEnd(m_files.size()); i < iEnd; ++i)
    {
        if (m_files.at(i)->doesNeedToBeSaved())
        {
            fileToSave.append(m_files.at(i));
        }
    }
    
    bool saveAll(false);
    while (!fileToSave.isEmpty())
    {
        if (saveAll)
        {
            fileToSave.last()->saveContent();
        }
        else
        {
            QMessageBox dialog;
            dialog.setText(tr("The file \"%1\" has been modified.").arg(fileToSave.last()->getName()));
            dialog.setInformativeText(tr("Do you want to save the changes?"));
            if (fileToSave.size() > 1)
            {
                dialog.setStandardButtons(QMessageBox::SaveAll | QMessageBox::Save | QMessageBox::NoToAll | QMessageBox::Discard | QMessageBox::Cancel);
                dialog.setDefaultButton(QMessageBox::SaveAll);
            }
            else
            {
                dialog.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
                dialog.setDefaultButton(QMessageBox::Save);
            }
            
            switch (dialog.exec())
            {
                case QMessageBox::Cancel:
                    return false;
                    
                case QMessageBox::NoToAll:
                    return true;
                    
                case QMessageBox::SaveAll:
                    saveAll = true;
                    // No break here!
                    
                case QMessageBox::Save:
                    fileToSave.last()->saveContent();
            }
        }
        
        fileToSave.takeLast();
    }
    
    return true;
}





void Project::deleteAllFileWidgets()
{
    for (int i(0), iEnd(m_files.size()); i < iEnd; ++i)
    {
        delete m_files.at(i)->getWidget();
    }
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





void Project::openFile(const QString& filePath)
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
            return m_files.at(index.row())->getDisplayableName();
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
    
    connect(file, &File::displayableNameChanged, [=]()
    {
        emit dataChanged(index(insertRow), index(insertRow), { Qt::DisplayRole });
    });
    
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
