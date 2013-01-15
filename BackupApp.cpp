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
