/**********************************************************************
 *  mxremastercc.h
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


#ifndef MXREMASTERCC_H
#define MXREMASTERCC_H

#include <QMessageBox>
#include <QProcess>

namespace Ui {
class mxremastercc;
}

struct Result {
    int exitCode;
    QString output;
};


class mxremastercc : public QDialog
{
    Q_OBJECT

public:
    explicit mxremastercc(QWidget *parent = 0);
    ~mxremastercc();

    Result runCmd(QString cmd);
    QString getVersion(QString name);

    QString version;
    QString output;

    void setup();    

private slots:
    void on_buttonAbout_clicked();
    void on_buttonHelp_clicked();    
    void on_buttonSetupPersistence_clicked();
    void on_buttonConfigPersistence_clicked();
    void on_buttonSaveRootPersist_clicked();
    void on_buttonRemaster_clicked();

private:
    Ui::mxremastercc *ui;
};


#endif // MXSNAPSHOT_H

