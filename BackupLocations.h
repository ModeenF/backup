/*
 * 2013 Alexander von Gluck IV, All rights reserved
 * Released under the terms of the MIT license
 *
 * Authors:
 *  Alexander von Gluck IV <kallisti5@unixzen.com>
 */
#ifndef __BACKUP_LOCATIONS_H
#define __BACKUP_LOCATIONS_H


#include <FindDirectory.h>


#define LOCATION_COUNT  4
struct location_map {
    uint32 flags;
    directory_which location;
    const char* name;
    const char* description;
    bool defaultValue;
    bool recurse;
};


#endif /* __BACKUP_LOCATIONS_H */
