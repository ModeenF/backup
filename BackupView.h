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
#define DO_BACKUP_APPS			(1<<2)


const uint32 kMsgDoBackup = 'bkup';
const uint32 kMsgUpdateSelection = 'upda';


class BackupView : public BView {
public:
							BackupView(BRect frame);
			void				RefreshTotal();
			uint32				GetTasks();

private:
			off_t				DirectorySize(BPath* path);
			void				RefreshSizes();
			BCheckBox*			fHomeEnable;
			BStringView*			fHomeSizeText;
			off_t				fHomeBytes;
			BCheckBox*			fSysSettingEnable;
			BStringView*			fSysSettingSizeText;
			off_t				fSysSettingBytes;
			BCheckBox*			fAppEnable;
			BStringView*			fAppSizeText;
			off_t				fAppBytes;

			BStringView*			fBackupSizeText;
};


#endif /* __BACKUPVIEW_H__ */

