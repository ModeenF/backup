/*
 * 2013 Alexander von Gluck IV, All rights reserved
 * Released under the terms of the MIT license
 *
 */
#ifndef __BACKUPVIEW_H__
#define __BACKUPVIEW_H__


#include <CheckBox.h>
#include <Path.h>
#include <String.h>
#include <StringView.h>
#include <View.h>
#include <ListItem.h>


const uint32 kMsgDoBackup = 'bkup';
const uint32 kMsgUpdateSelection = 'upda';


class BackupListItem : public BListItem {
public:
								BackupListItem(uint32 mapItem, const char* name,
									const char* description);
								~BackupListItem();

			void				DrawItem(BView* owner,
									BRect bounds, bool complete);
			void				Update(BView* owner, const BFont* font);
			void				Toggle();

			void				SetBytes(off_t bytes) { fSize = bytes; };
			off_t				Bytes() { return fSize; };

			int32				Value();

private:
			uint32				fIndex;
			off_t				fSize;
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

