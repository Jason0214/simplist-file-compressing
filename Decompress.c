#include "Decompress.h"
void Decompress(void){
	system("cls");
	printf("******************************************************************\n");
	printf("*Type the file name you want to decompress(must ended with'.ljc')*\n");
	printf("*end with 'enter'                                                *\n");
	printf("******************************************************************\n");
	char decompress_name[100] = {0};
	int length;
	int i,j;
	Heap heap= Init_heap(MAXSIZE);
	HuffTree t;
	HuffTree Tree;
	char newname[100]={0};
	
	scanf("%s",decompress_name);
	FILE* fp ;
	if((fp = fopen(decompress_name,"rb")) == NULL){
		printf("Open failed\n");
		getchar();
		getchar();
		return ;
	}
	
	//calc the new file name
	for(i = 0;decompress_name[i] != '[';i++){
		newname[i] = decompress_name[i];
	}
	strcat(newname,"_new");
	newname[i+4] = '.';
	for(j=i+5,i++;decompress_name[i] != ']';j++,i++){
		newname[j] = decompress_name[i];
	}
	
	length = fgetc(fp);
	if(length == 0)
		length = 256;
	for(i = 0;i < length;i++){
		t = Read_tree(fp);
		Insert_heap(t,heap);
	}
	while(heap->position > 1){
		Insert_heap(BuildTree(Pop(heap),Pop(heap)),heap);
	}	

	FILE* wfp;
	if((wfp = fopen(newname,"wb")) == NULL){
		printf("Generate new failed\n");
		getchar();
		getchar();
	} 
	Tree = Pop(heap);

	ThreeBitRead write_item = Init_write_item();
	write_item->store1 = fgetc(fp);
	write_item->store2 = fgetc(fp);
	write_item->store3 = fgetc(fp);
	while((write_item->stop == -1 || write_item->index > write_item->stop) && write_item->stop != 8) 
		Put_code(Tree,write_item,wfp,fp);
	fclose(wfp);
	printf("Decompress finished\n");
	getchar();
	getchar();
}


ThreeBitRead Init_write_item(void){
	ThreeBitRead ret = (ThreeBitRead)malloc(sizeof(struct threebitreadstruct));
	ret->store1 = 0;
	ret->store2 = 0;
	ret->index = 7;
	ret->stop = -1;
	return ret;
}

void Put_code(HuffTree tree,ThreeBitRead write_item,FILE* wfp,FILE* fp){
	if(write_item->index < 0 && feof(fp) == 0){
		write_item->index = 7;
		write_item->store1 = write_item->store2;
		write_item->store2 = write_item->store3;
		write_item->store3 = fgetc(fp);
		if(feof(fp))
			write_item->stop = write_item->store2;
	}
	if(tree->Left == NULL || tree->Right ==NULL){
		fputc(tree->code,wfp);
	}
	else if(write_item->stop == -1 || write_item->index > write_item->stop){
		if((write_item->store1 & (1 << write_item->index)) == 0){//this postion is 0
			write_item->index--;
			Put_code(tree->Left,write_item,wfp,fp);
		}
		else{
			write_item->index--;
			Put_code(tree->Right,write_item,wfp,fp);		
		}
	}
}

//three bits read used for the reading of last byte(uncompleted)