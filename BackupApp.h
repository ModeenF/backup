/*
 * 2013 Alexander von Gluck IV, All rights reserved
 * Released under the terms of the MIT license
 *
 */
#ifndef __BACKUPAPP_H__
#define __BACKUPAPP_H__


#include <Application.h>
#include <Deskbar.h>

#include "BackupWindow.h"


#define APPLICATION_SIG "application/x-vnd.Backup"


class BackupApp : public BApplication {
public:
							BackupApp();
	virtual						~BackupApp();
//		void					ReadyToRun();
//		void					MessageReceived(BMessage *message);
private:
		BackupWindow*				fMainWindow;
};


#endif /* __BACKUPAPP_H__ */

