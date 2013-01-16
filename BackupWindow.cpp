/*
 * 2013 Alexander von Gluck IV, All rights reserved
 * Released under the terms of the MIT license
 *
 */


#include "BackupWindow.h"

#include <Application.h>
#include <GroupLayout.h>


BackupWindow::BackupWindow(BRect frame)
	:
	BWindow(frame, "Backup", B_TITLED_WINDOW_LOOK,
		B_NORMAL_WINDOW_FEEL, B_NOT_RESIZABLE | B_NOT_ZOOMABLE
		| B_ASYNCHRONOUS_CONTROLS)
{
	SetLayout(new BGroupLayout(B_HORIZONTAL));

	fBackupView = new BackupView(frame);
	GetLayout()->AddView(fBackupView);

	Show();
}


bool
BackupWindow::QuitRequested()
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}
