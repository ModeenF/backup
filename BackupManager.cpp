/*
 * 2013 Alexander von Gluck IV, All rights reserved
 * Released under the terms of the MIT license
 *
 * Authors:
 *  Alexander von Gluck IV <kallisti5@unixzen.com>
 */


#include "BackupManager.h"

#include "BackupLocations.h"


extern struct location_map gLocationMap[LOCATION_COUNT];


BackupManager::BackupManager(uint32 taskMask)
	:
	fTaskMask(taskMask)
{

}


BackupManager::~BackupManager()
{

}
