#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <inttypes.h>
#include <string.h>
#include <uuid/uuid.h>

#include "disp.h"

void dispMBREntry (PartitionEntry entry, char* fileName, int count, size_t offset)
{
	if (entry.sector_count == 0) return;
	
	// Get partition type
	char* buf;
	switch (entry.partition_type)
	{
		case 0x83: buf = strdup("Linux");    break;
		case 0x05: buf = strdup("Extended"); break;
		default:   buf = strdup("N/A");      break;
	}

	// Get partition size
	long long size = entry.sector_count;
	double convSize;
	char  postfix;
	if      (size < 2)                 postfix = 'B';
	else if (size < 2 * 1024)        { postfix = 'K'; convSize = size / 1024.0; }
	else if (size < 2 * 1024 * 1024) { postfix = 'M', convSize = size / (1024.0 * 1024.0); }
	else                             { postfix = 'G', convSize = size / (1024.0 * 1024.0 * 1024.0); }	


	// Display partition info
	printf("%s%d    %-10c %-10lu %-10lu %-10u %.1lf%c   %-10s\n",
	       fileName,
	       count,
	       entry.status == 0x80 ? '*' : ' ',
	       entry.lba + (offset / 512),
	       entry.lba + entry.sector_count + (offset / 512) - 1,
	       entry.sector_count,
	       convSize * 512,
	       postfix,
	       buf);

}

void dispMBRTable (PartitionEntry* tableEntry, char* fileName, int fd)
{
	char buf[512];
	
	printf("%-10s    %-9s %-10s %-10s %-10s %-7s %s\n", "Device",
		"Boot", "Start", "End", "Sectors", "Size", "Id");
	
	// Display MBR partitions
	int extended = -1;
	for (int i = 0; i < 4; i++) {
		dispMBREntry(tableEntry[i], fileName, i + 1, 0);
		
		// Store the index of the extended partition if found
		if (tableEntry[i].partition_type == 0x05) extended = i;		
	}

	// Display EBR partitions if found
	if (extended != -1)
	{
		int count = 5;
		size_t offset = tableEntry[extended].lba;

		lseek(fd, offset* 512, SEEK_SET);
		read(fd, buf, 512);

		tableEntry = (PartitionEntry*) &buf[446];
		dispMBREntry(tableEntry[0], fileName, count++, offset);

		// Traverse linked list until NULL
		while (tableEntry[1].sector_count != 0)
		{
			size_t offsetInternal = offset + tableEntry[1].lba;
			
			lseek(fd, offsetInternal * 512, SEEK_SET);
			read(fd, buf, 512);			
		
			dispMBREntry(tableEntry[0], fileName, count++, offsetInternal * 512);
		}
	}
}

void dispGUIDEntry (GUIDEntry entry, char* fileName, int count)
{
	// Get partition type
	char  buf[37];
	char* type;
	uuid_unparse(entry.partition_type, buf);

	if      (strcmp(buf, "48616821-4964-6f6e-744e-656564454649") == 0) type = strdup("BIOS boot");
	else if (strcmp(buf, "28732ac1-1ff8-d211-ba4b-00a0c93ec93b") == 0) type = strdup("EFI System");
	else if (strcmp(buf, "af3dc60f-8384-7247-8e79-3d69d8477de4") == 0) type = strdup("Linux filesystem");
	

	// Get partition size
	long long size = entry.lastLBA - entry.firstLBA;
	double convSize;
	char  postfix;
	if      (size < 2)                 postfix = 'B';
	else if (size < 2 * 1024)        { postfix = 'K'; convSize = size / 1024.0; }
	else if (size < 2 * 1024 * 1024) { postfix = 'M', convSize = size / (1024.0 * 1024.0); }
	else                             { postfix = 'G', convSize = size / (1024.0 * 1024.0 * 1024.0); }	


	// Display partition info
	printf("%s%d    %-10lu %-10lu %-10lu %.1lf%-3c  %-10s\n",
	       fileName,
	       count,
	       entry.firstLBA,
	       entry.lastLBA,
	       entry.lastLBA - entry.firstLBA,
	       convSize * 512,
	       postfix,
	       type);
}



void dispGUIDTable (int fd, char* fileName) 
{
	printf("%-10s   %-10s %-10s %-10s %-10s %s\n", "Device", "Start", "End", "Sectors", "Size", "Type");
	
	char tableBuf[512];
	lseek(fd, 512, SEEK_SET);
	read(fd, tableBuf, 512);

	// Get number of partitions and address
	uint64_t* firstEntryLBA = (uint64_t*)&tableBuf[72];
	uint32_t* arrSize = (uint32_t*)&tableBuf[80];

	char entryBuf[128];
	
	// Print partition info
	for (int i = 0; i < *arrSize; i++)
	{
		lseek(fd, (*firstEntryLBA * 512) + (i * 128), SEEK_SET);
		read(fd, entryBuf, 128);
		GUIDEntry* entry = (GUIDEntry*)entryBuf;

		if (entry->firstLBA == 0) break;
		
		dispGUIDEntry(*entry, fileName, i + 1);
	}	
}	







