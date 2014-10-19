#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <QFile>
#include <QHash>
#include <Qsci/qscilexer.h>

#include "config/ConfigurableInterface.hpp"
#include "config/global.hpp"

class ProjectManager;





class Config : public QObject
{
        Q_OBJECT
        
        
        
    private:
        QString m_directory;
        QFile m_file;
        QHash<FileFormat::Enum, QsciLexer*> m_lexerList;
        
        
        
    public:
        Config(QObject* parent);
        
        
        
        QsciLexer* getLexer(const FileFormat::Enum format) const;
        
        
        
        void save(const ConfigurableInterface* configurable);
        
        
        
        void load(ConfigurableInterface* configurable);
};

#endif // CONFIG_HPP
