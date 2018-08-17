#include "Photo.h"

Photo::Photo()
{
	//get the filename
	cout << "What is the name of the Image?\n";
	cin >> m_fileName;

	//check our "database" and match with key
	if (m_fileName == "1demo.pgm") m_key = "key1demo.txt";
	if (m_fileName == "1S.pgm") m_key = "key1.txt";
	if (m_fileName == "2S.pgm") m_key = "key2.txt";
	if (m_fileName == "3S.pgm") m_key = "key3.txt";
	if (m_fileName == "4S.pgm") m_key = "key4.txt";
	if (m_fileName == "5S.pgm") m_key = "key5.txt";


	// This is from project 3
	ifstream file(m_fileName);

	if (!file)
		cerr << "Error: Could not find the specified file!" << endl;

	//read and check the magic number
	m_magicNumber;
	getline(file, m_magicNumber, ' ');

	//save the first 3 important parts of the image
	if (m_magicNumber == "P2")
	{
		file >> m_width;
		file >> m_height;
		file >> m_maxVal;
	}
	else //just in case they use P5
		cerr << "Error: Wrong file type!";

	//find necessary length and create storage array
	m_length = m_height * m_width;
	m_photoBits = new int[m_length];

	//store
	for (int i = 0; i < m_length; i++)
	{
		file >> m_photoBits[i];
	}


}

void Photo::findCipher()
{
	//initalize the file and open it
	ifstream file(m_key);

	//prepare for if it isn't there
	if (!file)
		cerr << "Error: Could not find the specified file!" << endl;
	
	//read the code length from the key, make a dynamic array for the bits
	file >> m_codeLength;
	m_cipher = new int[m_codeLength];

	//get the x and y coordinates
	for (int i = 0; i < m_codeLength; i++)
	{
		//read the coordinates as strings
		string xVal;
		getline(file, xVal, ',');
		string yVal;
		getline(file, yVal);


		int xValue(0);
		int yValue(0);

		// convert them to int and store them
		if (!(istringstream(xVal) >> xValue)) xValue = 0;
		if (!(istringstream(yVal) >> yValue)) yValue = 0;

		// find and store the LSB of the pixels in cipher
		m_cipher[i] = m_photoBits[index(xValue, yValue)] % 2;

	}

	for (int i = 0; i < m_codeLength; i += 7)
	{
		//convert 7 digit segments to decimal
		int total = 0;
		total += (m_cipher[i] * pow(2, 0));
		total += (m_cipher[i + 1] * pow(2, 1));
		total += (m_cipher[i + 2] * pow(2, 2));
		total += (m_cipher[i + 3] * pow(2, 3));
		total += (m_cipher[i + 4] * pow(2, 4));
		total += (m_cipher[i + 5] * pow(2, 5));
		total += (m_cipher[i + 6] * pow(2, 6));

		//convert to ASCII
		char letter = static_cast<char>(total);

		//add the characters to a string containing the code
		m_hiddenCode += letter;
	}

}

void Photo::negative()
{
	//ask and validate input
	cout << "Is the photo a Negative?(Y/N)";
	char answer;
	cin >> answer;

	switch (answer)
	{
	case 'Y':
	case 'y':
	{
		//if validate invert colors
		for (int i = 0; i < m_length; i++)
			m_photoBits[i] = 255 - m_photoBits[i];
		break;
	}
	case 'N':
	case 'n':
		break;
	default:
		cerr << "Error: That input was not recognized!";
	}
}

void Photo::print()
{
	/*
	cout << m_height << endl;
	cout << m_width << endl;
	cout << m_key << endl;
	for (int i = 0; i < m_codeLength; i++)
	{
	cout << m_cipher[i];
	if (!((i + 1) % 7))
	cout << ' ';
	}
	cout << endl;
	*/
	//print message
	cout << "The Hidden code is:\n ";
	cout << m_hiddenCode;
	cout << '\n';
}

void Photo::writePhoto()
{
	//Provided in the project description
	//start output stream
	ofstream file;
	file.open("1demo_modified.pgm");

	//output identifiers first
	file << m_magicNumber << " " << m_width << " " << m_height << " " << m_maxVal << "\n";

	//output altered array
	for (int i = 0; i < m_length; i++)
	{
		file << m_photoBits[i] << " ";
	}
}

void Photo::contrastFix()
{

	const int bits(pow(2, 8) - 1);
	double probability[pixelRange];

	for (int j = 0; j < pixelRange; j++)
	{
		// get the historgram
		for (int i = 0; i < m_length; i++)
		{
			if (m_photoBits[i] == j)
			{
				m_pixelCount[j]++;
			}
		}
		//find the probability of each pixel value appearing
		probability[j] = static_cast<double>(m_pixelCount[j]) / m_length;

	}

	//find the cumulative probability distribution and multiply it by the max pixel value
	double cumProbability[pixelRange];
	cumProbability[0] = probability[0] * bits;

	for (int i = 1; i < pixelRange; i++)
	{
		cumProbability[i] = probability[i] * bits + cumProbability[i - 1];
	}

	//replace the values in m_photoBits with the new adjusted values
	for (int i = 0; i < pixelRange; i++)
		for (int k = 0; k < m_length; k++)
			if (m_photoBits[k] == i)
				m_photoBits[k] = round(cumProbability[i]);

}

void Photo::contrastStretch()
{
	//highest and lowest
	int lL(0);
	int uL(255);

	//find the current smallest
	int smallest = m_photoBits[0];
	for (int i = 1; i < m_length; i++)
		if (m_photoBits[i] < smallest)
			smallest = m_photoBits[i];

	//find the current largest
	int largest = m_photoBits[0];
	for (int i = 1; i < m_length; i++)
		if (m_photoBits[i] > largest)
			largest = m_photoBits[i];

	//use the equation given in class
	for (int i = 0; i < m_length; i++)
	{
		m_photoBits[i] = (m_photoBits[i] - smallest)*((uL - lL) / (largest - smallest)) + lL;
	}

}

void Photo::writeHistogram()
{
	//rather than using the histogram defined a sampling method is used
	for (int i = 0; i < pixelRange; i += 10)
	{
		cout << i << " to " << i + 10 << ":";
		for (int k = 0; k < m_length; k += 500)
			if ((m_photoBits[k] >= i) && (m_photoBits[k] <= (i + 10)))
				cout << "*";
		cout << '\n';
	}
}

void Photo::flipImage()
{
	cout << "Is the image Flipped?(Y/N)";
	char answer;
	cin >> answer;

	switch (answer)
	{
	case 'Y':
	case 'y':
	{
		int* copy = new int[m_length];
		for (int j = 0; j < m_length; j++)
		{  //flip the image using the copy
			copy[j] = m_photoBits[m_length - 1 - j];
		}

		for (int j = 0; j < m_length; j++)
			m_photoBits[j] = copy[j];

		delete[] copy;
		break;
	}
	case 'N':
	case 'n':
		break;
	default:
		cerr << "Error: That input was not recognized!";
	}

	
}