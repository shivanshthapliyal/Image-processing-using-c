#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAX_VALUE 255
#define MIN_VALUE 0

int main()
{

    FILE* f1 = fopen("img1.bmp", "r"); // Input File name
    FILE* f2 = fopen("img2.bmp", "w+"); // Output File name

    int i, j, val, val1;
    unsigned char header[54];

    if (f1 == NULL) // check if the input file present
    {
        printf("File does not exist.\n");
        exit(0);
    }

    for (i = 0; i < 54; i++) // read the 54 byte header of input file
    {
        header[i] = getc(f1);
    }

    // writing the header back as it is
    fwrite(header, sizeof(unsigned char), 54, f2);

    // extract image height, width from Header
    int height = *(int*)&header[22];
    int width = *(int*)&header[18];

    printf("width: %d\n", width);
    printf("height: %d\n", height);

    unsigned char buffer[3][height][width]; // to store the image data

    int imghist[256], newhist[256];
	double cumprob[256], prob[256];
    for (i = 0; i < 256; i++)
    {
        imghist[i] = 0;
    }

    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            val = 0;

            val1 = buffer[0][i][j] = getc(f1); // pixel intensity

            // calculate the no of pixels for each intensity values
            imghist[val] = imghist[val] + 1;
        }
    }

    int desiredhisto[256];
    int desiredhistcount[256];
    int total_pixels = 0;
    for (i = 0; i < 256; i++)
    {
        if (i < 129)
        {
            desiredhisto[i] = i;
        }
        else
        {
            desiredhisto[i] = 128 - i;
        }

        total_pixels += desiredhisto[i];
    }

    for (i = 0; i < 256; i++)
    {

        prob[i] = imghist[i] / 256;
    }

    cumprob[0] = imghist[0];
    for (i = 1; i < 256; i++)
    {

        cumprob[i] = imghist[i] + cumprob[i - 1];
    }

    for (i = 1; i < 256; i++)
    {
	//double n=floor(cumprob[i]);
	
        //newhist[i] = (int)n * 256;

	newhist[i]=cumprob[i]*256;
    }

    // Create the lookup table for the new values of each grayscale color.
    j = 0;
    for (i = 0; i < 256; i++)
    {
        if (newhist[i] <= desiredhistcount[j])
        {
            newhist[i] = j;
        }
        else
        {
            while (newhist[i] > desiredhistcount[j])
            {
                j++;
            }
            if (desiredhistcount[j] - newhist[i] > newhist[i] - desiredhistcount[j - 1])
            {
                newhist[i] = j - 1;
            }
            else
            {
                newhist[i] = j;
            }
        }
    }


    // Create the new image.
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            int nh = newhist[i];
	if(nh > MAX_VALUE){											//if value exceeds
			nh = MAX_VALUE;
		}
            putc(nh, f2);
		putc(nh, f2);
putc(nh, f2);
        }
    }
    fclose(f1);
    fclose(f2);

    return 0;
}


/**
STEP 9: Calculate pixel_density in overall input image:
pixel_density=1/(width*height)
STEP 10:Create a cumulative_pixel_count array off 256 size to hold the frequency of pixels for each
intensity value ranging from 0 t0 255 in the input greyscale image and initialize it to 0
cumulative_pixel_count[256]={0}

STEP 11: Calculate cumulative pixel_count for i=0 to height and for j=0 to width
cumulative_pixel_count[input_buffer[i][j]]+=pixel_density

STEP 12: A desired histogram is set such that for each intensity values(i) ranging from 0 to 256
12.1) if intensity < 129, it has same number of pixel as the intensity value i.e
desired_pixel_count[i]=i
else  it has 128-intensity_value as number of pixel i.e
desired_pixel_count[i]=128-i
12.2) Calculate total pixels in this desired histogram
desired_total_pixels+=desired_pixel_count[i]
STEP 13: Calculate cumulative desired_pixel_count for i=0 to 256
fraction+=desired_pixel_count[i]*(1/desired_total_pixels)
cumulative_desired_pixel_count=fraction
STEP 14: Now create another array for storing new image intensity values as a replacement for each
of the 256 greyscale values in the input image
new_intensity[256]={0}
STEP 15: Initialize J as 0 and for i=0 to 256
15.1) if cumulative_pixel_count[i]<desired_cumulative_pixel_count[j]
new_intensity[i]=j; // if i is old_intensity to be replaced
15.2) else
10.2.1) while cumulative_pixel_count[i]>desired_cumulative_pixel_count[j]
j++;
10.2.2) if desired_cumulative_pixel_count[j]-cumulative_pixel_count[i] >
cumulative_pixel_count[i]-desired_cumulative_pixel_count[j-1]
new_intensity[i]=j-1;
else
new_intensity[i]=j;
STEP 16: Store the new intensity values present in output_buffer for i=0 to height and j=0 to width
and simultaneously store them to output image file
output_buffer[i][j]=new_intensity[input_buffer[i][j]]
// Store replaced intensities for only those intesity values present in image
putc(output_buffer[i][j],output_image_file)
STEP 17: STOP
**/
