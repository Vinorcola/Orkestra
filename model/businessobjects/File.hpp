#ifndef FILE_HPP
#define FILE_HPP

#include <QFileInfo>

#include "config/Config.hpp"
#include "view/widgets/FileWidget.hpp"





/**
 * @brief The File class manages an opened file.
 * 
 * When a new file is opened, a new File object is instanciate. The object will create the editor widget. This widget
 * will never be destroyed by the File object. So it must be put inside another widget (in order to configure a parent).
 * This way, the widget will be destroyed when his parent widget is.
 */
class File : public QObject, public ConfigurableInterface
{
        Q_OBJECT
        
        
        
    private:
        QString m_path;/// The file path.
        QString m_name;/// The file name.
        FileFormat::Enum m_format;/// The file format (determined using MIME type or file extension).
        Sf2FileType::Enum m_sf2Type;/// The Sf2 type of file (controller, entity, etc.)
        FileWidget* m_widget;/// The editor widget associated to the file.
        bool m_needsToBeSaved;/// Indicates if the file needs to be saved.
        
        
        
    public:
        /**
         * @brief Constructs a new File object.
         * 
         * In order to open a file, we need an information object on the file and the global configuration to get the
         * correct lexer to use in the editor widget.
         * 
         * @param fileInfo Information on the file to open.
         * @param config The global configuration.
         * @param parent The parent object.
         */
        File(const QFileInfo& fileInfo,
             const Config& config,
             QObject* parent);
        
        
        
        /**
         * @brief Constructs a new file in order to load it
         * 
         * @param parent The parent object.
         */
        explicit File(QObject* parent);
        
        
        
        /**
         * @brief Returns the path to the file.
         * 
         * @return The path to the file.
         */
        const QString& getPath() const;
        
        
        
        /**
         * @brief Returns the name of the file.
         * 
         * The name of the file includes the extension (e.g. "test.txt", "index.html", etc.)
         * 
         * @return The name of the file.
         */
        const QString& getName() const;
        
        
        
        /**
         * @brief Returns the name of the file to display.
         * 
         * This name is prepended by a star if the file needs to be saved.
         * 
         * @return The name of the file to display.
         */
        QString getDisplayableName() const;
        
        
        
        /**
         * @brief Returns the format of the file.
         * 
         * @return The format of the file.
         */
        FileFormat::Enum getFormat() const;
        
        
        
        /**
         * @brief Returns the Sf2 type of the file.
         * 
         * @return The Sf2 type of the file.
         */
        Sf2FileType::Enum getSf2Type() const;
        
        
        
        /**
         * @brief Returns the editor widget that contains the file content.
         * 
         * @return The editor widget.
         */
        FileWidget* getWidget() const;
        
        
        
        /**
         * @brief Indicates if the file needs to be saved.
         * 
         * @return @b @c true if the file needs to be saved, @b @c false otherwise.
         */
        bool doesNeedToBeSaved() const;
        
        
        
        /**
         * @brief Saves the details of the file into the output configuration stream @a outputStream.
         * 
         * NOTE: This method does NOT save the file content, but only file details in the configuration.
         * 
         * @param outputStream The output configuration stream.
         */
        virtual void save(QXmlStreamWriter& outputStream) const;
        
        
        
        /**
         * @brief Loads the details of the file from the input configuration stream @a inputStream.
         * 
         * The file object @b must have been construct with the File(QObject*) constructor.
         * 
         * @param inputStream The input configuration stream.
         * @param config The global configuration.
         * @return @b @c true if the load succed, @b @c false otherwise.
         */
        virtual bool load(QXmlStreamReader& inputStream,
                          const Config& config);
        
        
        
    signals:
        /**
         * @brief Signal triggered when the displayable name changed.
         * 
         * Typicaly, this signal is emited when a change occurs in the content or when the fle is saved.
         * 
         * @param name The new displayable name.
         */
        void displayableNameChanged(const QString& name);
        
        
        
    public slots:
        /**
         * @brief Handle the modification change in the file.
         * 
         * NOTE: A modification change occurs when the user first change the file content or when ha cancel all the
         * changes he made before saving the file.
         * 
         * @param hasModification Indicates if there is modification is the file content.
         */
        void handleModification(bool hasModification);
        
        
        
        /**
         * @brief Saves the file content.
         */
        void saveContent();
        
        
        
    private:
        /**
         * @brief Returns the format of the file.
         * 
         * The format is guessed using the MIME type. If the MIME type is not enough, the file extension is used.
         * 
         * @return The format of the file.
         */
        FileFormat::Enum initFormat() const;
        
        
        
        /**
         * @brief Returns the Sf2 type of the file.
         * 
         * The Sf2 type is guessed using the format of the file and the path of the file.
         * 
         * @return The Sf2 type of the file.
         */
        Sf2FileType::Enum initSf2Type() const;
        
        
        
        /**
         * @brief Returns the name of the given format.
         * 
         * @return The name of the format.
         */
        QString getFormatName() const;
        
        
        
        /**
         * @brief Sets the file format.
         * 
         * @param format Format name.
         */
        void setFormatFromName(const QString& format);
        
        
        
        /**
         * @brief Returns the name of the given sf2 type.
         * 
         * @return The name of the sf2 type.
         */
        QString getSf2TypeName() const;
        
        
        
        /**
         * @brief Sets the file Sf2 type.
         * 
         * @param sf2type Sf2 type name.
         */
        void setSf2TypeFromName(const QString& sf2type);
};

#endif // FILE_HPP
