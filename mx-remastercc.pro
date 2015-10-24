# **********************************************************************
# * Copyright (C) 2015 MX Authors
# *
# * Authors: Adrian
# *          MX & MEPIS Community <http://forum.mepiscommunity.org>
# *
# * This file is part of mx-remastercc.
# *
# * mx-remastercc is free software: you can redistribute it and/or modify
# * it under the terms of the GNU General Public License as published by
# * the Free Software Foundation, either version 3 of the License, or
# * (at your option) any later version.
# *
# * mx-remastercc is distributed in the hope that it will be useful,
# * but WITHOUT ANY WARRANTY; without even the implied warranty of
# * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# * GNU General Public License for more details.
# *
# * You should have received a copy of the GNU General Public License
# * along with mx-remastercc.  If not, see <http://www.gnu.org/licenses/>.
# **********************************************************************/

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mx-remastercc
TEMPLATE = app


SOURCES += main.cpp\
        mxremastercc.cpp

HEADERS  += mxremastercc.h

FORMS    += mxremastercc.ui

TRANSLATIONS += translations/mx-remastercc_ca.ts \
                translations/mx-remastercc_de.ts \
                translations/mx-remastercc_el.ts \
                translations/mx-remastercc_es.ts \
                translations/mx-remastercc_fr.ts \
                translations/mx-remastercc_it.ts \
                translations/mx-remastercc_ja.ts \
                translations/mx-remastercc_nl.ts \
                translations/mx-remastercc_pl.ts \
                translations/mx-remastercc_ro.ts \
                translations/mx-remastercc_ru.ts \
                translations/mx-remastercc_sv.ts


