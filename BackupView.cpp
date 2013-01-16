/*
 * 2013 Alexander von Gluck IV, All rights reserved
 * Released under the terms of the MIT license
 *
 */


#include "BackupView.h"

#include <Button.h>
#include <Directory.h>
#include <Entry.h>
#include <FindDirectory.h>
#include <LayoutBuilder.h>

#include <stdio.h>


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
		"Home Directory", NULL);
		fHomeEnable->SetValue(B_CONTROL_ON);
	fHomeSize = new BStringView("home size", "");

	fSysSettingEnable = new BCheckBox("backup system",
		"System Settings", NULL);
		fSysSettingEnable->SetValue(B_CONTROL_ON);
	fSysSettingSize = new BStringView("system setting size", "");

	BGroupLayout* settingsGroup = BLayoutBuilder::Group<>(B_VERTICAL, 0.0)
		.AddGrid()
			.Add(fHomeEnable, 0, 0)
			.Add(fHomeSize, 1, 0)
			.Add(fSysSettingEnable, 0, 1)
			.Add(fSysSettingSize, 1, 1)
		.End()
		.AddGlue()
		.SetInsets(B_USE_DEFAULT_SPACING, B_USE_DEFAULT_SPACING,
			B_USE_DEFAULT_SPACING, B_USE_DEFAULT_SPACING)
	;

	BButton* button = new BButton(BRect(0, 0, 10, 10), "backup", "Backup!", NULL);
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
	BPath homeDirectory;
	find_directory(B_USER_DIRECTORY, &homeDirectory);
	uint32 bytes = DirectorySize(&homeDirectory);
	printf("%s: %" B_PRId32 "\n", __func__, bytes);
	fHomeSize->SetText("100MB");

	BPath sysSettingsDirectory;
	find_directory(B_COMMON_SETTINGS_DIRECTORY, &sysSettingsDirectory);
	//bytes = DirectorySize(&sysSettingsDirectory);
	fSysSettingSize->SetText("10KB");
}


int32
BackupView::DirectorySize(BPath* path)
{
	printf("%s: %s\n", __func__, path->Path());
	BDirectory dir(path->Path());
	int32 entries = dir.CountEntries();
	printf("%s: items: %" B_PRId32 "\n", __func__, entries);
	dir.Rewind();
	int32 bytes = 0;
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
