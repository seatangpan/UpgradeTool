/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QLabel *label;
    QComboBox *cbx_baudrate;
    QLabel *label_2;
    QComboBox *cbx_serialnum;
    QPushButton *btn_connect;
    QLabel *label_3;
    QLineEdit *ledit_updatefile;
    QPushButton *btn_choice;
    QLabel *label_4;
    QProgressBar *progressBar;
    QPushButton *btn_upgrade;
    QTextEdit *textEdit;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(800, 600);
        QFont font;
        font.setPointSize(14);
        Widget->setFont(font);
        gridLayout_2 = new QGridLayout(Widget);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        gridLayout->setContentsMargins(6, 6, 6, 6);
        label = new QLabel(Widget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        cbx_baudrate = new QComboBox(Widget);
        cbx_baudrate->setObjectName(QString::fromUtf8("cbx_baudrate"));
        cbx_baudrate->setMinimumSize(QSize(0, 30));

        gridLayout->addWidget(cbx_baudrate, 0, 1, 1, 1);

        label_2 = new QLabel(Widget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        cbx_serialnum = new QComboBox(Widget);
        cbx_serialnum->setObjectName(QString::fromUtf8("cbx_serialnum"));
        cbx_serialnum->setMinimumSize(QSize(0, 30));

        gridLayout->addWidget(cbx_serialnum, 1, 1, 1, 1);

        btn_connect = new QPushButton(Widget);
        btn_connect->setObjectName(QString::fromUtf8("btn_connect"));
        btn_connect->setMinimumSize(QSize(0, 30));

        gridLayout->addWidget(btn_connect, 1, 2, 1, 1);

        label_3 = new QLabel(Widget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        ledit_updatefile = new QLineEdit(Widget);
        ledit_updatefile->setObjectName(QString::fromUtf8("ledit_updatefile"));
        ledit_updatefile->setMinimumSize(QSize(0, 30));
        ledit_updatefile->setClearButtonEnabled(true);

        gridLayout->addWidget(ledit_updatefile, 2, 1, 1, 1);

        btn_choice = new QPushButton(Widget);
        btn_choice->setObjectName(QString::fromUtf8("btn_choice"));
        btn_choice->setMinimumSize(QSize(0, 30));

        gridLayout->addWidget(btn_choice, 2, 2, 1, 1);

        label_4 = new QLabel(Widget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 3, 0, 1, 1);

        progressBar = new QProgressBar(Widget);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setMinimumSize(QSize(0, 30));
        progressBar->setValue(24);

        gridLayout->addWidget(progressBar, 3, 1, 1, 1);

        btn_upgrade = new QPushButton(Widget);
        btn_upgrade->setObjectName(QString::fromUtf8("btn_upgrade"));
        btn_upgrade->setMinimumSize(QSize(0, 30));

        gridLayout->addWidget(btn_upgrade, 3, 2, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);

        textEdit = new QTextEdit(Widget);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
        font1.setPointSize(14);
        textEdit->setFont(font1);

        gridLayout_2->addWidget(textEdit, 1, 0, 1, 1);


        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "\346\261\237\350\213\217\346\265\267\345\271\263\351\235\242\345\215\207\347\272\247\345\267\245\345\205\267", nullptr));
        label->setText(QCoreApplication::translate("Widget", "\346\263\242\347\211\271\347\216\207", nullptr));
        label_2->setText(QCoreApplication::translate("Widget", "\344\270\262\345\217\243\345\217\267", nullptr));
        btn_connect->setText(QCoreApplication::translate("Widget", "\350\277\236\346\216\245", nullptr));
        label_3->setText(QCoreApplication::translate("Widget", "\345\215\207\347\272\247\346\226\207\344\273\266", nullptr));
        btn_choice->setText(QCoreApplication::translate("Widget", "\346\265\217\350\247\210", nullptr));
        label_4->setText(QCoreApplication::translate("Widget", "\345\215\207\347\272\247\350\277\233\345\272\246", nullptr));
        btn_upgrade->setText(QCoreApplication::translate("Widget", "\345\215\207\347\272\247", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
