/****************************************************************************
 *                                                                          *
 * File    : main.c                                                         *
 *                                                                          *
 * Purpose : Console mode (command line) program.                           *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/
//#if defined(UNICODE) && !defined(_UNICODE)
//    #define _UNICODE
//#elif defined(_UNICODE) && !defined(UNICODE)
//    #define UNICODE
//#endif

#define _CRT_RAND_S
#include <stdlib.h>
#include <stdio.h>

#define MAX_COLS			60
#define	MIN_COLS			10
#define	MAX_ROWS			12

struct Header {
	unsigned char rows;
	unsigned char cols;
};


int rowscount = MAX_ROWS, colscount = 12;
unsigned char * array_main[MAX_ROWS];
void init_array(void);
void doshuffle(void);
void shuffle(unsigned char *, int);
void show(void);
void onexit(void);
void savetofile(void);
void loadfromfile(void);

int main(int argc, char *argv[])
{
	printf("Hello, world!\n");
	init_array();
	show();
	doshuffle();
	show();
	printf("\n----------------------\n");
	savetofile();
	FILE *pf;
	pf = fopen("record.bin", "rb");
	struct Header head;
	int size = colscount * sizeof(char);
	fread(&head, sizeof(head), 1, pf);
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < MAX_ROWS; j++)
			fread(array_main[j], size, 1, pf);
		show();
	}

	fclose(pf);
	//loadfromfile();
	//show();
	onexit();	
	return 0;
}

void init_array(void)
{
	for (int i = 0; i < MAX_ROWS; i++)
	{
		array_main[i] = (unsigned char *)calloc(colscount, sizeof(unsigned char));
		for (int j = 0; j < colscount; j++)
		{
			array_main[i][j] = j;
		}
	}

}

void doshuffle(void)
{
	for (int i = 0; i < MAX_ROWS; i++)
	{
		//array_main[i] = (unsigned char *)calloc(colscount, sizeof(unsigned char));
		shuffle(array_main[i],colscount);
	}
}

void shuffle(unsigned char * arr, int N)
{

 // инициализация генератора случайных чисел
    //srand(time(NULL));
	//
	unsigned int randval;
	//int randval;
	rand_s(&randval);
	//unsigned int val = randval;
	 //printf("%u_%d\n", randval, i );
  if (N < 1) return;
    // реализация алгоритма перестановки
    for (int i = N - 1; i >= 1; i--)
    {
        //int j = rand() % (i + 1);
 				int j = randval % (i + 1);
        unsigned char tmp = arr[j];
        arr[j] = arr[i];
        arr[i] = tmp;
				rand_s(&randval);
    }
}

void show(void)
{
	for (int i = 0; i < MAX_ROWS; i++)
	{
		//array_main[i] = (unsigned char *)calloc(colscount, sizeof(unsigned char));
		for (int j = 0; j < colscount; j++)
		{
			printf("%3u", array_main[i][j]);			
		}
		printf("\n");
	}
}

void onexit(void)
{
	for (int i = 0; i < MAX_ROWS; i++)
		free(array_main[i]);
}

void savetofile(void)
{
	FILE * pf;
	pf = fopen("record.bin","wb");
	struct Header head;
	head.cols = colscount;
	head.rows = MAX_ROWS;
	fwrite(&head, sizeof(head), 1, pf);
	//if (pf == NULL) return;
	//doshuffle();
	int size = colscount * sizeof(char);
	for(int i = 0; i < 10000; i++)
    {
        doshuffle();
		for(int j = 0; j < MAX_ROWS; j++)
			fwrite(array_main[j], size, 1, pf);
    }
	fclose(pf);
}

void loadfromfile(void)
{
	FILE *pf;
	pf = fopen("record.bin", "rb");
	struct Header head;
	fread(&head, sizeof(head), 1, pf);
    //printf()
	fread(array_main, sizeof(array_main), 1, pf);
	fclose(pf);
}
