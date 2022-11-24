#include "input.h"

Input::Input(QWidget *parent): QTextEdit(parent),menu(new QMenu(this))
{
    setStyleSheet(
         "QTextEdit{ border:0;}\n"
         "QTextEdit:hover{ border:1px solid gray;}\n"
         "QTextEdit:pressed{ border:1px solid gray;}");
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this,&QTextEdit::customContextMenuRequested, [=]() { menu->exec(QCursor::pos()); });
}

void Input::append(const QByteArray &bytes)
{
    moveCursor(QTextCursor::End);
    insertPlainText(QString::fromLocal8Bit(bytes));
}

void Input::append(const QString &text)
{
    moveCursor(QTextCursor::End);
    insertPlainText(text);
}

void Input::saveAsTXT() {
    const QString& text = toPlainText();
    if (!text.isEmpty()) savefile(".txt", [&](QTextStream& s) { s << text; });
}

void Input::saveAsCSV() {
    const QString& text = toPlainText();
    if (!text.isEmpty()) savefile(".csv", [&](QTextStream& s) { s << text; });
}
