#ifndef PHOTO_H
#define PHOTO_H

#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <sstream>

const int pixelRange(256);

using namespace std;

class Photo
{
	int* m_photoBits;				//the photo
	int* m_cipher;					//the bits
	int m_pixelCount[pixelRange]{}; //histogram
	int m_codeLength;				//length of the message
	int m_width;					
	int m_height;
	int m_maxVal;	
	int m_length;					//1D length
	string m_fileName;
	string m_key;
	string m_magicNumber;			//Identifier
	string m_hiddenCode;			//Hidden code

public:

	Photo();
	
	//Rather than using a 2D array, which uses a lot of memory and is very slow, we have a 1D array with the index function that lets us access the correct coordinates
	int index(int x, int y) const { return x * m_width + y; }
	//Find the hidden message
	void findCipher();
	//invert color if the photo is negative
	void negative();
	//print the message
	void print();
	//write the photo to a file
	void writePhoto();
	//Contrast equalization
	void contrastFix();
	//Stretch
	void contrastStretch();
	
	void writeHistogram();

	void flipImage();
	// Delete dynamic arrays that are left
	~Photo()
	{
		delete[] m_photoBits;
		delete[] m_cipher;
	}
};

#endif