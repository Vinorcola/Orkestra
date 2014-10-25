#include "FileWidget.hpp"

#include <QFile>





FileWidget::FileWidget(const QString& path,
                       QsciLexer* lexer) :
    QsciScintilla()
{
    QColor gris(192, 192, 192);
    
    setUtf8(true);
    
    setLexer(lexer);
    setMarginLineNumbers(1, true);
    setMarginWidth(1, "--------");
    
    setTabWidth(4);
    setAutoIndent(true);
    setIndentationGuides(true);
    setIndentationGuidesForegroundColor(gris);
    setIndentationsUseTabs(false);
    
    setWhitespaceVisibility(QsciScintilla::WsVisible);
    setWhitespaceForegroundColor(gris);
    
    setEdgeColumn(120);
    setEdgeMode(QsciScintilla::EdgeLine);
    
    QFile file(path);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        setText(file.readAll());
    }
}





void FileWidget::save()
{
    emit saveRequiered();
}
