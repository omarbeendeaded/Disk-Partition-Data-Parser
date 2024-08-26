# Disk Partition Data Parser
My own version of the -l option of the fdisk program. It supports MBR tables and GUID tables.

## Compilation steps
```
gcc myfdisk.c disp.c -luuid -o myfdisk
```

`Note` uuid library is needed to run the program.
Incase of not having uuid, run:

```
sudo apt-get update
sudo apt-get install uuid-dev
```

## Example Output
```
omar@Ubuntu22:~/Desktop/HDD/fdisk$ sudo ./f /dev/sda
Device       Start      End        Sectors    Size       Type
/dev/sda1    2048       4095       2047       1023.5K    BIOS boot 
/dev/sda2    4096       1054719    1050623    513.0M    EFI System
/dev/sda3    1054720    314570751  313516031  149.5G    Linux filesystem
omar@Ubuntu22:~/Desktop/HDD/fdisk$ sudo ./f /dev/sdb
Device        Boot      Start      End        Sectors    Size    Id
/dev/sdb1               2048       2099199    2097152    1.0G   Linux     
/dev/sdb3               2099200    16777215   14678016   7.0G   Extended  
/dev/sdb5               6148       2103299    2097152    1.0G   Linux     
/dev/sdb6               6299648    8396799    2097152    1.0G   Linux     
/dev/sdb7               4200448    6297599    2097152    1.0G   Linux     
/dev/sdb8               8398848    10495999   2097152    1.0G   Linux     
/dev/sdb9               10498048   12595199   2097152    1.0G   Linux     
/dev/sdb10               12597248   16777215   4179968    2.0G   Linux     
omar@Ubuntu22:~/Desktop/HDD/fdisk$ 
```
