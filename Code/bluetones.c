#include <stdio.h>
#include <stdlib.h>
#define MAX_VALUE 255
#define MIN_VALUE 0

int main() {
    
    FILE * f1 = fopen("RefImg.bmp", "r"); //Input File name
    FILE * f2 = fopen("Bluetone.bmp", "w+"); //Output File name

    int i, j, r, g, b, r1, g1, b1;
    unsigned char header[54];

    if (f1 == NULL) // check if the input file present
    {
        printf("File does not exist.\n");
	exit(0);
    }
	
    for (i = 0; i < 54; i++) //read the 54 byte header of input file
    {
        header[i] = getc(f1);
    }

    //writing the header back as it is
    fwrite(header, sizeof(unsigned char), 54, f2); 

    // extract image height, width from Header 
    int height = * (int * ) & header[22];
    int width = * (int * ) & header[18];
   
    printf("width: %d\n", width);
    printf("height: %d\n", height);

    unsigned char buffer[3][height][width]; //to store the image data

    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            r = 0;
            g = 0;
            b = 0;
	    r1 = buffer[0][i][j] = getc(f1); //red
            g1 = buffer[1][i][j] = getc(f1); //green
            b1 = buffer[2][i][j] = getc(f1); //blue
           

            //conversion formula of rgb to negative
            r = (buffer[0][i][j]*0.393)+(buffer[0][i][j]*0.769)+(buffer[0][i][j]*0.189);
            g = (buffer[0][i][j]*0.349)+(buffer[0][i][j]*0.686)+(buffer[0][i][j]*0.168);
            b = (buffer[0][i][j]*0.272)+(buffer[0][i][j]*0.534)+(buffer[0][i][j]*0.131);
	    
  	   
		if(r > MAX_VALUE){											//if value exceeds
			r = MAX_VALUE;
		}
		if(g > MAX_VALUE){
			g = MAX_VALUE;
		}
		if(b > MAX_VALUE){
			b = MAX_VALUE;
		}
            putc(r, f2);
            putc(g, f2);
            putc(b, f2);
        }
    }

    fclose(f1);
    fclose(f2);

    return 0;
}
