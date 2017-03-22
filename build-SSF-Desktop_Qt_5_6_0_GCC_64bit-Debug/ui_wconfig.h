/********************************************************************************
** Form generated from reading UI file 'wconfig.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WCONFIG_H
#define UI_WCONFIG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_wConfig
{
public:

    void setupUi(QWidget *wConfig)
    {
        if (wConfig->objectName().isEmpty())
            wConfig->setObjectName(QStringLiteral("wConfig"));
        wConfig->resize(400, 300);

        retranslateUi(wConfig);

        QMetaObject::connectSlotsByName(wConfig);
    } // setupUi

    void retranslateUi(QWidget *wConfig)
    {
        wConfig->setWindowTitle(QApplication::translate("wConfig", "Form", 0));
    } // retranslateUi

};

namespace Ui {
    class wConfig: public Ui_wConfig {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WCONFIG_H
