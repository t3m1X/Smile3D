#include "ModFS.h"
#include <Windows.h>
#include <string>
#include "Utilities/Logger.h"

/* Libraries */
#include "Libraries/PhysFS/include/physfs.h"
#include "Libraries/Assimp/include/cfileio.h"
#include "Libraries/Assimp/include/types.h"
#include "Libraries/SDL/include/SDL_filesystem.h"

#pragma comment (lib, "Libraries/PhysFS/libx64/physfs.lib")
#pragma comment (lib, "Libraries/Assimp/libx64/assimp.lib")

namespace filesys {

const char *kAssetsFolder = "Assets/";

std::string working_directory;

/* Assimp variable*/
aiFileIO *assimp_io = nullptr;

/* Assimp IO Functions */

size_t AssimpWrite(aiFile* file, const char* data, size_t size, size_t chunks) {
	PHYSFS_sint64 ret = PHYSFS_write((PHYSFS_File*)file->UserData, (void*)data, size, chunks);
	if (ret == -1)
		CONSOLE_LOG("Filesystem error on WRITE via assimp: %s", PHYSFS_getLastError());

	return (size_t)ret;
}

size_t AssimpRead(aiFile* file, char* data, size_t size, size_t chunks) {
	PHYSFS_sint64 ret = PHYSFS_read((PHYSFS_File*)file->UserData, (void*)data, size, chunks);
	if (ret == -1)
		CONSOLE_LOG("Filesystem error on READ via assimp: %s", PHYSFS_getLastError());

	return (size_t)ret;
}

size_t AssimpTell(aiFile* file) {
	PHYSFS_sint64 ret = PHYSFS_tell((PHYSFS_File*)file->UserData);
	if (ret == -1)
		CONSOLE_LOG("Filesystem error on TELL via assimp: %s", PHYSFS_getLastError());

	return (size_t)ret;
}

size_t AssimpSize(aiFile* file) {
	PHYSFS_sint64 ret = PHYSFS_fileLength((PHYSFS_File*)file->UserData);
	if (ret == -1)
		CONSOLE_LOG("File System error on SIZE via assimp: %s", PHYSFS_getLastError());

	return (size_t)ret;
}

void AssimpFlush(aiFile* file) {
	if (PHYSFS_flush((PHYSFS_File*)file->UserData) == 0)
		CONSOLE_LOG("Filesystem error on FLUSH via assimp: %s", PHYSFS_getLastError());
}

aiReturn AssimpSeek(aiFile* file, size_t pos, aiOrigin from) {
	int res = 0;

	switch (from) {
	case aiOrigin_SET:
		res = PHYSFS_seek((PHYSFS_File*)file->UserData, pos);
		break;
	case aiOrigin_CUR:
		res = PHYSFS_seek((PHYSFS_File*)file->UserData, PHYSFS_tell((PHYSFS_File*)file->UserData) + pos);
		break;
	case aiOrigin_END:
		res = PHYSFS_seek((PHYSFS_File*)file->UserData, PHYSFS_fileLength((PHYSFS_File*)file->UserData) + pos);
		break;
	}

	if (res == 0)
		CONSOLE_LOG("Filesystem error on SEEK via assimp: %s", PHYSFS_getLastError());

	return (res != 0) ? aiReturn_SUCCESS : aiReturn_FAILURE;
}

aiFile* AssimpOpen(aiFileIO* io, const char* name, const char* format) {
	static aiFile file;

	file.UserData = (char*)PHYSFS_openRead(name);
	file.ReadProc = AssimpRead;
	file.WriteProc = AssimpWrite;
	file.TellProc = AssimpTell;
	file.FileSizeProc = AssimpSize;
	file.FlushProc = AssimpFlush;
	file.SeekProc = AssimpSeek;

	return &file;
}

void AssimpClose(aiFileIO* io, aiFile* file) {
	if (PHYSFS_close((PHYSFS_File*)file->UserData) == 0)
		CONSOLE_LOG("Filesystem error on CLOSE via assimp: %s", PHYSFS_getLastError());
}

void CreateAssimpIO() {
    if (assimp_io) {
        delete assimp_io;
        assimp_io = nullptr;
    }

    assimp_io = new aiFileIO;
    assimp_io->OpenProc = AssimpOpen;
    assimp_io->CloseProc = AssimpClose;
}

/* Filesys Functions */
char Init() {
    // Initial Setup
    char* base_path = SDL_GetBasePath();
    PHYSFS_init(base_path);
    SDL_free(base_path);

    WCHAR buf[256];
    GetCurrentDirectoryW(256, buf); //Forcing the UNICODE version
    std::wstring ws(buf);
    working_directory = std::string(ws.begin(), ws.end());
    working_directory += "/";

    AddPath(".");

    // Starting Up
    if (!PHYSFS_setWriteDir("."))
        CONSOLE_AND_SYSTEM_LOG("File system error while creating write dir: %s", PHYSFS_getLastError());

    CreateAssimpIO();

    return 1;
}

char AddPath(const char *path) {
    char ret = PHYSFS_mount(path, nullptr, 1);

    if (!ret)
        CONSOLE_LOG("Filesystem error while adding path: %s", PHYSFS_getLastError());

    return ret;
}

char FileExists (const char *path) {
    return static_cast<char>(PHYSFS_exists(path));
}

char IsDirectory (const char *path) {
    return static_cast<char>(PHYSFS_isDirectory(path));
}

void CreateDir(const char *path) {
    PHYSFS_mkdir(path);
}

char CopyIntoFS (const char* absolute_path_source, const char* destination) {
    char ret = 0;

    const uint kBufferSize = 8192;
    char buffer[kBufferSize];
    size_t size;

    FILE *source = nullptr;
    fopen_s(&source, absolute_path_source, "rb");
    PHYSFS_file *dst = PHYSFS_openWrite(destination);

    if (source && dst) {
        while (size = fread_s(buffer, kBufferSize, 1, kBufferSize, source))
            PHYSFS_write(dst, buffer, 1, size);

        ret = 1;
    }
    else 
        CONSOLE_LOG ("Filesystem error copying \"%s\" into FS as \"%s\"", absolute_path_source, destination);

    fclose (source);
    PHYSFS_close(dst);

    return ret;
}

char Copy (const char* source, const char* destination) {
    char ret = 0;

    const uint kBufferSize = 8192;
    char buffer[kBufferSize];

    PHYSFS_file* src = PHYSFS_openRead(source);
    PHYSFS_file* dst = PHYSFS_openRead(destination);

    if (src && dst) {
        while (PHYSFS_uint32 size = static_cast<PHYSFS_uint32>(PHYSFS_read(src, buffer, 1, kBufferSize)))
            PHYSFS_write(dst, buffer, 1, size);

        ret = 1;
    }
    else 
        CONSOLE_LOG ("Filesystem error copying \"%s\" to \"%s\"", source, destination);

    PHYSFS_close(src);
    PHYSFS_close(dst);

    return ret;
} 



} //namespace filesys