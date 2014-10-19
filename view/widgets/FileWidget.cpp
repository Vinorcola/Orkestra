#include "FileWidget.hpp"

#include <QFile>





FileWidget::FileWidget(const QString& path,
                       QsciLexer* lexer) :
    QsciScintilla()
{
    setUtf8(true);
    
    setLexer(lexer);
    setMarginLineNumbers(1, true);
    setMarginWidth(1, "--------");
    setAutoIndent(true);
    
    QFile file(path);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        setText(file.readAll());
    }
}
