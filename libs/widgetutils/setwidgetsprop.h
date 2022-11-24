#include <QWidget>

void setWidgetsEnable(bool enable,QWidget* widget,...);
void setWidgetsVisible(bool visible,QWidget* widget,...);
// usage: setWidgetsVisible(checked, ui->label,ui->combobox, Q_NULLPTR);
