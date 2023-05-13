#include "fs.h"
#include "disk.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#define FS_MAGIC           0xf0f03410
#define INODES_PER_BLOCK   128
#define POINTERS_PER_INODE 5
#define POINTERS_PER_BLOCK 1024

// Returns the number of dedicated inode blocks given the disk size in blocks
#define NUM_INODE_BLOCKS(disk_size_in_blocks) (1 + (disk_size_in_blocks / 10))

int *bitmap;

struct fs_superblock {
  int magic;          // Magic bytes
  int nblocks;        // Size of the disk in number of blocks
  int ninodeblocks;   // Number of blocks dedicated to inodes
  int ninodes;        // Number of dedicated inodes
};

struct fs_inode {
  int isvalid;                      // 1 if valid (in use), 0 otherwise
  int size;                         // Size of file in bytes
  int direct[POINTERS_PER_INODE];   // Direct data block numbers (0 if invalid)
  int indirect;                     // Indirect data block number (0 if invalid)
};

union fs_block {
  struct fs_superblock super;               // Superblock
  struct fs_inode inode[INODES_PER_BLOCK];  // Block of inodes
  int pointers[POINTERS_PER_BLOCK];         // Indirect block of direct data block numbers
  char data[DISK_BLOCK_SIZE];               // Data block
};

int fs_debug(char **args) {
  union fs_block block;

  disk_read(0,block.data);

  printf("superblock:\n");
  if(FS_MAGIC == block.super.magic)
    printf("    magic number is valid\n");
  else
    printf("    magic number is invalid\n");    
  printf("    %d blocks\n",block.super.nblocks);
  printf("    %d inode blocks\n",block.super.ninodeblocks);
  printf("    %d inodes total\n",block.super.ninodes);
  int blocks = block.super.ninodeblocks,inodes = block.super.ninodes;
  
  for(int i=0;i<blocks;i++){
    disk_read(i+1,block.data);
    for(int j=0;j<inodes;j++){
      if(block.inode[i].isvalid){
	printf("inode %d:",j);
	printf("\n    size: %d bytes",block.inode[j].size);
	printf("\n    direct blocks:");
	for(int w=0;w<POINTERS_PER_INODE;w++)
	  if(block.inode[j].direct[w])
	    printf(" %d",block.inode[j].direct[w]);
	printf("\n");
      }
    }
      
    if(block.inode[i].isvalid)
      printf("\n");
  }
  return 1;
}

int fs_format(char **args){
  union fs_block block;
  int blocks;
  
  block.super.magic = FS_MAGIC;
  blocks = disk_size();
  block.super.nblocks = blocks;
  block.super.ninodeblocks = NUM_INODE_BLOCKS(blocks);
  block.super.ninodes = 0;

  disk_write(0,block.data);

  memset(block.pointers,0,sizeof(block.pointers));

  for(int i=1;i<blocks;i++)
    disk_write(i,block.data);
  
  return 1;
}

int fs_mount(char **args){
  union fs_block block;

  int inode_blocks = NUM_INODE_BLOCKS(disk_size());
  int inode_magic_number = ((DISK_BLOCK_SIZE * inode_blocks)/32);
  //printf("magic number: %d\n",inode_magic_number);
  
  if(bitmap == NULL)
    bitmap = (int*)calloc(inode_magic_number,sizeof(int));

  if(bitmap == NULL)
    return 0;
 
  for(int i=0;i<inode_blocks;i++){
    disk_read(i+1,block.data);
    for(int j=0;j<128;j++){
      if(block.inode[j].isvalid)
	bitmap[(i*128)+j] = 1;
      else
	bitmap[(i*128)+j] = 0;
    }
  }
  return 1;
}

int fs_unmount(char **args){
  if(!(bitmap == NULL)){
    free(bitmap);
    return 1;
  }
  return 0;
}

// Create an empty file
// Returns newly allocated inode number (>= 0) on success and -1 on failure
int fs_create(char **args){
  if(bitmap == NULL){
    printf("bitmap is NULL\n");
    return -1;
  }

  union fs_block block;
  
  int inode_block_num = NUM_INODE_BLOCKS(disk_size());
  
  for(int i=0;i<inode_block_num;i++){
    for(int j=0;j<128;j++){
      if(bitmap[(i*128)+j] == 0){
	bitmap[(i*128)+j] = 1;
	disk_read(i+1,block.data);
	block.inode[j].isvalid = 1;
	block.inode[j].size = 0;
	disk_write(i+1,block.data);
	disk_read(0,block.data);
	block.super.ninodes++;
	disk_write(0,block.data);
	return j;
      }
    }
  }
  
  return -1;
}

int fs_delete(char **args) { // int inumber ) {
  return 0;
}

int fs_getsize(char **args) { // int inumber )
  return -1;
}

int fs_read( int inumber, char *data, int length, int offset ) {
  return 0;
}

int fs_write( int inumber, const char *data, int length, int offset ) {
  return 0;
}
