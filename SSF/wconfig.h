#ifndef WCONFIG_H
#define WCONFIG_H

#include <QWidget>

namespace Ui {
class wConfig;
}

class wConfig : public QWidget
{
    Q_OBJECT

public:
    explicit wConfig(QWidget *parent = 0);
    ~wConfig();

private:
    Ui::wConfig *ui;
};

#endif // WCONFIG_H
