/**********************************************************************
 *  mxremastercc.cpp
 **********************************************************************
 * Copyright (C) 2015 MX Authors
 *
 * Authors: Adrian
 *          MX & MEPIS Community <http://forum.mepiscommunity.org>
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
    QString cmd = QString("dpkg -l %1 | awk 'NR==6 {print $3}'").arg(name);
    return runCmd(cmd).output;
}


//// slots ////


// About button clicked
void mxremastercc::on_buttonAbout_clicked()
{
    QMessageBox msgBox(QMessageBox::NoIcon,
                       tr("About MX Remaster Control Center"), "<p align=\"center\"><b><h2>" +
                       tr("MX Remaster Command Center") + "</h2></b></p><p align=\"center\">" + tr("Version: ") + version + "</p><p align=\"center\"><h3>" +
                       tr("This program provides access to different remaster and persistence tools in MX Linux") +
                       "</h3></p><p align=\"center\"><a href=\"http://www.mepiscommunity.org/mx\">http://www.mepiscommunity.org/mx</a><br /></p><p align=\"center\">" +
                       tr("Copyright (c) MX Linux") + "<br /><br /></p>", 0, this);
    msgBox.addButton(tr("Cancel"), QMessageBox::AcceptRole); // because we want to display the buttons in reverse order we use counter-intuitive roles.
    msgBox.addButton(tr("License"), QMessageBox::RejectRole);
    if (msgBox.exec() == QMessageBox::RejectRole) {
        system("mx-viewer file:///usr/share/doc/mx-remastercc/license.html '" + tr("MX RemasterCC").toUtf8() + " " + tr("License").toUtf8() + "'");
    }
}

// Help button clicked
void mxremastercc::on_buttonHelp_clicked()
{
    QString cmd = QString("mx-viewer http://mepiscommunity.org/wiki/help-files/help-mx-remaster '%1'").arg(tr("MX RemasterCC"));
    system(cmd.toUtf8());
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
    system("su-to-root -X -c remaster-live");
    this->show();
}
