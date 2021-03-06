

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
// file reading...

#pragma pack(push,1)
typedef struct header
{
	unsigned short signature;		// 2 bytes should be 'BM'
	unsigned long file_size;		// 4 bytes
	unsigned long reserved;			// 4 bytes
	unsigned long data_offset;		//4 bytes, offset from the beginning of the
									// bitmap data...
} header;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct infoheader
{
	unsigned long size;				//4 bytes, size of infoheader = 40
	unsigned long width;			// horizontal width of pixels
	unsigned long height;			// vertical width of pixels
	unsigned short planes;			// #planes = 1
	unsigned short bits_per_pixel;  //need a 8 bit pallet
	unsigned long compression;		// working with no compression...
	unsigned long image_size;		//valid to set to 0 if compression =0
	unsigned long x_pixel_per_M;	
	unsigned long y_pixel_per_M;
	unsigned long colors_used;		//should be 0x100 or 256
	unsigned long important_colors;	//0 = all

}infoheader;
#pragma pack(pop)

typedef struct color_table
{
	int size;
	int padding_size;
	unsigned char *red;
	unsigned char *green;
	unsigned char *blue;
	unsigned char *padding;
}color_table;

int main()
{	

	//SetConsoleDisplayMode(GetStdHandle(STD_OUTPUT_HANDLE), CONSOLE_FULLSCREEN_MODE, 0);


	header *my_header = (header*)malloc(sizeof(header));
	infoheader *my_info_header = (infoheader*)malloc(sizeof(infoheader));

	color_table *my_color_data = (color_table*)malloc(sizeof(color_table));

	my_header->signature = 0;
	my_header->file_size = 0;
	size_t result;
	FILE *fileptr = fopen("darwin.bmp", "r");

	rewind(fileptr);

	result = fread(my_header, sizeof(char),14, fileptr);
	printf("Header Size: \t\t\t%d\n", result);
	printf("Signature: \t\t\t%hx\n", my_header->signature);	
	printf("File Size: \t\t\t%lx\n", my_header->file_size);
	printf("Reserved: \t\t\t%lx\n", my_header->reserved);
	printf("Data Offset: \t\t\t%lx\n\n", my_header->data_offset);


	result = fread(my_info_header, sizeof(char), 40, fileptr);
	printf("Header Info Size: \t\t%d\n", result);
	printf("Size: \t\t\t\t%hx\n", my_info_header->size);
	printf("Width: \t\t\t\t%hx\n", my_info_header->width);
	printf("Height: \t\t\t%hx\n", my_info_header->height);
	printf("Planes: \t\t\t%hx\n", my_info_header->planes);
	printf("Bits per pixel: \t\t%hx\n", my_info_header->bits_per_pixel);
	printf("Compression: \t\t\t%hx\n", my_info_header->compression);
	printf("Image Size: \t\t\t%hx\n", my_info_header->image_size);
	printf("X pixel per Meter: \t\t%hx\n", my_info_header->x_pixel_per_M);
	printf("Y pixel per Meter: \t\t%hx\n", my_info_header->y_pixel_per_M);
	printf("Colors Used: \t\t\t%hx\n", my_info_header->colors_used);
	printf("Important Colors: \t\t%hx\n\n", my_info_header->important_colors);


	//init is done, we can malloc the color table data;

	fseek(fileptr, my_header->data_offset, SEEK_SET);

	my_color_data->size = my_info_header->height*my_info_header->width;
	
	my_color_data->red = (unsigned char *)malloc(my_color_data->size*sizeof(unsigned char) + 1 * (sizeof(unsigned char)));
	my_color_data->red[my_color_data->size * sizeof(unsigned char)] = '\0';

	my_color_data->blue = (unsigned char *)malloc(my_color_data->size * sizeof(unsigned char) + 1 * (sizeof(unsigned char)));
	my_color_data->blue[my_color_data->size * sizeof(unsigned char)] = '\0';

	my_color_data->green = (unsigned char *)malloc(my_color_data->size * sizeof(unsigned char) + 1 * (sizeof(unsigned char)));
	my_color_data->green[my_color_data->size * sizeof(unsigned char)] = '\0';

	if (my_info_header->width * 3 % 4 == 3){my_color_data->padding_size = 1;	}
	else if (my_info_header->width * 3 % 4 == 2){my_color_data->padding_size = 2;}
	else if (my_info_header->width * 3 % 4 == 1){my_color_data->padding_size = 3;}
	else{my_color_data->padding_size = 0;}

	my_color_data->padding = (unsigned char *)malloc(my_color_data->padding_size * (sizeof(unsigned char)));

	unsigned char *temp = (unsigned char *)malloc(3 + my_color_data->padding_size);

	int count = 0;
	int low =0;
	int med=0;
	int high=0;

	for (int i = my_info_header->height - 1; i >= 0; i--)
	{
		for (int j = 0; j < my_info_header->width; j++)
		{
			result = fread(temp, sizeof(unsigned char), 3, fileptr);
			my_color_data->blue[count] = temp[0];
			my_color_data->green[count] = temp[1];
			my_color_data->red[count] = temp[2];
			// padding gets lost??
			//printf("Red[%d] = %d\n", count, my_color_data->red[count]);
			//printf("Green[%d] = %d\n", count, my_color_data->green[count]);
			//printf("Blue[%d] = %d\n", count, my_color_data->blue[count]);
			//printf("---------\n");
			

			count++;
		}
		result = fread(my_color_data->padding, sizeof(unsigned char), my_color_data->padding_size, fileptr);
	}
	

	float luminance =0;
	count = (my_info_header->height * my_info_header->width) -1;
	int delta = 0;
	int new_line = 0;
	int some_num = 0;
	for (int i= 2; i >= 2; i--)
	{
		delta = 2;
		while (count >= 0)
		{
			luminance = 0.2126*(float)(my_color_data->red[count]) + 0.7152*(float)(my_color_data->green[count]) + 0.0722*(float)(my_color_data->blue[count]);

			if (count < (my_info_header->height*my_info_header->width - (my_info_header->width*delta)) && count >(my_info_header->width*delta))
			{
				for (int i = 1; i <= delta; i++)
				{
					luminance += 0.2126*(float)(my_color_data->red[count + i]) + 0.7152*(float)(my_color_data->green[count + i]) + 0.0722*(float)(my_color_data->blue[count + i]);
					luminance += 0.2126*(float)(my_color_data->red[count - i]) + 0.7152*(float)(my_color_data->green[count - i]) + 0.0722*(float)(my_color_data->blue[count - i]);
					luminance += 0.2126*(float)(my_color_data->red[count + (i*my_info_header->width)] + 0.7152*(float)(my_color_data->green[count + (i*my_info_header->width)]) + 0.0722*(float)(my_color_data->blue[count + (i*my_info_header->width)]));
					luminance += 0.2126*(float)(my_color_data->red[count + (i*my_info_header->width)] + 0.7152*(float)(my_color_data->green[count + (i*my_info_header->width)]) + 0.0722*(float)(my_color_data->blue[count + (i*my_info_header->width)]));
				}
				if (delta > 0) { luminance = luminance / (1 + (4 * delta)); }
			}


			if (luminance > 225) { printf(" "); }
			else if (luminance > 200) { printf("."); }
			else if (luminance > 175) { printf(":"); }
			else if (luminance > 150) { printf("-"); }
			else if (luminance > 125) { printf("="); }
			else if (luminance > 100) { printf("+"); }
			else if (luminance > 75) { printf("*"); }
			else if (luminance > 50) { printf("#"); }
			else if (luminance > 25) { printf("$"); }
			else { printf("@"); }
			count -= (1+delta);
			new_line += (1+delta); 

			if (new_line >= (my_info_header->width)) { printf("\n"); new_line = 0; count -= (my_info_header->width *(delta+1)); }
		}

		count = (my_info_header->height * my_info_header->width) - 1;
	}

	int c = getchar();
	
	return 0;
}