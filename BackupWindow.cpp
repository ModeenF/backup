/*
 * 2013 Alexander von Gluck IV, All rights reserved
 * Released under the terms of the MIT license
 *
 */


#include "BackupWindow.h"

#include <Alert.h>
#include <Application.h>
#include <GroupLayout.h>
#include <stdio.h>


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


void
BackupWindow::MessageReceived(BMessage *msg)
{
	switch (msg->what) {
		case kMsgDoBackup:
			DoBackup(fBackupView->GetTasks());
			break;
		case kMsgUpdateSelection:
			fBackupView->RefreshSizes();
			break;
		default:
			BWindow::MessageReceived(msg);
	}
}


status_t
BackupWindow::DoBackup(uint32 tasks)
{
	printf("tasks: %" B_PRIx32 "\n", tasks);
	if (tasks == 0) {
		BAlert* alert = new BAlert("nothing to do alert",
			"Nothing was selected to backup.\n\n"
			"Please choose at least one item to backup before continuing.",
			"OK", NULL, NULL, B_WIDTH_AS_USUAL,
			B_OFFSET_SPACING, B_WARNING_ALERT);
		alert->Go(NULL);
		return B_ERROR;
	}

	return B_OK;
}
