#ifndef FILEWIDGET_HPP
#define FILEWIDGET_HPP

#include <Qsci/qscilexer.h>
#include <Qsci/qsciscintilla.h>





/**
 * @brief The FileWidget class is an editor widget that allow the user to view and edit the content of a file.
 * 
 * An instance of FileWidget is created for each file opened by the user. This widget is instanciated by the File class.
 */
class FileWidget : public QsciScintilla
{
        Q_OBJECT
        
        
        
    public:
        /**
         * @brief Constructs a new editor widget for a file.
         * 
         * @param path The path of the file to open.
         * @param lexer The lexer used for syntax highlighting.
         */
        FileWidget(const QString& path,
                   QsciLexer* lexer);
        
        
        
    signals:
        /**
         * @brief Indicates that the user ask to save the file.
         */
        void saveRequiered();
        
        
        
    public slots:
        /**
         * @brief Requieres the file to be saved.
         */
        void save();
};

#endif // FILEWIDGET_HPP
