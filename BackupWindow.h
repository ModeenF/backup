/*
 * 2013 Alexander von Gluck IV, All rights reserved
 * Released under the terms of the MIT license
 *
 */
#ifndef __BACKUPWINDOW_H__
#define __BACKUPWINDOW_H__


#include <Window.h>

#include "BackupView.h"


class BackupWindow : public BWindow {
public:
							BackupWindow(BRect frame);
	virtual	bool					QuitRequested();

private:
		BackupView*				fBackupView;
};


#endif /* __BACKUPWINDOW_H__ */

