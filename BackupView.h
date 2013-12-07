/*
 * 2013 Alexander von Gluck IV, All rights reserved
 * Released under the terms of the MIT license
 *
 */
#ifndef __BACKUPVIEW_H__
#define __BACKUPVIEW_H__


#include <CheckBox.h>
#include <FindDirectory.h>
#include <Path.h>
#include <String.h>
#include <StringView.h>
#include <View.h>
#include <ListItem.h>


#define DO_BACKUP_USER_HOME		(1<<0)
#define DO_BACKUP_SYS_SETTINGS		(1<<1)
#define DO_BACKUP_APPS			(1<<2)


const uint32 kMsgDoBackup = 'bkup';
const uint32 kMsgUpdateSelection = 'upda';

#define LOCATION_COUNT  3
struct location_map {
	uint32 flag;
    uint32 location;
    const char* name;
	const char* description;
	bool defaultValue;
    bool recurse;
};


class BackupListItem : public BListItem {
public:
								BackupListItem(uint32 mapItem, const char* name,
									const char* description);
								~BackupListItem();

			void				DrawItem(BView* owner,
									BRect bounds, bool complete);
			void				Update(BView* owner, const BFont* font);

private:
			uint32				fIndex;
			BString				fName;
			BString				fDescription;
			BCheckBox*			fEnabled;

			float				fFirstlineOffset;
			float				fSecondlineOffset;
};


class BackupView : public BView {
public:
								BackupView(BRect frame);
			void				RefreshTotal();
			uint32				GetTasks();

private:
			off_t				DirectorySize(BPath* path, bool recurse = true);
			void				RefreshSizes();

			BListView*			fBackupList;
			BStringView*		fBackupSizeText;
};


#endif /* __BACKUPVIEW_H__ */

