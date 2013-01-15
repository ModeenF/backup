/*
 * 2013 Alexander von Gluck IV, All rights reserved
 * Released under the terms of the MIT license
 *
 */
#ifndef __BACKUPAPP_H__
#define __BACKUPAPP_H__


#include <Application.h>
#include <Deskbar.h>


#define APPLICATION_SIG "application/x-vnd.Mynti"


class BackupApp : public BApplication {
public:
							BackupApp();
	virtual						~BackupApp();
//		void					ReadyToRun();
//		void					MessageReceived(BMessage *message);
};


#endif /* __BACKUPAPP_H__ */

