/**********************************************************************
 *  mxremastercc.cpp
 **********************************************************************
 * Copyright (C) 2015 MX Authors
 *
 * Authors: Adrian
 *          MX Linux <http://mxlinux.org>
 *
 * This file is part of mx-remastercc.
 *
 * mx-remastercc is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * mx-remastercc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with mx-remastercc.  If not, see <http://www.gnu.org/licenses/>.
 **********************************************************************/


#include "mxremastercc.h"
#include "ui_mxremastercc.h"

#include <QTextEdit>
#include <QFileInfo>

//#include <QDebug>

mxremastercc::mxremastercc(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::mxremastercc)
{
    ui->setupUi(this);
    setup();
}

mxremastercc::~mxremastercc()
{
    delete ui;
}

// setup versious items first time program runs
void mxremastercc::setup()
{
    version = getVersion("mx-remastercc");
    this->setWindowTitle(tr("MX Remaster Control Center"));
    this->adjustSize();
    ui->buttonSetupPersistence->setStyleSheet("text-align:left;");
    ui->buttonConfigPersistence ->setStyleSheet("text-align:left;");
    ui->buttonSaveRootPersist->setStyleSheet("text-align:left;");
    ui->buttonRemaster->setStyleSheet("text-align:left;");
}

// Util function for getting bash command output and error code
Result mxremastercc::runCmd(QString cmd)
{
    QEventLoop loop;
    QProcess *proc = new QProcess(this);
    proc->setReadChannelMode(QProcess::MergedChannels);
    connect(proc, SIGNAL(finished(int)), &loop, SLOT(quit()));
    proc->start("/bin/bash", QStringList() << "-c" << cmd);
    loop.exec();
    disconnect(proc, SIGNAL(finished(int)), 0, 0);
    Result result = {proc->exitCode(), proc->readAll().trimmed()};
    delete proc;
    return result;
}

// Get version of the program
QString mxremastercc::getVersion(QString name)
{
    return runCmd("dpkg-query -f '${Version}' -W " + name).output;
}

void mxremastercc::displayDoc(QString url)
{
    QString exec = "xdg-open";
    if (system("command -v mx-viewer") == 0) { // use mx-viewer if available
        exec = "mx-viewer";
    }
    QString cmd = exec + " " + url + "&";
    system(cmd.toUtf8());
}

//// slots ////


// About button clicked
void mxremastercc::on_buttonAbout_clicked()
{
    this->hide();
    QMessageBox msgBox(QMessageBox::NoIcon,
                       tr("About MX Remaster Control Center"), "<p align=\"center\"><b><h2>" +
                       tr("MX Remaster Control Center") + "</h2></b></p><p align=\"center\">" + tr("Version: ") + version + "</p><p align=\"center\"><h3>" +
                       tr("This program provides access to different remaster and persistence tools in MX Linux") +
                       "</h3></p><p align=\"center\"><a href=\"http://mxlinux.org\">http://mxlinux.org</a><br /></p><p align=\"center\">" +
                       tr("Copyright (c) MX Linux") + "<br /><br /></p>", 0, this);
    QPushButton *btnLicense = msgBox.addButton(tr("License"), QMessageBox::HelpRole);
    QPushButton *btnChangelog = msgBox.addButton(tr("Changelog"), QMessageBox::HelpRole);
    QPushButton *btnCancel = msgBox.addButton(tr("Cancel"), QMessageBox::NoRole);
    btnCancel->setIcon(QIcon::fromTheme("window-close"));

    msgBox.exec();

    if (msgBox.clickedButton() == btnLicense) {
        QString url ="file:///usr/share/doc/mx-remastercc/license.html";
        displayDoc(url);
    } else if (msgBox.clickedButton() == btnChangelog) {
        QDialog *changelog = new QDialog(this);
        changelog->resize(600, 500);

        QTextEdit *text = new QTextEdit;
        text->setReadOnly(true);
        text->setText(runCmd("zless /usr/share/doc/" + QFileInfo(QCoreApplication::applicationFilePath()).fileName()  + "/changelog.gz").output);

        QPushButton *btnClose = new QPushButton(tr("&Close"));
        btnClose->setIcon(QIcon::fromTheme("window-close"));
        connect(btnClose, &QPushButton::clicked, changelog, &QDialog::close);

        QVBoxLayout *layout = new QVBoxLayout;
        layout->addWidget(text);
        layout->addWidget(btnClose);
        changelog->setLayout(layout);
        changelog->exec();
    }
    this->show();
}

// Help button clicked
void mxremastercc::on_buttonHelp_clicked()
{
    QLocale locale;
    QString lang = locale.bcp47Name();

    QString url = "https://mxlinux.org/wiki/help-files/help-mx-remaster";

    if (lang.startsWith("fr")) {
        url = "https://mxlinux.org/wiki/help-files/help-mx-r%C3%A9masterisation";
    }
    displayDoc(url);
}

void mxremastercc::on_buttonSetupPersistence_clicked()
{
    this->hide();
    system("su-to-root -X -c persist-makefs");
    this->show();
}

void mxremastercc::on_buttonConfigPersistence_clicked()
{
    this->hide();
    system("su-to-root -X -c persist-config");
    this->show();
}

void mxremastercc::on_buttonSaveRootPersist_clicked()
{
    this->hide();
    system("su-to-root -X -c persist-save");
    this->show();
}

void mxremastercc::on_buttonRemaster_clicked()
{
    this->hide();
    system("su-to-root -X -c live-remaster");
    this->show();
}
