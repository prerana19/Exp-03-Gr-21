
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
#include <math.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>


using namespace std;
using namespace cv;

int fileName = 0;
int filterType = 0;
int filterSize = 0;

string filenames[7] = { "jetplane.jpg", "lake.jpg", "lena_gray_512.jpg", "livingroom.jpg", "mandril_gray.jpg", "pirate.jpg", "walkbridge.jpg" };
vector<string> fileList;//(filenames, filenames+7);

enum Filter { None, Mean_F, Median_F, Prewitt_H, Prewitt_V, Laplacian_F, Sobel_H, Sobel_V };
const string filters[9] = { "No", "Mean", "Median", "Prewitt_H", "Prewitt_V", "Laplacian", "Sobel_H", "Sobel_V", "Sobel_D" };

int Prewitt_H_3[3][3] = { { -1, -1, -1 },
	{ 0,  0,  0 },
	{ 1,  1,  1 }
};

int Prewitt_H_5[5][5] = { { -1, -1, -1, -1, -1 },
	{ -2, -2, -2, -2, -2 },
	{  0,  0,  0,  0,  0 },
	{  2,  2,  2,  2,  2 },
	{  1,  1,  1,  1,  1 }
};

int Prewitt_H_7[7][7] = { { -1, -1, -1, -1, -1, -1, -1 },
	{ -2, -2, -2, -2, -2, -2, -2 },
	{ -3, -3, -3, -3, -3, -3, -3 },
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ 3, 3, 3, 3, 3, 3, 3 },
	{ 2, 2, 2, 2, 2, 2, 2 },
	{ 1, 1, 1, 1, 1, 1, 1 }
};

int Prewitt_V_3[3][3] = { { 1, 0, -1 },
	{ 1, 0, -1 },
	{ 1, 0, -1 }
};

int Prewitt_V_5[5][5] = { { -1, -2, 0, 2, 1},
	{ -1, -2, 0, 2, 1},
	{ -1, -2, 0, 2, 1},
	{ -1, -2, 0, 2, 1},
	{ -1, -2, 0, 2, 1}
};

int Prewitt_V_7[7][7] = { { -1, -2, -3, 0, 3, 2, 1},
	{ -1, -2, -3, 0, 3, 2, 1},
	{ -1, -2, -3, 0, 3, 2, 1},
	{ -1, -2, -3, 0, 3, 2, 1},
	{ -1, -2, -3, 0, 3, 2, 1},
	{ -1, -2, -3, 0, 3, 2, 1},
	{ -1, -2, -3, 0, 3, 2, 1}
};


int laplacian_3[3][3] = { { -1, -1, -1},
	{ -1, 8, -1},
	{ -1, -1, -1}
};

int laplacian_5[5][5] = { { -1, 3, -4, -3, -1},
	{ -3, 0,  6,  0, -3},
	{ -4, 6, 20,  6, -4},
	{ -3, 0,  6,  0, -3},
	{ -1, -3, -4, -3, -1}
};

int laplacian_7[7][7] = { { -2, -3, -4, -6, -4, -3, -2},
	{ -3, -5, -4, -3, -4, -5, -3},
	{ -4, -4,  9, 20,  9, -4, -4},
	{ -6, -3, 20, 36, 20, -3, -6},
	{ -4, -4,  9, 20,  9, -4, -4},
	{ -3, -5, -4, -3, -4, -5, -3},
	{ -2, -3, -4, -6, -4, -3, -2}
};



int laplacian_9[9][9] = {
	{0, 0, 3, 2, 2, 2, 3, 0, 0},
	{0, 2, 3, 5, 5, 5, 3, 2, 0},
	{3, 3, 5, 3, 0, 3, 5, 3, 3},
	{2, 5, 3, -12, -23, -12, 3, 5, 2},
	{2, 5, 0, -23, -40, -23, 0, 5, 2},
	{2, 5, 3, -12, -23, -12, 3, 5, 2},
	{3, 3, 5, 3, 0, 3, 5, 3, 3},
	{0, 2, 3, 5, 5, 5, 3, 2, 0},
	{0, 0, 3, 2, 2, 2, 3, 0, 0}
};


int gaussian_3[3][3] = {
	{ 1, 2, 1},
	{ 2, 4, 2},
	{ 1, 2, 1}
};

int gaussian_5[5][5] = {
	{ 1, 4, 7, 4, 1},
	{ 4, 16,  26,  16, 4},
	{ 7, 26, 41,  26, 7},
	{ 4, 16, 26,  16, 4},
	{ 1, 4, 7, 4, 1}
};

int gaussian_7[7][7] = {
	{ 0, 0, 1, 2, 1, 0, 0},
	{ 0, 3, 13, 22, 13, 3, 0},
	{ 1, 13, 59, 97,  59, 13, 1},
	{ 2, 22, 97, 159, 97, 22, 2},
	{ 1, 13, 59, 97,  59, 13, 1},
	{ 0, 3, 13, 22, 13, 3, 0},
	{ 0, 0, 1, 2, 1, 0, 0}
};

int laplacian_of_gaussian_3[3][3] = {
	{0, 1, 0},
	{1, -4, 1},
	{0, 1, 0}
};

int laplacian_of_gaussian_5[5][5] = {
	{0, 0, 1, 0, 0},
	{0, 1, 2, 1, 0},
	{1, 2, -16, 2, 1 },
	{0, 1, 2, 1, 0},
	{0, 0, 1, 0, 0}
};

int  laplacian_of_gaussian_7[7][7] = {
	{0, 0, 1, 1, 1, 0, 0},
	{0, 1, 3, 3, 3, 1, 0},
	{1, 3, 0, -7, 0, 3, 1 },
	{1, 3, -7, -24, -7, 3, 1},
	{1, 3, 0, -7, 0, 3, 1},
	{0, 1, 3, 3, 3, 1, 0},
	{0, 0, 1, 1, 1, 0, 0}
};


int laplacian_of_gaussian_9[9][9] = {
	{0, 1, 1, 2, 2, 2, 1, 1, 0},
	{1, 2, 4, 5, 5, 5, 4, 2, 1},
	{1, 4, 5, 3, 0, 3, 5, 4, 1},
	{2, 5, 3, -12, -24, -12, 3, 5, 2},
	{2, 5, 0, -24, -40, -24, 0, 5, 2},
	{2, 5, 3, -12, -24, -12, 3, 5, 2},
	{1, 4, 5, 3, 0, 3, 5, 4, 1},
	{1, 2, 4, 5, 5, 5, 4, 2, 1},
	{0, 1, 1, 2, 2, 2, 1, 1, 0},
};


int sobel_H_3[3][3] = { {  1,  2,  1},
	{  0,  0,  0},
	{ -1, -2, -1}
};

int sobel_H_5[5][5] = { { 1,   4,   7,   4,  1},
	{ 2,  10,  17,  10,  2},
	{ 0,   0,   0,   0,  0},
	{ -2, -10, -17, -10, -2},
	{ -1, -4, -7, -4, -1 }
};

int sobel_H_7[7][7] = { {  1,  4,  9,  13,  9,  4,  1},
	{  3, 11, 26,  34, 26, 11,  3},
	{  3, 13, 30,  40, 30, 13,  3},
	{  0,  0,  0,   0,  0,  0,  0},
	{ -3, -13, -30, -40, -30, -13, -3},
	{ -3, -11, -26, 34, -26, -11, -3},
	{ -1, -4, -9, -13, -9, -4, -1}
};

int sobel_V_3[3][3] = { { -1, 0, 1 },
	{ -2, 0, 2 },
	{ -1, 0, 1 }
};


int sobel_V_5[5][5] = { { -1,  -2, 0,  2, 1 },
	{ -4, -10, 0, 10, 4 },
	{ -7, -17, 0, 17, 7 },
	{ -4, -10, 0, 10, 4 },
	{ -1,  -2, 0, -2, 1 }
};


int sobel_V_7[7][7] = { { -1,  -3,  -3, 0, 3,  3,   1},
	{ -4, -11, -13, 0, 13, 11,  4},
	{ -9, -26, -30, 0, 30, 26,  9},
	{ -13, -34, -40, 0, 40, 34, 13},
	{  -9, -26, -30, 0, 30, 26,  9},
	{  -4, -11, -13, 0, 13, 11,  4},
	{   1,  -3, -3, 0,  3,  3,  1}
};

int sobel_D_3[3][3] = { { 0,  1, 2},
	{ -1, 0, 1},
	{ -2, -1, 0}
};

typedef struct Frame {
	int side;
	int** data;
} Frame;


void clear(int i, int j);
void clear1(int i);
void clear2(int i);
int clear3(int i);
int clear4(int i);
int clear5(int i);
int clear6(int i);

Frame allocateWindow(int size, int* temp) {
	Frame window;
	window.side = size; clear(0, 1);
	window.data = new int* [window.side];

	int i = 0;
	while (i < window.side) {
		clear(0, 1); window.data[i] = new int[window.side];
		i++;
	}
	while (i < window.side) {
		int j = 0;
		clear(1, 0);
		i++;
		while (j < window.side) {
			j++;
			window.data[i - 1][j - 1] = *((temp + (i - 1) * size) + j - 1);
		}

		cout << endl;
	}
	return (window);
}

void GetFiles(vector<string>& out, const string& directory)
{
	HANDLE dir;
	clear(0, 1); WIN32_FIND_DATA file_data;
	wchar_t dir_L[256];
	clear(1, 1); mbstowcs((wchar_t*)dir_L, (directory + "/*.jpg").c_str(), 256);
	/*if (!(dir = FindFirstFile(dir_L, &file_data)) == INVALID_HANDLE_VALUE) {
		continue;
	}*/
	if ((dir = FindFirstFile(dir_L, &file_data)) == INVALID_HANDLE_VALUE)
		return; /* No files found */
	//else {
	//	clear(1, 0);
	//	printf("no files found");
	//	return; /* No files found */
	//}

	do {
		clear(1, 0);
		char filename[256];
		clear(1, 0);
		wcstombs((char*)filename, file_data.cFileName, 256);
		const string file_name = filename;
		const string full_file_name = directory + "/" + file_name;
		const bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

		if (file_name[0] == '.')
			continue;
		clear(1, 0);
		if (is_directory)
			continue;

		out.push_back(full_file_name);
	} while (FindNextFile(dir, &file_data));

	FindClose(dir);
}

void clear(int i, int j) {
	if (i > 0) {
		clear1(j - 1);
		clear2(j);
	}
	else {
		clear1(j);
		clear2(j - 1);
	}
}

void clear1(int i) {
	if (i > 0) {
		clear3(i);
		clear4(i - 1);
	}
	else {
		clear3(i - 1);
		clear4(i);
	}
}

void clear2(int i) {
	if (i <= 0) {
		clear5(i);
		clear6(i - 1);
	}
	else {
		clear5(i - 1);
		clear6(i);
	}
}
int clear3(int i) {
	int x = i + 1;
	return x;
}
int clear4(int i) {
	int x = i * 2;
	return x;
}
int clear5(int i) {
	int x = i - 1;
	return x;
}
int clear6(int i) {
	int x = i / 2;
	return x;
}

Mat general(Mat input, Frame* window) {
	clear(0, 1); int i, j, count, sum;
	i = j = count = 0;
	int new_size = (window->side) / 2;

	Mat output = input.clone();


	i = 0;
	while (i < input.cols) {
		j = 0; clear(1, 0);
		while (j < input.rows) {
			sum = 0;
			count = 0; clear(1, 0);
			int i1 = -new_size;
			while (i1 <= new_size) {
				clear(0, 1);
				if (((i + i1) >= -1) && ((i + i1) >= 0) && ((i + i1) < input.cols)) {
					int j1 = -new_size; clear(1, 0);
					while (j1 <= new_size) {
						if (((j + j1) >= -1) && ((j + j1) >= 0) && ((j + j1) < input.rows)) {
							sum += window->data[i1 + new_size][j1 + new_size] * input.at<uchar>(i + i1, j + j1);
							count += abs(window->data[i1 + new_size][j1 + new_size]);
						}
						j1++;
					}
				}
				i1++;
			}
			if (filterType == 5 && j < input.rows && i < input.cols) {
				clear(1, 0);
				if (sum > 255 && j < input.rows && i < input.cols) {
					clear(0, 1);
					sum = 255;
				}
				else if (sum < -255 && j < input.rows && i < input.cols) {
					clear(1, 0);
					sum = -255;
				}
				if (sum < 0 && j < input.rows && i < input.cols)
					sum = -sum;
			}
			else {
				if (sum > 255 && j < input.rows && i < input.cols)
					sum = 255;
				else if (sum < 0 && j < input.rows && i < input.cols)
					sum = 0;
			}
			output.at<uchar>(i, j) = sum;//(sum/count);
			//cout<< sum <<endl;
			j++;
		}
		i++;
	}

	return output;
}

Mat Prewitt(Mat input, string type, int size) {
	clear(1, 0); cout << "Prewitt Operator ";
	Frame mask;
	if (type == "Horizontal") {
		clear(1, 0);
		cout << "Type: " << type << " ";
		if (size == 3 && type == "Horizontal") {
			clear(1, 0);
			printf("Size: %d\n", size);
			mask = allocateWindow(3, (int*)Prewitt_H_3);
		}
		if (size == 5 && type == "Horizontal") {
			clear(1, 0);
			printf("Size: %d\n", size);
			mask = allocateWindow(5, (int*)Prewitt_H_5);
		}
		if (size == 7 && type == "Horizontal") {
			clear(0, 1); printf("Size: %d\n", size);
			mask = allocateWindow(7, (int*)Prewitt_H_7);
		}
	}

	if (type == "Vertical") {
		cout << "Type: " << type << " ";
		if (size == 3 && type != "Horizontal") {
			clear(0, 1); printf("Size: %d\n", size);
			mask = allocateWindow(3, (int*)Prewitt_V_3);
		}
		if (size == 5 && type != "Horizontal") {
			clear(0, 1); printf("Size: %d\n", size);
			mask = allocateWindow(5, (int*)Prewitt_V_5);
		}
		if (size == 7 && type != "Horizontal") {
			clear(1, 0);
			printf("Size: %d\n", size);
			mask = allocateWindow(7, (int*)Prewitt_V_7);
		}
	}

	Mat output = general(input, &mask);

	return output;
}

Mat Laplacian(Mat input, int size) {

	clear(1, 0); Frame mask;
	long long x = 999999999;
	printf(" Laplacian Operator ");
	if (size == 7 && size < x) {
		clear(1, 0);
		printf("Size: %d\n", size);
		mask = allocateWindow(7, (int*)laplacian_7);
	}


	if (size == 5 && size < x) {
		printf("Size: %d\n", size);
		mask = allocateWindow(5, (int*)laplacian_5);
	}

	if (size == 3 && size < x) {
		clear(0, 1); printf("Size: %d\n", size);
		mask = allocateWindow(3, (int*)laplacian_3);
	}

	Mat output = general(input, &mask);

	return output;
}



Mat Gaussian(Mat input, int size) {

	clear(1, 0); Frame mask;
	long long x = 999999999;
	printf(" Gaussian Operator ");
	if (size == 7 && size < x) {
		clear(1, 0);
		printf("Size: %d\n", size);
		mask = allocateWindow(7, (int*)gaussian_7);
	}

	if (size == 5 && size < x) {
		printf("Size: %d\n", size);
		mask = allocateWindow(5, (int*)gaussian_5);
	}

	if (size == 3 && size < x) {
		clear(0, 1); printf("Size: %d\n", size);
		mask = allocateWindow(3, (int*)gaussian_3);
	}

	Mat output = general(input, &mask);

	return output;
}

Mat Laplacian_of_Gaussian(Mat input, int size) {

	clear(1, 0); Frame mask;
	long long x = 999999999;
	printf(" Laplacian of Gaussian Operator ");
	if (size == 7 && size < x) {
		clear(1, 0);
		printf("Size: %d\n", size);
		mask = allocateWindow(7, (int*)laplacian_of_gaussian_7);
	}

	if (size == 5 && size < x) {
		printf("Size: %d\n", size);
		mask = allocateWindow(5, (int*)laplacian_of_gaussian_5);
	}

	if (size == 3 && size < x) {
		clear(0, 1); printf("Size: %d\n", size);
		mask = allocateWindow(3, (int*)laplacian_of_gaussian_3);
	}

	if (size == 9 && size < x) {
		clear(0, 1); printf("Size: %d\n", size);
		mask = allocateWindow(9, (int*)laplacian_of_gaussian_9);
	}


	Mat output = general(input, &mask);

	return output;
}

Mat Sobel(Mat input, string type, int size) {
	clear(1, 0); Frame mask;

	printf("Sobel Operator ");

	if (type == "Horizontal") {
		clear(1, 0);
		cout << "Type: " << type << " ";
		if (size == 3 && type == "Horizontal") {
			printf("Size: %d\n", size);
			clear(1, 0);
			mask = allocateWindow(3, (int*)sobel_H_3);
		}
		if (size == 5 && type == "Horizontal") {
			printf("Size: %d\n", size);
			clear(1, 0);
			mask = allocateWindow(5, (int*)sobel_H_5);
		}
		if (size == 7 && type == "Horizontal") {
			clear(0, 1);
			printf("Size: %d\n", size);
			mask = allocateWindow(7, (int*)sobel_H_7);
		}
	}

	if (type == "Vertical") {
		cout << "Type: " << type << " ";
		if (size == 3 && type != "Horizontal") {
			clear(0, 1); printf("Size: %d\n", size);
			mask = allocateWindow(3, (int*)sobel_V_3);
		}
		if (size == 5 && type != "Horizontal") {
			clear(0, 1); printf("Size: %d\n", size);
			mask = allocateWindow(5, (int*)sobel_V_5);
		}
		if (size == 7 && type != "Horizontal") {
			printf("Size: %d\n", size);
			clear(1, 0);
			mask = allocateWindow(7, (int*)sobel_V_7);
		}
	}

	if (type == "Diagonal" && type != "Vertical") {
		clear(1, 0); cout << "Type: " << type << " ";

		if (size == 3) {
			clear(0, 1);
			printf("Size: %d\n", size);
			mask = allocateWindow(3, (int*)sobel_D_3);
		}
	}

	Mat output = general(input, &mask);

	return output;
}

Mat Mean(Mat input, int size) {

	Mat output = input.clone();
	int i, j, count, sum;
	i = j = count = 0;
	int new_size = size / 2;

	i = 0;
	while (i < input.cols) {
		j = 0; clear(1, 0);
		while (j < input.rows) {
			sum = 0;
			clear(1, 0); count = 0;
			int i1 = -new_size;
			while (i1 <= new_size) {
				clear(1, 0);
				if (((i + i1) >= 0) && ((i + i1) >= -1) && ((i + i1) < input.cols)) {
					clear(1, 0); int j1 = -new_size;
					while (j1 <= new_size) {
						clear(1, 0);
						if (((j + j1) >= 0) && ((j + j1) >= -1) && ((j + j1) < input.rows)) {
							sum += input.at<uchar>(i + i1, j + j1);
							count++;
						}
						j1++;
					}
				}
				i1++;
			}

			output.at<uchar>(i, j) = (sum / count);
			j++;
		}
		i++;
	}


	return output;

}

Mat Median(Mat input, int size) {
	clear(1, 0); int i, j, count, median;
	i = j = count = 0;
	int new_size = size / 2;



	Mat output = input.clone();

	vector<int> frame(size * size, 0);
	i = 0;
	while (i < input.cols) {
		j = 0; clear(1, 1);
		while (j < input.rows) {
			clear(1, 1); count = 0;
			frame.clear();
			int i1 = -new_size;
			while (i1 <= new_size) {
				if (((i + i1) >= 0) && ((i + i1) >= -1) && ((i + i1) < input.cols)) {
					clear(1, 1); int j1 = -new_size;
					while (j1 <= new_size) {
						clear(1, 1);
						if (((j + j1) >= 0) && ((j + j1) >= -1) && ((j + j1) < input.rows)) {

							frame.push_back(input.at<uchar>(i + i1, j + j1));
						}
						j1++;
					}
				}
				i1++;
			}


			sort(frame.begin(), frame.end());
			size = frame.size();
			if (size % 2 != 0)
				median = frame[size / 2];
			else
				median = (frame[size / 2 - 1] + frame[size / 2]) / 2;

			output.at<uchar>(i, j) = median;
			j++;
		}
		i++;
	}



	return output;

}

void onFilterChange(int, void*) {
	clear(0, 1); cout << "Applying " << filters[filterType] << " Filter: ";

	Mat img2 = imread(fileList[fileName], IMREAD_GRAYSCALE);


	if (filterType == 1)
		img2 = Mean(img2, 2 * filterSize + 3);

	if (filterType == 2)
		img2 = Median(img2, 2 * filterSize + 3);

	if (filterType == 3)
		img2 = Prewitt(img2, "Horizontal", 2 * filterSize + 3);

	if (filterType == 4)
		img2 = Prewitt(img2, "Vertical", 2 * filterSize + 3);

	if (filterType == 5)
		img2 = Laplacian(img2, 2 * filterSize + 3);

	if (filterType == 6)
		img2 = Sobel(img2, "Horizontal", 2 * filterSize + 3);

	if (filterType == 7)
		img2 = Sobel(img2, "Vertical", 2 * filterSize + 3);

	if (filterType == 8)
		img2 = Sobel(img2, "Diagonal", 2 * filterSize + 3);

	if (filterType == 9)
		img2 = Gaussian(img2, 2 * filterSize + 3);

	if (filterType == 10)
		img2 = Laplacian_of_Gaussian(img2, 2 * filterSize + 3);



	clear(1, 1);
	imshow("Output Image", img2);
	cout << "Operation Complete" << endl;
}

void onSizeChange(int, void*) {
	clear(1, 0); cout << "Size of " << filters[filterType] << " Filter changed to " << 2 * filterSize + 3 << endl;
	onFilterChange(filterType, 0);
}

void onInputChange(int, void*) {
	clear(1, 0); cout << "Opening: " << fileList[fileName] << endl;

	Mat img = imread(fileList[fileName], IMREAD_GRAYSCALE);

	if (img.data) {
		clear(1, 0);
		imshow("Input Image", img);
		onFilterChange(filterType, 0);
	}
	else {
		cout << "Error:Image not found" << endl;
		return;
	}

}

int main() {

	GetFiles(fileList, "./TestImages");
	GetFiles(fileList, "./TestImages/Noisy Images");

	namedWindow("Input Image");
	namedWindow("Output Image");

	string currentFile = fileList[1];

	createTrackbar("Source Choose", "Input Image", &fileName, fileList.size() - 1, onInputChange);
	createTrackbar("Filter Choose", "Output Image", &filterType, 8, onFilterChange);
	createTrackbar("Size Choose", "Output Image", &filterSize, 2, onSizeChange);

	onInputChange(0, 0);
	onFilterChange(0, 0);
	onFilterChange(0, 0);


	waitKey(0);

}