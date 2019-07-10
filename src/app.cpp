/*This file is part of OpenDXMC.

OpenDXMC is free software : you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenDXMC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with OpenDXMC. If not, see < https://www.gnu.org/licenses/>.

Copyright 2019 Erlend Andersen
*/

#include <QApplication>
#include <QSurfaceFormat>

#include "mainwindow.h"



int main (int argc, char *argv[])
{
	
	QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
	QApplication app(argc, argv);
	QCoreApplication::setApplicationName("CRevision");
	QCoreApplication::setOrganizationName("SSHF");
	app.setWindowIcon(QIcon("resources/icons/icon.png"));

	MainWindow win;
	win.show();

    return app.exec();
}
