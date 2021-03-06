#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <QFile>
#include <QHash>
#include <Qsci/qscilexer.h>

#include "config/ConfigurableInterface.hpp"
#include "config/global.hpp"
#include "model/config/ComposerConfig.hpp"
#include "model/config/PHPConfig.hpp"

class ProjectManager;





class Config : public QObject
{
        Q_OBJECT
        
        
        
    private:
        QString m_directory;
        QFile m_file;
        QHash<FileFormat::Enum, QsciLexer*> m_lexerList;
        PHPConfig m_phpConfig;
        ComposerConfig m_composerConfig;
        
        
        
    public:
        explicit Config(QObject* parent);
        
        
        
        QsciLexer* getLexer(const FileFormat::Enum format) const;
        
        
        
        PHPConfig& getPHPConfig();
        
        
        
        ComposerConfig& getComposerConfig();
        
        
        
        void save(const ProjectManager* projectManager);
        
        
        
        void load(ProjectManager* projectManager);
};

#endif // CONFIG_HPP
