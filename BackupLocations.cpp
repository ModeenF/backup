/*
 * 2013 Alexander von Gluck IV, All rights reserved
 * Released under the terms of the MIT license
 *
 * Authors:
 *  Alexander von Gluck IV <kallisti5@unixzen.com>
 */


#include "BackupLocations.h"


// Our backup locations
struct location_map gLocationMap[LOCATION_COUNT] = {
    { (1<<0), B_USER_SETTINGS_DIRECTORY, "User Settings", "User configurations and settings", true, true },
    { (1<<1), B_SYSTEM_SETTINGS_DIRECTORY, "System Settings", "System configurations and settings", true, true },
    { (1<<2), B_SYSTEM_PACKAGES_DIRECTORY, "System Packages", "Installed system software packages", false, false },
    { (1<<3), B_USER_PACKAGES_DIRECTORY, "User Packages", "Installed user software packages", true, false},
};
