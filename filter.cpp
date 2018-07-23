
#include "stdafx.h"
#include <strsafe.h>
#include "resource.h"
#include "colours.h"
#include <vector>

#define filterWidth 3
#define filterHeight 3


double filter[filterHeight][filterWidth] =
{
	0, 0, 0,
	0, 1, 0,
	0, 0, 0
};

double factor = 1.0;
double bias = 0.0;

int main(int argc, char *argv[])
{
	//load the image into the buffer
	unsigned long w = 0, h = 0;
	std::vector<RGBQUAD> image;
	LoadBitmapA(image, "name.bmp");//gotta convert from RGBimage to hinstance
	//gotta fix and modify name of file, so that comes from user input or previous definition
	std::vector<RGBQUAD> result(image.size());
	
	//not sure about fuction in line below, theorically sets up the screen, do we really need it?
	//screen(w, h, 0, "Filters");
	
	RGBQUAD color; //the color for the pixels

					//apply the filter
	for (int x = 0; x < w; x++)
		for (int y = 0; y < h; y++)
		{
			double red = 0.0, green = 0.0, blue = 0.0;

			//multiply every value of the filter with corresponding image pixel
			for (int filterY = 0; filterY < filterHeight; filterY++)
				for (int filterX = 0; filterX < filterWidth; filterX++)
				{
					int imageX = (x - filterWidth / 2 + filterX + w) % w;
					int imageY = (y - filterHeight / 2 + filterY + h) % h;
					red += image[imageY * w + imageX].rgbRed * filter[filterY][filterX];
					green += image[imageY * w + imageX].rgbGreen * filter[filterY][filterX];
					blue += image[imageY * w + imageX].rgbBlue * filter[filterY][filterX];
				}

			//truncate values smaller than zero and larger than 255
			result[y * w + x].rgbRed = min(max(int(factor * red + bias), 0), 255);
			result[y * w + x].rgbGreen = min(max(int(factor * green + bias), 0), 255);
			result[y * w + x].rgbBlue = min(max(int(factor * blue + bias), 0), 255);
		}

	//draw the result buffer to the screen
	for (int y = 0; y < h; y++)
		for (int x = 0; x < w; x++)
		{
			COLORREF SetPixel( //this function handles COLORREF, while the data are now in RGBQUAD. gotta fix this crap
			//	NULL,//here goes the HDC, handling drawing attributes and device context. not sure if leaving to NULL is ok
				//x,
				//y,
				result[y * w + x]
			);
		}

	//redraw & sleep
	redraw();
	sleep();
}