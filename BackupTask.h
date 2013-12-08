/*
 * 2013 Alexander von Gluck IV, All rights reserved
 * Released under the terms of the MIT license
 *
 * Authors:
 *  Alexander von Gluck IV <kallisti5@unixzen.com>
 */
#ifndef __BACKUP_TASK_H
#define __BACKUP_TASK_H


#include <FilePanel.h>

#include "BackupView.h"


class BackupTask {
public:
							BackupTask(uint32 taskMask);
							~BackupTask();

	virtual	void			MessageReceived(BMessage *msg);

private:
			void			DestinationDialog();
			uint32			fTaskMask;
};


#endif /* __BACKUP_TASK_H */
