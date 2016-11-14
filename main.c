#include <stdio.h>
#include <stdlib.h>

void Compress(void);
void Decompress(void);

int main(){
	int choice = 0;
	do{
		system("cls");
		printf("**********************************************************\n");
		printf("* Item List:                                             *\n");
		printf("* 1.Compress a file                                      *\n");
		printf("* 2.Decompress a file                                    *\n");
		printf("* 3.Exit                                                 *\n");
		printf("* Type the number before the item to enter.              *\n");
		printf("**********************************************************\n");
		scanf("%d",&choice);
		if(choice == 1)
			Compress();
		else if (choice == 2)
			Decompress();
		else 
			break;
	}while(1);
	return 0;
}
