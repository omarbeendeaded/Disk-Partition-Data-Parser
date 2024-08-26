#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <inttypes.h>
#include "disp.h"

int main(int argc, char **argv)
{
	char buf[512];

	// Get first sector data
	int fd = open(argv[1], O_RDONLY);
	read(fd, buf, 512);
	PartitionEntry *tableEntry = (PartitionEntry *) &buf[446];
	
	// Parse table based on type
	if (tableEntry[0].partition_type == 0x83 || tableEntry[0].partition_type == 0x05) 
	{
		dispMBRTable(tableEntry, argv[1], fd);
	}
	else if (tableEntry[0].partition_type == 0xee)
	{
		dispGUIDTable(fd, argv[1]);
	}
	else
	{
		printf("Unsupported Partition Table\n");
	}

	close(fd);

    return 0;
}

