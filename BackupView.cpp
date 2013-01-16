/*
 * 2013 Alexander von Gluck IV, All rights reserved
 * Released under the terms of the MIT license
 *
 */


#include "BackupView.h"

#include <Button.h>
#include <LayoutBuilder.h>


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
	fHomeSize->SetText("100MB");
	fSysSettingSize->SetText("10KB");
}
