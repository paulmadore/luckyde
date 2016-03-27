/********************************************************************************
** Form generated from reading UI file 'grepshortcutkeydlg.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GREPSHORTCUTKEYDLG_H
#define UI_GREPSHORTCUTKEYDLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GrepShortcutKeyDlg
{
public:
    QHBoxLayout *hboxLayout;
    QLineEdit *shortcutPreview;
    QPushButton *pushButton;

    void setupUi(QWidget *GrepShortcutKeyDlg)
    {
        if (GrepShortcutKeyDlg->objectName().isEmpty())
            GrepShortcutKeyDlg->setObjectName(QString::fromUtf8("GrepShortcutKeyDlg"));
        GrepShortcutKeyDlg->resize(255, 50);
        hboxLayout = new QHBoxLayout(GrepShortcutKeyDlg);
#ifndef Q_OS_MAC
        hboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        hboxLayout->setContentsMargins(9, 9, 9, 9);
#endif
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        shortcutPreview = new QLineEdit(GrepShortcutKeyDlg);
        shortcutPreview->setObjectName(QString::fromUtf8("shortcutPreview"));
        shortcutPreview->setFocusPolicy(Qt::NoFocus);
        shortcutPreview->setAlignment(Qt::AlignHCenter);
        shortcutPreview->setReadOnly(true);

        hboxLayout->addWidget(shortcutPreview);

        pushButton = new QPushButton(GrepShortcutKeyDlg);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setFocusPolicy(Qt::NoFocus);

        hboxLayout->addWidget(pushButton);


        retranslateUi(GrepShortcutKeyDlg);
        QObject::connect(pushButton, SIGNAL(clicked()), GrepShortcutKeyDlg, SLOT(close()));

        QMetaObject::connectSlotsByName(GrepShortcutKeyDlg);
    } // setupUi

    void retranslateUi(QWidget *GrepShortcutKeyDlg)
    {
        pushButton->setText(QApplication::translate("GrepShortcutKeyDlg", "Cancel", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(GrepShortcutKeyDlg);
    } // retranslateUi

};

namespace Ui {
    class GrepShortcutKeyDlg: public Ui_GrepShortcutKeyDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GREPSHORTCUTKEYDLG_H
