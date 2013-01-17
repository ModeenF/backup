/*
 * 2013 Alexander von Gluck IV, All rights reserved
 * Released under the terms of the MIT license
 *
 */
#ifndef __BACKUPVIEW_H__
#define __BACKUPVIEW_H__


#include <CheckBox.h>
#include <Path.h>
#include <StringView.h>
#include <View.h>


#define DO_BACKUP_USER_HOME		(1<<0)
#define DO_BACKUP_SYS_SETTINGS		(1<<1)


const uint32 kMsgDoBackup = 'bkup';


class BackupView : public BView {
public:
							BackupView(BRect frame);
			void				RefreshSizes();
			uint32				GetTasks();

private:
			off_t				DirectorySize(BPath* path);
			BCheckBox*			fHomeEnable;
			BStringView*			fHomeSize;
			BCheckBox*			fSysSettingEnable;
			BStringView*			fSysSettingSize;
};


#endif /* __BACKUPVIEW_H__ */

