#ifndef QDOCKPOPUP_H
#define QDOCKPOPUP_H

#include <QDialog>
#include <QEvent>
#include <QGridLayout>
#include <QKeyEvent>

class QDockPopup : public QDialog {
    Q_OBJECT
public:
    explicit QDockPopup(QWidget* content, QWidget* parent = nullptr);

    QWidget* const m_content;

protected:
    bool event(QEvent* event) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent* event) Q_DECL_OVERRIDE;

signals:
    void dragRelease(const QPoint& pos);
    void closePopup(QWidget*);
};

#endif  // QDOCKPOPUP_H
