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


class BackupView : public BView {
public:
							BackupView(BRect frame);
			void				RefreshSizes();
private:
			int32				DirectorySize(BPath* path);
			BCheckBox*			fHomeEnable;
			BStringView*			fHomeSize;
			BCheckBox*			fSysSettingEnable;
			BStringView*			fSysSettingSize;
};


#endif /* __BACKUPVIEW_H__ */

