#ifndef INPUT_H
#define INPUT_H

#include <QWidget>

#include <QTime>
#include <QTextEdit>

#include <QApplication>
#include <QClipboard>

#include "savefile.h"
#include "qmenuplus.h"

class Input : public QTextEdit
{

public:
    Input(QWidget* parent = Q_NULLPTR);
    ~Input(){};

    void append(const QString& text);
    void append(const QByteArray& bytes);

    void saveAsCSV();
    void saveAsTXT();

    QMenu* const menu;

    static QString timestamp(const QString& format="[hh:mm:ss]") { return QTime::currentTime().toString(format); }
};

#endif // INPUT_H
