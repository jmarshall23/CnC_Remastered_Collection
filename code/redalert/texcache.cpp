// TexCache.cpp
//

#include "FUNCTION.H"
#include "TEXCACHE.H"

#define CACHE_HEADER_FILENAME		"cache/cache_header_v2.vhdr"
#define CACHE_DATA_FILERNAME		"cache/cache_data_v2.vdata"

TexCache texCache;

void TexCache::Init(void) {
	// Check to see if we have a texture cache, if not make one.
	if (!WwFile::FileExists(CACHE_HEADER_FILENAME) || !WwFile::FileExists(CACHE_DATA_FILERNAME)) {
		texCacheHeaderFileWrite = new WwFile();
		texCacheHeaderFileWrite->InitWrite(CACHE_HEADER_FILENAME);

		texCacheDataFileWrite = new WwFile();
		texCacheDataFileWrite->InitWrite(CACHE_DATA_FILERNAME);
	}
	else {
		texCacheHeaderFileWrite = new WwFile();
		texCacheHeaderFileWrite->InitAppend(CACHE_HEADER_FILENAME);

		texCacheDataFileWrite = new WwFile();
		texCacheDataFileWrite->InitAppend(CACHE_DATA_FILERNAME);
		
		texCacheHeaderFileWrite->Seek(0, SEEK_END);
		texCacheDataFileWrite->Seek(0, SEEK_END);
	}

	texCacheHeaderFile = new WwFile();
	texCacheHeaderFile->InitOpen(CACHE_HEADER_FILENAME);
	texCacheDataFile = new WwFile();
	texCacheDataFile->InitOpen(CACHE_DATA_FILERNAME);

	while (!texCacheHeaderFile->EndOfFile()) {
		CacheEntry_t entry;
		texCacheHeaderFile->Scanf("texture %s %d %d %d %llu\n", &entry.name, &entry.width, &entry.height, &entry.bpp, &entry.cachePosition);
		entry.hash = generateHashValue(entry.name, strlen(entry.name));
		cacheEntries.push_back(entry);
	}
}

void TexCache::AddImage(const char* name, int width, int height, int bpp, byte* data) {
	CacheEntry_t entry;
	strcpy(entry.name, name);
	entry.width = width;
	entry.height = height;
	entry.bpp = bpp;
	entry.hash = generateHashValue(name, strlen(name));
	entry.cachePosition = texCacheDataFileWrite->Tell();
	cacheEntries.push_back(entry);

	int length = entry.width * entry.height * entry.bpp;
	texCacheDataFileWrite->Write(data, length);
	texCacheHeaderFileWrite->Printf("texture %s %d %d %d %llu\n", entry.name, entry.width, entry.height, entry.bpp, entry.cachePosition);
	texCacheHeaderFileWrite->Flush();
	texCacheDataFileWrite->Flush();
}

bool TexCache::FindCachedImage(int64_t hash, int& width, int& height, int& bpp, byte* data) {
	int cacheEntriesSize = cacheEntries.size();
	if (cacheEntriesSize <= 0)
		return false;

	CacheEntry_t* cacheEntry = &cacheEntries[0];
	for (int i = 0; i < cacheEntriesSize; i++, cacheEntry++) {		
		if (cacheEntry->hash == hash) {
			int length = cacheEntry->width * cacheEntry->height * cacheEntry->bpp;
			texCacheDataFile->Seek(cacheEntry->cachePosition, SEEK_SET);
			texCacheDataFile->Read(data, length);
			width = cacheEntry->width;
			height = cacheEntry->height;
			bpp = cacheEntry->bpp;
			return true;
		}
	}	

	return false;
}
