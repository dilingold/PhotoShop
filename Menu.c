#include <stdio.h>
#include <string.h>

char inputImage[50];
char task[300];
char outputImage[50];
char plevel[4];

void DisplayMenu(char *command)
{
	int selection;
	
	printf("\nCurrent Command:\n%s\n\nWhat would you like to do?\n\n", command);
	printf("1.Specify input image\n");
	printf("--------------------------------\n");
	printf("2.Negate (invert values)\n");
	printf("3.Posterise\n");
	printf("--------------------------------\n");
	printf("8.Specify output image\n");
	printf("9.Reset command\n");
	printf("0.Exit\n\n");
	
	scanf("%d", &selection);
	
	MenuSwitch(selection, command);
}

int MenuSwitch(int selection, char *command)
{
	
	switch (selection)
	{
		case 1:
					printf("Enter input image filename: ");
					scanf("%s", inputImage);
					strcat(inputImage, " |");
					strcpy(command, inputImage);
					strcat(command, task);
					strcat(command, outputImage);
					DisplayMenu(command);
					break;
		case 2:
					strcat(task, "Negate | ");
					strcpy(command, inputImage);
					strcat(command, task);
					strcat(command, outputImage);
					DisplayMenu(command);
					break;
		case 3:
					printf("How many levels? (Enter 2 or more): ");
					scanf("%s", plevel);
					strcat(task, "Posterise ");
					strcat(task, plevel);
					strcat(task, " | ");
					strcpy(command, inputImage);
					strcat(command, task);
					strcat(command, outputImage);
					DisplayMenu(command);
					break;
		case 8:
					printf("Enter output image filename: ");
					scanf("%s", outputImage);
					strcpy(command, inputImage);
					strcat(command, task);
					strcat(command, outputImage);
					DisplayMenu(command);
					break;
		case 9:
					strcpy(inputImage, "cat defaultIn.pnm |");
					strcpy(task, " ");
					strcpy(outputImage, "convert - defaultOut.png");
					strcpy(command, inputImage);
					strcat(command, task);
					strcat(command, outputImage);
					DisplayMenu(command);
					break;
		case 0:
					return 0;
					
	}
	return 0;
}

int main()
{
	char command[400];
	strcpy(inputImage, "cat defaultIn.pnm |");
	strcpy(task, " ");
	strcpy(outputImage, "convert - defaultOut.png");
	strcat(command, inputImage);
	strcat(command, task);
	strcat(command, outputImage);
	DisplayMenu(command);
	return 0;
}