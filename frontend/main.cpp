// Copyright © Michael Winkelmann <michael@winkelmann.site>
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}
