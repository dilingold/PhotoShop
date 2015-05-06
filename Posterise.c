/*I was able to use many of my own pictures with this program.
I had a few issues when working with my own pictures:
My original code used while(!FEOF) which became a problem
when counting the pixels - they did not match up with the pix_count
from the header. I output the file through this program to another PNM file
to see what the problem was saw that FEOF was causing an extra pixel to be 
printed at the bottom (because it goes through the loop one more time after the
EOF.
Also, the new line character was being converted into zeros when I turned tokens
into ints. I had to weed them out.
The only problem I continue to have is that I was not able to reset the file pointer.
I went through the file once to validate the pixels and header, and then couldn't 
work out how to go through again to copy the pixels. So each function works on its own,
but unfortunately not together. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int pix_count;
int max_val;

int posterise (int level, int pix_value)
{
	int pos_value;
	int range;
	
	range = 255/level;
	pos_value = ((pix_value/range) + 1) * range;
	return pos_value;
}

/*validate the header values and determine max_val - maximum pixel value
and pix_count - the number of pixels set in header */
int validate (int num, char *line)
{
	char *token;
	int value;
	switch (num)
	{
		case 1:
				if (strcmp(line, "P2\n") == 0)
				{
					return 1;
				}
				else
				{
					fprintf(stderr, "Header must be P2\n");
					return 0;
				}
				
		case 2:
				pix_count = 1;
				token = strtok(line, " ");
				while(token != NULL)
				{
					value = atoi(token);
					pix_count = pix_count * value;
					token = strtok(NULL, " ");
					
				}
				return 1;
				
		case 3:
				max_val = atoi(line);
				if (max_val > 255)
				{
					fprintf(stderr, "Max pixel value in header is greater than 255\n");
					return 0;
				}
				return 1;
	}
	return 1;
}
/*go through the pnm file and checks that the pix_count and max_val from the header
are correct */
int check(FILE *fp)
{
	int i;
	int j;
	int count;
	int valid_value;
	int value;
	char line[100];
	char *token;
	
	i = 1;
	count = 0;
	while(fgets(line, 100, fp))
	{
	/*send the first three lines (the header) to the validate function */
		if (i < 4)
		{
			valid_value = validate(i, line);
			if (valid_value < 1)
			{
				return 0;
			}
			i++;
		}
		else
		{
			j = 0;
			token = strtok(line, " ");
			while (token != NULL)
			{
			/*remove the new line character before casting token
			into an int */
				if (strcmp(token, "\n") == 0)
					{
						break;
					}
				value = atoi(token);
				/*check that each pixel value does not exceed the max_val
				set in the header */
				if (value > max_val)
				{
					fprintf(stderr, "there are value(s) out of range\n");
					return 0;
				}
				count++;

				token = strtok(NULL, " ");
			}
		}
	}
	/*check that total counted pixels set the pixel count set in header*/
	if (count != pix_count)
	{
		fprintf(stderr, "The number of pixels: %d does not match the "
			"dimensions noted in the header; %d\n", count, pix_count);
		return 0;
	}
	return 1;
}

int main(int argc, char *argv[])
{
	FILE *fp;
	char line[100];
	char tok_line[100];
	char *token;
	int i;
	int j;
	int valid_value;
	int value;
	int count;
	int level;
	char c[4];
	
	level = atoi(argv[1]);
	
	/*check the input arguments for posterise level */
	if (argc < 1 || level < 1)
	{
		fprintf(stderr, "invalid posterising level\n");
		return 0;
	}
	
	fp = fopen("/dev/stdin","r");
	if(fp == NULL)
	{
		perror("Error opening file");
		return(-1);
	}
	valid_value = check(fp);
	if (valid_value < 1)
	{
		return 0;
	}
	i = 1;
	count = 0;
	
	/*here I could not get the file pointer to reset to the 
	beginning to be read through again. To get this to work,
	comment out the code above
	valid_value = check(fp);
	*/
	while(fgets(line,100,fp))
	{
		/*simply copy the first 3 lines - the header*/
		if (i < 4)
		{
			fprintf(stdout, "%s\n", line);
			i++;
		}
		else
		{
			j = 0;
			token = strtok(line, " ");
			while (token != NULL)
			{
				/*remove the new line character before casting token
				into an int */
				if (strcmp(token, "\n") == 0)
					{
						break;
					}
				value = atoi(token);

				/*posterise each pixel */
				value = posterise(level, value);
				sprintf(c, "%d", value);
				token = c;
				count++;
				if (j < 1)
				{
					strcpy(tok_line, token);
					j++;
					token = strtok(NULL, " ");		
				}
				else
				{
					strcat (tok_line, " ");
					strcat (tok_line, token);
					token = strtok(NULL, " ");
				}
			}
			/*print each posterised line of pixels to stdout */
			fprintf(stdout, "%s\n", tok_line);
		}
	}
	fclose(fp);
	return 0;
}