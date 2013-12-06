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
	fUserSettingEnable = new BCheckBox("backup user settings",
		"User settings", new BMessage(kMsgUpdateSelection));
	fUserSettingEnable->SetValue(B_CONTROL_ON);
	fUserSettingSizeText = new BStringView("user settings size", "");

	fSysSettingEnable = new BCheckBox("backup system",
		"System settings", new BMessage(kMsgUpdateSelection));
	fSysSettingEnable->SetValue(B_CONTROL_ON);
	fSysSettingSizeText = new BStringView("system setting size", "");

	fSysPackageEnable = new BCheckBox("backup apps",
		"Installed Packages", new BMessage(kMsgUpdateSelection));
	fSysPackageEnable->SetValue(B_CONTROL_OFF);
	fSysPackageSizeText = new BStringView("sys app size", "");

	BStringView* backupSize = new BStringView("total size", "Total size:");
	fBackupSizeText = new BStringView("backup size", "");

	BGroupLayout* settingsGroup = BLayoutBuilder::Group<>(B_VERTICAL, 0.0)
		.AddGrid()
			.Add(fUserSettingEnable, 0, 0)
			.Add(fUserSettingSizeText, 1, 0)
			.Add(fSysSettingEnable, 0, 1)
			.Add(fSysSettingSizeText, 1, 1)
			.Add(fSysPackageEnable, 0, 2)
			.Add(fSysPackageSizeText, 1, 2)
			.Add(backupSize, 0, 3)
			.Add(fBackupSizeText, 1, 3)
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
	BPath userSettingDirectory;
	find_directory(B_USER_SETTINGS_DIRECTORY, &userSettingDirectory);
	fUserSettingBytes = DirectorySize(&userSettingDirectory, true);
	size_to_string(fUserSettingBytes, sizeText, 512);
	fUserSettingSizeText->SetText(sizeText);

	// Refresh System Directory
	BPath sysSettingDirectory;
	find_directory(B_SYSTEM_SETTINGS_DIRECTORY, &sysSettingDirectory);
	fSysSettingBytes = DirectorySize(&sysSettingDirectory, true);
	size_to_string(fSysSettingBytes, sizeText, 512);
	fSysSettingSizeText->SetText(sizeText);

	// Refresh System Package Directory
	BPath sysPackageDirectory;
	find_directory(B_SYSTEM_PACKAGES_DIRECTORY, &sysPackageDirectory);
	fSysPackageBytes = DirectorySize(&sysPackageDirectory, false);
	size_to_string(fSysPackageBytes, sizeText, 512);
	fSysPackageSizeText->SetText(sizeText);

	RefreshTotal();
}


void
BackupView::RefreshTotal()
{
	char sizeText[512];
	off_t totalSize = 0;
	if ((fUserSettingEnable->Value() && B_CONTROL_ON) != 0)
		totalSize += fUserSettingBytes;
	if ((fSysSettingEnable->Value() && B_CONTROL_ON) != 0)
		totalSize += fSysSettingBytes;
	if ((fSysPackageEnable->Value() && B_CONTROL_ON) != 0)
		totalSize += fSysPackageBytes;

	// Update total backup size
	size_to_string(totalSize, sizeText, 512);
	fBackupSizeText->SetText(sizeText);
}


uint32
BackupView::GetTasks()
{
	uint32 tasks = 0;

	if ((fUserSettingEnable->Value() && B_CONTROL_ON) != 0)
		tasks |= DO_BACKUP_USER_HOME;
	if ((fSysSettingEnable->Value() && B_CONTROL_ON) != 0)
		tasks |= DO_BACKUP_SYS_SETTINGS;
	if ((fSysPackageEnable->Value() && B_CONTROL_ON) != 0)
		tasks |= DO_BACKUP_APPS;

	return tasks;
}


off_t
BackupView::DirectorySize(BPath* path, bool recurse)
{
	off_t bytes = 0;
	if (!path) {
		printf("Error: Invalid path passed!\n");
		return bytes;
	}

	printf("%s: %s\n", __func__, path->Path());
	BDirectory dir(path->Path());
	int32 entries = dir.CountEntries();
	printf("%s: items: %" B_PRId32 "\n", __func__, entries);
	dir.Rewind();
	for (int32 i = 0; i <= entries; i++) {
		BEntry entry;
		dir.GetNextEntry(&entry);
		struct stat st;
		entry.GetStat(&st);
		if (S_ISDIR(st.st_mode) && recurse == true) {
			BPath path;
			entry.GetPath(&path);
			bytes += DirectorySize(&path);
		} else {
			bytes += st.st_size;
		}
	}
	return bytes;
}
