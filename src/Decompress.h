#ifndef _Decompress_h
#define _Decompress_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXSIZE 256
typedef struct heapstruct* Heap;
typedef struct TreeNode* HuffTree;
typedef struct threebitreadstruct* ThreeBitRead; 
struct heapstruct{
	HuffTree* data;
	int maxsize;
	int position;
};
struct TreeNode{
	int frequency;
	unsigned char code;
	HuffTree Left;
	HuffTree Right;
};
struct threebitreadstruct{
	unsigned char store1;
	unsigned char store2;
	unsigned char store3;
	int index;
	int stop;
};

ThreeBitRead Init_write_item(void);
Heap Init_heap(int len);
void Insert_heap(HuffTree t,Heap heap);
HuffTree Pop(Heap heap);
void swap(HuffTree* a,HuffTree* b);
HuffTree Read_tree(FILE* fp);
HuffTree BuildTree(HuffTree a,HuffTree b);
void Put_code(HuffTree tree,ThreeBitRead write_item,FILE* wfp,FILE* fp);

#endif