/*
 * 2013 Alexander von Gluck IV, All rights reserved
 * Released under the terms of the MIT license
 *
 * Authors:
 *  Alexander von Gluck IV <kallisti5@unixzen.com>
 */
#ifndef __BACKUP_MANAGER_H
#define __BACKUP_MANAGER_H


#include "BackupView.h"


class BackupManager {
public:
							BackupManager(uint32 taskMask);
							~BackupManager();
private:
			uint32			fTaskMask;
};


#endif /* __BACKUP_MANAGER_H */
