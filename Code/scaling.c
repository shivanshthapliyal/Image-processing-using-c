#include <stdio.h>
#include <time.h>
#include <math.h>
#include<stdlib.h>
#define MAX_VALUE 255

float* alloc1df (int n)
{
    int i;
    float* array;
    if ((array = (float*) malloc(n * sizeof(float))) == NULL) {
        printf("Unable to allocate memory for 1D float array...\n");
        exit(0);
    }

    for (i = 0; i < n; i++) {
        array[i] = 0.0;
    }

    return array;
}

float** alloc2df (int m, int n)
{
    int i;
    float** array;
    if ((array = (float **) malloc(m * sizeof(float*))) == NULL) {
        printf("Unable to allocate memory for 2D float array...\n");
        exit(0);
    }

    for (i = 0; i < m; i++) {
        array[i] = alloc1df(n);
    }

    return array;
}

float*** alloc3df (int l, int m, int n)
{
    int i;
    float*** array;

    if ((array = (float ***) malloc(l * sizeof(float**))) == NULL) {
        printf("Unable to allocate memory for 3D float array...\n");
        exit(0);
    }

    for (i = 0; i < l; i++) {
        array[i] = alloc2df(m,n);
    }

    return array;
}

float bilinearly_interpolate (int top, int bottom, int left, int right,float horizontal_position, float vertical_position, float **input)
{
   
    float top_left = input[top][left];
    float top_right = input[top][right];
    float bottom_left = input[bottom][left];
    float bottom_right = input[bottom][right];

//printf("%d\t%d\n",input[top][left],input[bottom][right]);

   
    float horizontal_progress = horizontal_position - (float) left;
    float vertical_progress = vertical_position - (float) top;

  
    float top_block = top_left + horizontal_progress* (top_right - top_left);
    float bottom_block = bottom_left + horizontal_progress * (bottom_right - bottom_left);

    
//printf("%f\n",top_block + vertical_progress * (bottom_block - top_block));
    return top_block + vertical_progress * (bottom_block - top_block);
	
}

int main()
{

	FILE *f1 = fopen("RefImg.bmp","r");		
	FILE *f2 = fopen("Scaled.bmp","w+");		  

	int i,j,k,r,g,b,r1,g1,b1;
	unsigned char header[54];
	
	if(f1==NULL)				
	{											
		printf("File does not exist.\n");
	}

	for(i=0;i<54;i++)	
	{									
		header[i] = getc(f1);								
	}

	//fwrite(header,sizeof(unsigned char),54,f2);		
	//fwrite(header,sizeof(unsigned char),17,f2);

	
	int width = *(int*)&header[18];
	int height = *(int*)&header[22];
	int bitDepth = *(int*)&header[28];
	int *wt=(int*)&header[18];
	int *ht=(int*)&header[22];
	//printf("%d\t%d\n",&header[18],&header[22]);
	
	//printf("width: %d\n",width);
	//printf("height: %d\n",height );

	int size = height*width;

	float verti_resize_factor, horiz_resize_factor;
    	printf("By what factor do you want to scale your image horizontally?\n");
   	scanf("%f", &horiz_resize_factor);
    	printf("By what factor do you want to scale your image vertically?\n");
   	scanf("%f", &verti_resize_factor);
	printf("Resizing...\n");

    // Figure out the dimensions of the new, resized image and def1e the new array.
   int new_height  = height * verti_resize_factor;
    int new_width =  width * horiz_resize_factor;
    *ht=new_height;
    *wt=new_width;
//printf("%d\t%d\n",*ht,*wt);
//printf("%d\t%d\n",ht,wt);
    fwrite(header,sizeof(unsigned char),54,f2);
    float *** output = alloc3df(3, *ht, *wt);			
    float*** buffer = alloc3df(3, height, width);
	// unsigned char buffer[3][height][width];
	//to store the image data
	//printf("v");
	for(i=0;i<height;i++){
	for(j=0;j<width;j++){
	
		buffer[2][i][j] =  getc(f1);									//blue
		buffer[1][i][j]=  getc(f1);									//green
		buffer[0][i][j]= getc(f1);									//red



		if(r > MAX_VALUE){	//if value exceeds
			r = MAX_VALUE;
		}
		if(g > MAX_VALUE){
			g = MAX_VALUE;
		}
		if(b > MAX_VALUE){
			b = MAX_VALUE;
		}
		//putc(b,f2);
		//putc(g,f2);
		//putc(r,f2);
		
	}}

 for (i = 0; i < new_height; ++i) {
        for (j = 0; j < new_width; ++j) {
            float vertical_position = i * ( (float) height / new_height);
            float horizontal_position = j * ( (float) width / new_width);
	//printf("%d\t%f\n",i,vertical_position);
	//printf("%d\t%f\n\n",j,horizontal_position);
            int top = (vertical_position); //floor
            int bottom = top + 1;
            int left = (horizontal_position);//floor
            int right = left + 1;

            // Check if any of the four locations are invalid. If they are,simply access the valid adjacent pixel.
            if (bottom >= height) {
                bottom = top;
            }
            if (right >= width) {
                right = left;
            }

            // Interpolate the pixel according to the dimension set above and set the resulting pixel. Do so for each color.
            for (k = 0; k < 3; k++) {
                float interpolated = bilinearly_interpolate(top, bottom, left,right, horizontal_position, vertical_position,  buffer[k]);
                output[k][i][j] = interpolated;
		//putc(output[k][i][j],f2);
            }

        }
    }

for(i=0;i<new_height;i++){
	for(j=0;j<new_width;j++){
	putc(output[2][i][j],f2);
	putc(output[1][i][j],f2);
	putc(output[0][i][j],f2);
}
	}
		
	fclose(f2);
	fclose(f1);
	printf("Resizing Done\n");
	return 0;
}
