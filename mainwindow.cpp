/**********************************************************************
 *  mainwindow.cpp
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

#include <QDebug>
#include <QFileInfo>
#include <QTextEdit>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "version.h"


MainWindow::MainWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainWindow)
{
    qDebug().noquote() << QCoreApplication::applicationName() << "version:" << VERSION;

    ui->setupUi(this);
    setWindowFlags(Qt::Window); // for the close, min and max buttons
    setup();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// setup versious items first time program runs
void MainWindow::setup()
{
    this->setWindowTitle(tr("MX Remaster Control Center"));
    this->adjustSize();
    ui->buttonSetupPersistence->setStyleSheet("text-align:left;");
    ui->buttonConfigPersistence ->setStyleSheet("text-align:left;");
    ui->buttonSaveRootPersist->setStyleSheet("text-align:left;");
    ui->buttonRemaster->setStyleSheet("text-align:left;");
    ui->buttonSaveRootPersist->setIcon(QIcon::fromTheme("filesave", QIcon(":/icons/filesave.svg")));
}

// Util function for getting bash command output and error code
Result MainWindow::runCmd(QString cmd)
{
    QEventLoop loop;
    QProcess *proc = new QProcess(this);
    proc->setReadChannelMode(QProcess::MergedChannels);
    connect(proc, static_cast<void (QProcess::*)(int)>(&QProcess::finished), &loop, &QEventLoop::quit);
    proc->start("/bin/bash", QStringList() << "-c" << cmd);
    loop.exec();
    disconnect(proc, static_cast<void (QProcess::*)(int)>(&QProcess::finished), nullptr, nullptr);
    Result result = {proc->exitCode(), proc->readAll().trimmed()};
    delete proc;
    return result;
}

void MainWindow::displayDoc(QString url)
{
    QString exec = "xdg-open";
    if (system("command -v mx-viewer") == 0) // use mx-viewer if available
        exec = "mx-viewer";
    QString cmd = exec + " " + url + "&";
    system(cmd.toUtf8());
}

// About button clicked
void MainWindow::on_buttonAbout_clicked()
{
    this->hide();
    QMessageBox msgBox(QMessageBox::NoIcon,
                       tr("About MX Remaster Control Center"), "<p align=\"center\"><b><h2>" +
                       tr("MX Remaster Control Center") + "</h2></b></p><p align=\"center\">" + tr("Version: ") + VERSION + "</p><p align=\"center\"><h3>" +
                       tr("This program provides access to different remaster and persistence tools in MX Linux") +
                       "</h3></p><p align=\"center\"><a href=\"http://mxlinux.org\">http://mxlinux.org</a><br /></p><p align=\"center\">" +
                       tr("Copyright (c) MX Linux") + "<br /><br /></p>", nullptr, this);
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
void MainWindow::on_buttonHelp_clicked()
{
    QLocale locale;
    QString lang = locale.bcp47Name();

    QString url = "/usr/share/doc/mx-remastercc/mx-remastercc.html";

    if (lang.startsWith("fr"))
        url = "https://mxlinux.org/wiki/help-files/help-mx-r%C3%A9masterisation";
    displayDoc(url);
}

void MainWindow::on_buttonSetupPersistence_clicked()
{
    this->hide();
    system("su-to-root -X -c 'env GTK_THEME=Adwaita persist-makefs'");
    this->show();
}

void MainWindow::on_buttonConfigPersistence_clicked()
{
    this->hide();
    system("su-to-root -X -c 'env GTK_THEME=Adwaita persist-config'");
    this->show();
}

void MainWindow::on_buttonSaveRootPersist_clicked()
{
    this->hide();
    system("su-to-root -X -c 'env GTK_THEME=Adwaita persist-save'");
    this->show();
}

void MainWindow::on_buttonRemaster_clicked()
{
    this->hide();
    system("su-to-root -X -c 'env GTK_THEME=Adwaita live-remaster'");
    this->show();
}
