/*
 * 2013 Alexander von Gluck IV, All rights reserved
 * Released under the terms of the MIT license
 *
 */


#include "BackupApp.h"


BackupApp::BackupApp()
	:
	BApplication(APPLICATION_SIG)
{
	BRect windowRect(50, 50, 300, 200);
	fMainWindow = new BackupWindow(windowRect);
}


BackupApp::~BackupApp()
{
}


int
main()
{
	BackupApp app;
	app.Run();
	return 0;
}
