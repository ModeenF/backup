/*
 * 2013 Alexander von Gluck IV, All rights reserved
 * Released under the terms of the MIT license
 *
 */


#include "BackupWindow.h"


BackupWindow::BackupWindow(BRect frame)
	:
	BWindow(frame, "Backup", B_TITLED_WINDOW_LOOK,
		B_NORMAL_WINDOW_FEEL, B_NOT_RESIZABLE | B_NOT_ZOOMABLE
		| B_ASYNCHRONOUS_CONTROLS)
{
	Show();
}


bool
BackupWindow::QuitRequested()
{
	//be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}
