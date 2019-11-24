
# Image-processing-using-c

>Image processing is the process of converting an image into digital form, storing the image data in an array and then manipulating those values using different mathematical operations to do different types of editing and modifications.
Various algorithms can be designed to do various types of operations after the image data is digitalized. 

- We are using Bitmap which is a digital image composed of matrix of dots and each dot corresponds to an individual pixel on a display.
- Each pixel in bitmap image represent different color and together these pixels can be used to represent any type of rectangular picture.
- Bitmap image (BMP) format is capable of storing 2-D digital images both monochrome and color. 
- Bitmap image header is of 54 bytes in which 18th bit stores image width, 22nd bit stores image height and 28th bit stores Bitdepth. 
- If value of Bitdepth is less than 8, then there is a color table of 1024 bytes. 
- Color table is a block of bytes listing the colors used by image. 

The project is basically divided in three modules 
- a client,
- a server and
- the program application running on the server with the image processing algorithms. 

## Development

Images are read using file input output (I/O) routines and filters are applied on the images to produce enhanced or edited images (according to user’s choice). Filtering is done by understanding and modifying the bitmap image headers. 
Reference Image: ![alt text](https://github.com/shivanshthapliyal/Image-processing-using-c/blob/master/Output%20Images/JPEGs/RefImg.jpg)

- Halftoning technique would be used to transform a coloured image to a Greyscale image. 
![alt text](https://github.com/shivanshthapliyal/Image-processing-using-c/blob/master/Output%20Images/JPEGs/Greyscale.jpg "Greyscale")

- Histogram equalization technique would be used to correct the poor contrast of the user’s images. 
- Altering geometry of images would also be possible by displacement, scaling, rotations and cross product. 
- Operations such as brightness, contrast, saturation would be performed. 

All these image processing is executed on server side after the successful image transfer from client to server where server can handle multiple clients together using threads. Server then transfers the new image back to the client. This server to client file transfer and vice versa is achieved using TCP sockets by creating a connection between the two. 

For any read/write on server side there is a corresponding write/read on client side and vice versa
