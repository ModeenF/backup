/*
 * 2013 Alexander von Gluck IV, All rights reserved
 * Released under the terms of the MIT license
 *
 * Authors:
 *  Alexander von Gluck IV <kallisti5@unixzen.com>
 */


#include "BackupTask.h"

#include "BackupLocations.h"


extern struct location_map gLocationMap[LOCATION_COUNT];


BackupTask::BackupTask(uint32 taskMask)
	:
	fTaskMask(taskMask)
{
	// Set destination directory
	DestinationDialog();

}


BackupTask::~BackupTask()
{

}


void
BackupTask::DestinationDialog()
{
	BFilePanel* filePanel = new BFilePanel(B_SAVE_PANEL);
	filePanel->Show();
}


void
BackupTask::MessageReceived(BMessage *msg)
{

}
