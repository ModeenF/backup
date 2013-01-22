/*
 * 2013 Alexander von Gluck IV, All rights reserved
 * Released under the terms of the MIT license
 *
 * Authors:
 *	Matthijs Hollemans (size_to_string)
 */


#include "BackupView.h"

#include <Button.h>
#include <Directory.h>
#include <Entry.h>
#include <FindDirectory.h>
#include <LayoutBuilder.h>

#include <stdio.h>


static void
size_to_string(off_t byteCount, char* name, int maxLength)
{
	struct {
		off_t           limit;
		float           divisor;
		const char*     format;
	} scale[] = {
		{ 0x100000, 1024.0, "%.2f KiB" },
		{ 0x40000000, 1048576.0, "%.2f MiB" },
		{ 0x10000000000ull, 1073741824.0, "%.2f GiB" },
		{ 0x4000000000000ull, 1.09951162778e+12, "%.2f TiB" }
	};

	if (byteCount < 1024) {
		snprintf(name, maxLength, "%lld bytes",
			byteCount);
	} else {
		int i = 0;
		while (byteCount >= scale[i].limit)
			i++;

		snprintf(name, maxLength, scale[i].format,
			byteCount / scale[i].divisor);
        }
}


BackupView::BackupView(BRect frame)
	:
	BView(frame, "BackupView", B_FOLLOW_ALL_SIDES, B_WILL_DRAW)
{
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	SetLowColor(ViewColor());
	SetHighColor(0, 0, 0);
	SetLayout(new BGroupLayout(B_VERTICAL));

	// Create "Settings Group"
	fHomeEnable = new BCheckBox("backup home",
		"Home directory", new BMessage(kMsgUpdateSelection));
	fHomeEnable->SetValue(B_CONTROL_ON);
	fHomeSizeText = new BStringView("home size", "");

	fSysSettingEnable = new BCheckBox("backup system",
		"System settings", new BMessage(kMsgUpdateSelection));
	fSysSettingEnable->SetValue(B_CONTROL_ON);
	fSysSettingSizeText = new BStringView("system setting size", "");

	BStringView* backupSize = new BStringView("total size", "Total size:");
	fBackupSizeText = new BStringView("backup size", "");

	BGroupLayout* settingsGroup = BLayoutBuilder::Group<>(B_VERTICAL, 0.0)
		.AddGrid()
			.Add(fHomeEnable, 0, 0)
			.Add(fHomeSizeText, 1, 0)
			.Add(fSysSettingEnable, 0, 1)
			.Add(fSysSettingSizeText, 1, 1)
			.Add(backupSize, 0, 2)
			.Add(fBackupSizeText, 1, 2)
		.End()
		.AddGlue()
		.SetInsets(B_USE_DEFAULT_SPACING, B_USE_DEFAULT_SPACING,
			B_USE_DEFAULT_SPACING, B_USE_DEFAULT_SPACING)
	;

	BButton* button = new BButton(BRect(0, 0, 10, 10), "backup", "Backup!",
		new BMessage(kMsgDoBackup));
	button->MakeDefault(true);

	// Attach all of the LayoutGroups to the view
	AddChild(BLayoutBuilder::Group<>(B_VERTICAL, 0.0)
		.Add(settingsGroup)
		.Add(button)
	);

	// Refresh the sizes of each item
	RefreshSizes();
	Show();
}


void
BackupView::RefreshSizes()
{
	char sizeText[512];

	// Refresh Home Directory
	BPath homeDirectory;
	find_directory(B_USER_DIRECTORY, &homeDirectory);
	fHomeBytes = DirectorySize(&homeDirectory);
	size_to_string(fHomeBytes, sizeText, 512);
	fHomeSizeText->SetText(sizeText);

	// Refresh System Directory
	BPath sysSettingsDirectory;
	find_directory(B_COMMON_SETTINGS_DIRECTORY, &sysSettingsDirectory);
	fSysSettingBytes = DirectorySize(&sysSettingsDirectory);
	size_to_string(fSysSettingBytes, sizeText, 512);
	fSysSettingSizeText->SetText(sizeText);

	RefreshTotal();
}


void
BackupView::RefreshTotal()
{
	char sizeText[512];
	off_t totalSize = 0;
	if ((fHomeEnable->Value() && B_CONTROL_ON) != 0)
		totalSize += fHomeBytes;
	if ((fSysSettingEnable->Value() && B_CONTROL_ON) != 0)
		totalSize += fSysSettingBytes;

	// Update total backup size
	size_to_string(totalSize, sizeText, 512);
	fBackupSizeText->SetText(sizeText);
}


uint32
BackupView::GetTasks()
{
	uint32 tasks = 0;

	if ((fHomeEnable->Value() && B_CONTROL_ON) != 0)
		tasks |= DO_BACKUP_USER_HOME;
	if ((fSysSettingEnable->Value() && B_CONTROL_ON) != 0)
		tasks |= DO_BACKUP_SYS_SETTINGS;

	return tasks;
}


off_t
BackupView::DirectorySize(BPath* path)
{
	//printf("%s: %s\n", __func__, path->Path());
	BDirectory dir(path->Path());
	int32 entries = dir.CountEntries();
	//printf("%s: items: %" B_PRId32 "\n", __func__, entries);
	dir.Rewind();
	off_t bytes = 0;
	for (int32 i; i < entries; i++) {
		BEntry entry;
		dir.GetNextEntry(&entry);
		struct stat st;
		entry.GetStat(&st);
		if (S_ISDIR(st.st_mode)) {
			BPath path;
			entry.GetPath(&path);
			bytes += DirectorySize(&path);
		} else {
			bytes += st.st_size;
		}
	}
	return bytes;
}
