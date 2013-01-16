/*
 * 2013 Alexander von Gluck IV, All rights reserved
 * Released under the terms of the MIT license
 *
 */


#include "BackupView.h"


BackupView::BackupView(BRect frame)
	:
	BView(frame, "BackupView", B_FOLLOW_ALL_SIDES, B_WILL_DRAW)
{
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	Show();
}
