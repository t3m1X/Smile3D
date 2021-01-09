#ifndef SMILE3D_MODULES_MODFS_H
#define SMILE3D_MODULES_MODFS_H

#include "globals.hpp"

struct SDL_RWops;

namespace filesys {

extern const char *kAssetsFolder;
extern const char *kLibraryFolder;
extern const char *kSettingsFolder;

char Init();

/* Utility */
char AddPath (const char* path);
char FileExists (const char* path);
char IsDirectory (const char* path);
void CreateDir (const char* path);
//Copy from outside virtual filesystem
char CopyIntoFS (const char* absolute_path_source, const char* destination);
char Copy (const char* source, const char* destination);

/* I/O */
uint Load(const char* path, char** buffer);
SDL_RWops* Load(const char* path);
uint Save(const char* path, const void* buffer, uint size, char append = 0);
char DeleteFile (const char* path);
char DeleteDirectoryAndContents(const char* path, char recursive = 1);

/* Path Getters */
const char* GetBasePath();
const char* GetWritePath();
const char* GetReadPath();
const char* GetWorkingPath();

} //filesys

#endif //SMILE3D_MODULES_MODFS_H