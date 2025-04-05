#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "utils.h"

Image *read_PPM(char *filename)
{
    /* opens a PPM file, constructs an Image object and returns a pointer to it.
    Use fopen(), fscanf(), fprintf(), and fclose().*/
    char extension[10];
    int height, width, colour, i, j, k;

    FILE *fp=fopen(filename,"r");
    if(fp==NULL)
        exit(-1);

    fscanf(fp,"%*s%d %d\n%*d\n",&width,&height);

    Image *image = (Image*)malloc(sizeof(Image));

    image->rows=height;
    image->cols=width;

    image->image=(Color**)malloc(height*width*sizeof(Color*));
    for(i=0;i<height;i++){
        *((image->image)+i)=(Color*)malloc(width*sizeof(Color));
    }

    for(i=0;i<height;i++)
        for(j=0;j<width;j++)
            fscanf(fp,"%d%d%d",&((*(*((image->image)+i)+j)).R),&((*(*((image->image)+i)+j)).G),&((*(*((image->image)+i)+j)).B));

    fclose(fp);
    return image;
}


void write_PPM(Image *image, char *filename)
{
	/* takes an Image object and writes to filename in PPM format.*/
	char extension[10];
    int height, width, colour, i, j, k;

    FILE *fp=fopen(filename,"r");
    if(fp==NULL)
        exit(-1);
    fscanf(fp,"%s%d %d\n%d\n",&extension,&width,&height,&colour);
    fclose(fp);

    FILE *fop=fopen("Deciphered.ppm","w");
    fprintf(fop,"%s\n%d %d\n%d\n",extension,width,height,colour);

	for(i=0;i<height;i++){
        fputc('\n',fop);

        for(j=0;j<width;j++){
            fprintf(fop,"%3d %3d %3d",(*(image->image+i)+j)->R,(*(image->image+i)+j)->G,(*(image->image+i)+j)->B);

            if(j!=width-1)
                fputs("   ",fop);
        }
    }

}


void free_image(Image *image)
{
	/* takes an Image object and frees all the memory associated with it.
    This involves not only calling free on image but also on the appropriate
    members of it.
    */
    int i, j;

    for(i=0;i<image->rows;i++)
        free(*(image->image+i));

    free(image->image);
    free(image);
    printf("\nThe process was successful\n");
}


Color *evaluate_one_pixel(Image *image, int row, int col)
{
	/* Takes an Image object and returns what color the pixel at the given row/col
    should be in the secret image. This function should not change image*/
    Color *color=(Color*)malloc(sizeof(Color));

    color->R=color->G=color->B=((*(image->image+row)+col)->B)%2?0:255;

    return color;
}

Image *get_secret_image(Image *image)
{
	/* takes an Image object, and constructs the secret image from it by extracting
    the LSB of the B channel. You should call evaluate_one_pixel() here. */
    int i,j;
    Color *temp;

    for(i=0;i<image->rows;i++)

        for(j=0;j<image->cols;j++){

            temp=evaluate_one_pixel(image, i, j);
            *(*(image->image+i)+j)=*temp;
            free(temp);
        }
    return image;
}

int main()
{
    /* Call read_PPM(), write_PPM(), free_image(), get_secret_image() in some order
    to obtain the hidden message.*/
    char filename[1000];
    printf("Enter file name: ");
    scanf("%s",&filename);

    Image *image=read_PPM(filename);
    get_secret_image(image);
    write_PPM(image, filename);
    free_image(image);

	return 0;
}


