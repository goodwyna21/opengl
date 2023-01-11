#include <iostream>
#include <algorithm>


void writeFile(const char* fname, unsigned char* image, const unsigned int w, const unsigned int h){
	FILE *f;
	int filesize = 54 + 3*w*h;

	unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
	unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};

	bmpfileheader[ 2] = (unsigned char)(filesize    );
	bmpfileheader[ 3] = (unsigned char)(filesize>> 8);
	bmpfileheader[ 4] = (unsigned char)(filesize>>16);
	bmpfileheader[ 5] = (unsigned char)(filesize>>24);

	bmpinfoheader[ 4] = (unsigned char)(       w    );
	bmpinfoheader[ 5] = (unsigned char)(       w>> 8);
	bmpinfoheader[ 6] = (unsigned char)(       w>>16);
	bmpinfoheader[ 7] = (unsigned char)(       w>>24);
	bmpinfoheader[ 8] = (unsigned char)(       h    );
	bmpinfoheader[ 9] = (unsigned char)(       h>> 8);
	bmpinfoheader[10] = (unsigned char)(       h>>16);
	bmpinfoheader[11] = (unsigned char)(       h>>24);

	f = fopen(fname,"wb");
	fwrite(bmpfileheader,1,14,f);
	fwrite(bmpinfoheader,1,40,f);
	fwrite(image, 1, w*h*3, f);

	fclose(f);
}

void blur(unsigned char* image, const unsigned int w, const unsigned int h){
	unsigned int arr_size = w*h*3;

	int cnt, index;
	int avg[3];
	int offsets[4] = {-(3*w), -3, 3, 3*w};
	unsigned char img_copy[3*w*h];

	for(int i = 0; i < w; i++){
		for(int j = 0; j < h; j++){
			cnt = 1;
			index = 3*(w*j + i);
			avg[0] = image[index];
			avg[1] = image[index+1];
			avg[2] = image[index+2];

			for(int k = 0; k < 4; k++){
				if(index + offsets[k] >= 0 && index + offsets[k] < arr_size){
					cnt++;
					avg[0] += image[index+offsets[k]];
					avg[1] += image[index+offsets[k]+1];
					avg[2] += image[index+offsets[k]+2];
				}
			}

			img_copy[index] = avg[0] / cnt;
			img_copy[index+1] = avg[1] / cnt;
			img_copy[index+2] = avg[2] / cnt;
		}
	}

	std::copy(img_copy, img_copy+(3*w*h), image);
}

int main(){
	std::cout << "Slime Mold!" << std::endl;

	const unsigned int w = 100;
	const unsigned int h = 80;

	unsigned char image[w*h*3];
	for(int i = 0; i < w; i++){
		for(int j = 0; j < h; j++){
			image[3*(w*j+i)+2] = 30;
			image[3*(w*j+i)+1] = 30;
			image[3*(w*j+i)] = 30;
		}
	}

	for(int j = 0; j < h; j++){
		image[3*(w*j+50)] = 255;
		image[3*(w*j+50)+1] = 0;
		image[3*(w*j+50)+2] = 0;
	}

	writeFile("noBlur.bmp",image, w, h);
	blur(image, w, h);
	writeFile("blur.bmp",image, w, h);
	
	return 0;
}