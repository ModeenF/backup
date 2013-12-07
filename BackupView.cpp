/*
 * 2013 Alexander von Gluck IV, All rights reserved
 * Released under the terms of the MIT license
 *
 * Authors:
 *	Matthijs Hollemans (size_to_string)
 *	Alexander von Gluck IV <kallisti5@unixzen.com>
 */


#include "BackupView.h"

#include <Button.h>
#include <Directory.h>
#include <Entry.h>
#include <LayoutBuilder.h>
#include <ListView.h>
#include <ScrollView.h>

#include <stdio.h>


// Our backup locations
struct location_map gLocationMap[LOCATION_COUNT] = {
	{ (1<<0), B_USER_SETTINGS_DIRECTORY, "User Settings", "User configurations and settings", true, true },
	{ (1<<1), B_SYSTEM_SETTINGS_DIRECTORY, "System Settings", "System configurations and settings", true, true },
	{ (1<<2), B_SYSTEM_PACKAGES_DIRECTORY, "System Packages", "Installed system software packages", false, false },
	{ (1<<3), B_USER_PACKAGES_DIRECTORY, "User Packages", "Installed user software packages", true, false},
};


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


// #pragma mark - BackupListItem


BackupListItem::BackupListItem(uint32 mapItem, const char* name, const char* description)
	:
	BListItem(0, false),
	fIndex(mapItem),
	fName(name),
	fSize(0),
	fEnabled(NULL),
	fDescription(description)
{
}


BackupListItem::~BackupListItem()
{
	delete fEnabled;
}


void
BackupListItem::DrawItem(BView* owner, BRect /*bounds*/, bool complete)
{
	BListView* list = dynamic_cast<BListView*>(owner);

	if (list == NULL)
		return;

	owner->PushState();

	BRect bounds = list->ItemFrame(list->IndexOf(this));

	rgb_color textColor = ui_color(B_LIST_ITEM_TEXT_COLOR);
	rgb_color backgroundColor = ui_color(B_LIST_BACKGROUND_COLOR);

	if (fIndex % 2)
		backgroundColor = (rgb_color){ 247, 247, 247, 255 };

	// draw background
	list->SetDrawingMode(B_OP_OVER);
	list->SetHighColor(backgroundColor);
	list->FillRect(bounds);

	// set proper colors for "normal" items
	list->SetHighColor(textColor);
	list->SetLowColor(backgroundColor);

	// Set up points for things in BListItem
	BPoint checkboxPt = bounds.LeftTop();
	BPoint namePt = bounds.LeftTop();
	BPoint descriptionPt = bounds.LeftTop();
	BPoint sizePt = bounds.RightTop();
	namePt += BPoint(16 + 8, fFirstlineOffset);
	sizePt += BPoint(0, fFirstlineOffset);
	descriptionPt += BPoint(16 + 8, fSecondlineOffset);
	checkboxPt += BPoint(4, 2);

	list->SetFont(be_bold_font);
	list->DrawString(fName.String(), namePt);

	char sizeText[512];
	size_to_string(fSize, sizeText, 512);
	BString sizeString(sizeText);

	list->SetFont(be_plain_font);
	sizePt -= BPoint(
		be_plain_font->StringWidth(sizeString.String()) + 4.0f, 0);

	list->DrawString(sizeString.String(), sizePt);

	if (textColor.red + textColor.green + textColor.blue > 128 * 3)
		list->SetHighColor(tint_color(textColor, B_DARKEN_1_TINT));
	else
		list->SetHighColor(tint_color(textColor, B_LIGHTEN_1_TINT));

	list->SetFontSize(11);
	list->SetHighColor(ui_color(B_LIST_SELECTED_ITEM_TEXT_COLOR));
	list->DrawString(fDescription.String(), descriptionPt);

	if (!fEnabled) {
		fEnabled = new BCheckBox(BRect(0, 0, 16, 16), fName.String(),
			fName.String(), new BMessage(kMsgUpdateSelection));
		list->AddChild(fEnabled);
		// First run, set default value
		fEnabled->SetValue(gLocationMap[fIndex].defaultValue
			? B_CONTROL_ON : B_CONTROL_OFF);
	}

	fEnabled->SetHighColor(textColor);
	fEnabled->SetLowColor(backgroundColor);
	fEnabled->MoveTo(checkboxPt.x, checkboxPt.y);

	owner->PopState();
}


void
BackupListItem::Update(BView* owner, const BFont* font)
{
	BListItem::Update(owner, font);
	font_height height;
	font->GetHeight(&height);

	float lineHeight = ceilf(height.ascent) + ceilf(height.descent)
		+ ceilf(height.leading);

	fFirstlineOffset = 2 + ceilf(height.ascent + height.leading / 2);
	fSecondlineOffset = fFirstlineOffset + lineHeight;

	SetHeight(2 * lineHeight + 4);
}


int32
BackupListItem::Value()
{
	if (fEnabled)
		return fEnabled->Value();

	return B_CONTROL_OFF;
}


void
BackupListItem::Toggle()
{
	// Toggle item if selected
	if (Value() == B_CONTROL_OFF) {
		fEnabled->SetValue(B_CONTROL_ON);
		return;
	}
	fEnabled->SetValue(B_CONTROL_OFF);
}


// #pragma mark - BackupView


BackupView::BackupView(BRect frame)
	:
	BView(frame, "BackupView", B_FOLLOW_ALL_SIDES, B_WILL_DRAW)
{
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	SetLowColor(ViewColor());
	SetHighColor(0, 0, 0);
	SetLayout(new BGroupLayout(B_VERTICAL));

	fBackupList = new BListView(BRect(0, 0, 0, 0), "items", B_SINGLE_SELECTION_LIST, B_FOLLOW_ALL_SIDES);

	for (uint32 i = 0; i < LOCATION_COUNT; i++) {
		BackupListItem* item = new BackupListItem(i, gLocationMap[i].name,
			gLocationMap[i].description);
		fBackupList->AddItem(item);
	}

	// Add total size
	BStringView* backupSize = new BStringView("total size", "Total size:");
	fBackupSizeText = new BStringView("backup size", "");
	fBackupSizeText->SetAlignment(B_ALIGN_RIGHT);

	BButton* button = new BButton(BRect(0, 0, 10, 10), "backup", "Backup!",
		new BMessage(kMsgDoBackup));
	button->MakeDefault(true);

	BGroupLayout* totalGroup = BLayoutBuilder::Group<>(B_VERTICAL, 0.0)
		.AddGrid()
			.Add(backupSize, 0, 0)
			.Add(fBackupSizeText, 1, 0)
		.End()
		.SetInsets(B_USE_DEFAULT_SPACING, B_USE_DEFAULT_SPACING,
			B_USE_DEFAULT_SPACING, B_USE_DEFAULT_SPACING)
	;

	// Attach all of the LayoutGroups to the view
	AddChild(BLayoutBuilder::Group<>(B_VERTICAL, 0.0)
		.Add(new BScrollView("backup item list", fBackupList, B_FOLLOW_LEFT | B_FOLLOW_TOP, 0, false, true))
		.Add(totalGroup)
		.Add(button)
	);

	// Refresh the sizes of each item
	RefreshSizes();
	Show();
}


void
BackupView::RefreshSizes()
{
	off_t size;

	for (uint32 i = 0; i < LOCATION_COUNT; i++) {
		BPath directory;
		find_directory(gLocationMap[i].location, &directory);
		size = DirectorySize(&directory, gLocationMap[i].recurse);
		BackupListItem* item = (BackupListItem*)fBackupList->ItemAt(i);
		if (!item)
			continue;
		item->SetBytes(size);
	}

	RefreshTotal();
}


void
BackupView::RefreshTotal()
{
	char sizeText[512];
	off_t totalSize = 0;

	for (uint32 i = 0; i < LOCATION_COUNT; i++) {
		BackupListItem* item = (BackupListItem*)fBackupList->ItemAt(i);
		if (item->Value() == B_CONTROL_ON) {
			totalSize += item->Bytes();
		}
	}
	
	// Update total backup size
	size_to_string(totalSize, sizeText, 512);
	fBackupSizeText->SetText(sizeText);
}


uint32
BackupView::GetTasks()
{
	uint32 tasks = 0;

	for (uint32 i = 0; i < LOCATION_COUNT; i++) {
		BackupListItem* item = (BackupListItem*)fBackupList->ItemAt(i);
		if (item->Value() == B_CONTROL_ON) {
			tasks |= gLocationMap[i].flags;
		}
	}

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
	//printf("%s: %s\n", __func__, path->Path());
	BDirectory dir(path->Path());
	int32 entries = dir.CountEntries();
	//printf("%s: items: %" B_PRId32 "\n", __func__, entries);
	dir.Rewind();
	for (int32 i = 0; i < entries; i++) {
		BEntry entry;
		dir.GetNextEntry(&entry);
		struct stat st;
		entry.GetStat(&st);
		if (S_ISDIR(st.st_mode) && recurse == true) {
			BPath nextPath;
			entry.GetPath(&nextPath);
			bytes += DirectorySize(&nextPath);
		} else {
			bytes += st.st_size;
		}
	}
	return bytes;
}
