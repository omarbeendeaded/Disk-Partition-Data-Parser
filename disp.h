#ifndef DISP_H_
#define DISP_H_

#include <uuid/uuid.h>

typedef struct {
	uint8_t status;
	uint8_t first_chs[3];
	uint8_t partition_type;
	uint8_t last_chs[3];
	uint32_t lba;
	uint32_t sector_count;
} PartitionEntry;

typedef struct {
	uuid_t partition_type;
	uuid_t partition_GUID;
	uint64_t firstLBA;
	uint64_t lastLBA;
	uint64_t attr_flags;
	char partition_name[72];
} GUIDEntry;


void dispMBREntry  (PartitionEntry entry, char* fileName, int count, size_t offset);
void dispMBRTable  (PartitionEntry* tableEntry, char* fileName, int fd);
void dispGUIDEntry (GUIDEntry entry, char* fileName, int count);
void dispGUIDTable (int fd, char* fileName);

#endif 

