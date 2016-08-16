#ifndef _Compress_h
#define _Compress_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXSIZE 256
typedef struct headstruct* List_head;
typedef struct liststruct* List;
typedef struct heapstruct* Heap;
typedef struct TreeNode* HuffTree;
typedef struct bitreadstruct* BitRead;;
struct headstruct{
	List list;
	int num;
};
struct liststruct{
	int frequency;
	unsigned char origin;
	List next;
};
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
struct bitreadstruct{
	unsigned char store;
	int index;
};

void Insert_item(unsigned char code,List_head head);
void Free_list(List_head head);
BitRead Init_read_item(void);
int BackHuffCode(unsigned char code,HuffTree T,char* temp);
void Write(char* temp,BitRead read_item,FILE* wfp);
Heap Init_heap(int len);
void Insert_heap(HuffTree t,Heap heap);
HuffTree Pop(Heap heap);
void swap(HuffTree* a,HuffTree* b);
HuffTree Read_tree(FILE* fp);
HuffTree BuildTree(HuffTree a,HuffTree b);

#endif