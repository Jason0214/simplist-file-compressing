#include "compress.h"
void Compress(void){
	int i,j;
	int flag;
	Heap heap;
	List ptr; //used for link-list traversal
	char document_name[100]={0};
	char doc_compressed_name[100]={0};
	char* tail = ".jas";
	char  extension_name[10]={0};
	unsigned char cc;
	unsigned int freq=0;
	
	system("cls");
	printf("*****************************************************************************\n");
	printf("* Type the document name (included the extension name) you want to compress *\n");
	printf("* make sure there is no more than one '.' in the filename                   *\n");
	printf("* End with key 'enter'                                                      *\n");
	printf("*****************************************************************************\n");
	scanf("%s",document_name);
	FILE *fp;
	if((fp = fopen(document_name,"rb")) == NULL){
		printf("open failed\n");
		getchar();
		getchar();
		return;		
	} 
	//generate the name of the compressed file 
	for(i = 0;document_name[i] != '.'; i++){
		doc_compressed_name[i] = document_name[i];
	}
	extension_name[0] = '['; 
	for(i++ ,j = 1;document_name[i] != 0;i++,j++){
		extension_name[j] = document_name[i];
	}
	extension_name[j] = ']';
	strcat(doc_compressed_name,extension_name);
	strcat(doc_compressed_name,tail);
	
	// init data struct
	List_head head = (List_head)malloc(sizeof(struct headstruct));
	head->list = NULL;
	head->num = 0;
	heap = Init_heap(MAXSIZE);
	HuffTree Tree = NULL;
	
	//main processing 
	cc = fgetc(fp);
	while(feof(fp) == 0){
		Insert_item(cc,head); //insert code to link-list ans btw sum the frequency
		cc = fgetc(fp);
	} 
	// build heap
	for(ptr = head->list;ptr != NULL;ptr = ptr->next){
		HuffTree t = (HuffTree)malloc(sizeof(struct TreeNode));
		t->frequency = ptr->frequency;
		t->Left = NULL;
		t->Right = NULL;
		t->code = ptr->origin;
		Insert_heap(t,heap);
	}
	//free link list
	Free_list(head);

	//open object file
	//write current HuffTree into the file
	FILE* wfp = fopen(doc_compressed_name,"wb");
	fputc(heap->position,wfp);
	for(i = 1;i <= heap->position;i++){
		fputc(heap->data[i]->code,wfp);
		freq = heap->data[i]->frequency;
		cc = freq >> 24;
		fputc(cc,wfp);
		cc = freq >> 16;
		fputc(cc,wfp);
		cc = freq >> 8;
		fputc(cc,wfp);
		cc = freq;
		fputc(cc,wfp); 
	}

	//build tree 
	while(heap->position > 1){
		Insert_heap(BuildTree(Pop(heap),Pop(heap)),heap);
	}

	//define variable
	Tree = Pop(heap);	
	char temp[40]={0};
	BitRead read_item = Init_read_item();
	
//	FILE* test = fopen("HUFFMAN CODE.txt","w");  
//	char temp_[40]={0}; 
	
	fseek(fp,0,SEEK_END);
	fseek(fp,0,SEEK_SET);
	flag =1;
	cc = fgetc(fp);
	while(feof(fp) == 0){
		memset(temp,'\0',sizeof(char)*40);
		if(BackHuffCode(cc,Tree,temp) != 1){
			printf("Problem detected.\n");
			flag = 0;
			break;
		}
		
//		memset(temp_,'\0',sizeof(char)*40);
//		for(i =0;i < strlen(temp);i++){
//			temp_[i] = temp[strlen(temp)-1- i];
//		}
//		fputc(cc,test);
//		fprintf(test,"   ");
//		fprintf(test,temp_);
//		fprintf(test,"\r\n");		
		
		cc = fgetc(fp);
		Write(temp,read_item,wfp);
	}
	fputc(read_item->store,wfp);	
	// tell the decompression how many bits have been used in the last byte(maybe uncompleted ) 	
	fputc(read_item->index,wfp);
	//	printf("%d",read_item->index);

	if(flag == 1)
		printf("Compress Finished.\n");
	fclose(fp);
	fclose(wfp);
	
//	fclose(test);
	
	getchar();
	getchar();
}
Heap Init_heap(int len){
	Heap heap = (Heap)malloc(sizeof(struct heapstruct));
	heap->data = (HuffTree*)malloc(sizeof(struct TreeNode) * len);
	heap->maxsize = len;
	heap->position = 0;
	heap->data[0] = (HuffTree)malloc(sizeof(struct TreeNode));
	heap->data[0]->frequency = -1;
	return heap;
}
void Insert_heap(HuffTree t,Heap heap){
	if(heap->position == heap->maxsize)
		printf("heap segmentation fault!");
	else{
		heap->data[++heap->position] = t;
		int i;
		for(i = heap->position; i > 0 ; i/=2 ){
			if(heap->data[i]->frequency < heap->data[i/2]->frequency)
				swap(&heap->data[i],&heap->data[i/2]);
		}
	}
}
HuffTree Pop(Heap heap){
	if(heap->position == 0){
		printf("heap pop failed\n");
		return NULL;
	}
	else{
		HuffTree min = heap->data[1];
		HuffTree last = heap->data[heap->position--];
		int i;
		int child;
		for(i = 1;i*2 < heap->position; i = child){
			child = i*2;
			if(child != heap->position && heap->data[child]->frequency > heap->data[child+1]->frequency)
				child++;
			if(heap->data[child]->frequency < last->frequency)
				heap->data[i] = heap->data[child];
			else
				break;
		}
		heap->data[i] = last;
		return min;		
	}
}

HuffTree BuildTree(HuffTree a,HuffTree b){
	HuffTree T = (HuffTree)malloc(sizeof(struct TreeNode));
	T->Left = a;
	T->Right = b;
	T->code = 0;
	T->frequency = a->frequency + b->frequency;
	return T;
}
HuffTree Read_tree(FILE* fp){
	unsigned char c;
	unsigned int frequency = 0;
	HuffTree tree = (HuffTree)malloc(sizeof(struct TreeNode));
	c = fgetc(fp);
	tree->code = c;
	c = fgetc(fp);
	frequency += (c << 24);
	c = fgetc(fp);
	frequency += (c << 16);
	c = fgetc(fp);
	frequency += (c << 8);
	c = fgetc(fp);
	frequency += c;
	tree->frequency = frequency;
	tree->Left = NULL;
	tree->Right = NULL;
	return tree;
}

void swap(HuffTree* a,HuffTree* b){
	HuffTree temp;
	temp = *a;
	*a = *b;
	*b = temp;
}

BitRead Init_read_item(void){
	BitRead ret = (BitRead)malloc(sizeof(struct bitreadstruct));
	ret->index = 7;
	ret->store = 0;
	return ret;
}

void Insert_item(unsigned char code,List_head head){
	List ptr;
	List last = head->list;
	List p = (List)malloc(sizeof(struct liststruct));
	int flag = 0;
	for(ptr = head->list; ptr != NULL ;ptr = ptr->next){
		if(ptr->origin == code){
			ptr->frequency++;
			flag = 1;
		}
	}
	if(flag == 0){
		head->num++;
		p->frequency = 1;
		p->origin = code;
		p->next = NULL;
		if(last == NULL){
			head->list = p;
		}
		else{
			while(last->next != NULL)
				last = last->next;
			last->next = p;			
		}
	}
}


void Free_list(List_head head){
	List q;
	List p = head->list;
	if(p != NULL){
		do{
			q = p->next;
			free(p);
			p = q; 
		}while(p);		
	}
	free(head);
}

int BackHuffCode(unsigned char code,HuffTree T,char* temp){
	int ret = 0;
	if(T->Left == NULL || T->Right == NULL){
		if(T->code == code)
			ret = 1;
		else
			ret = 0;
	}
	else{
		if(BackHuffCode(code,T->Left,temp) == 1){
			strcat(temp,"0");
			ret = 1;
		}
		else if(BackHuffCode(code,T->Right,temp) == 1){
			strcat(temp,"1");
			ret = 1;
		}
	}
	return ret;
}

void Write(char* temp,BitRead read_item,FILE* wfp){
	int i;
	for(i = 0; temp[i] != 0;i++){
		if(temp[strlen(temp)-1-i] == '1')
			read_item->store |= (1 << read_item->index);	
		read_item->index--;
		if(read_item->index < 0){
			fputc(read_item->store,wfp);
			read_item->store = 0;
			read_item->index = 7;
		}		
	}
}

