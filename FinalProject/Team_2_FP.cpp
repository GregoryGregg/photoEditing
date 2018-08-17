#include "Photo.h"

//check the input and handle any errors if they happen
bool newPhoto()
{
	cout << "Would you like to process another Photo? (Y/N) \n";
	char answer;
	cin >> answer;

	switch (answer)
	{
	case 'Y': return 1;
	case 'y': return 1;
	case 'N': return 0;
	case 'n': return 0;
	default:
	{
		cout << "That input is not recognized, please try again";
		
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(32767, '\n');
		}
		return newPhoto();
	}
	}
}

int main()
{

	//Take from Project 5 PDF, NOT MADE BY STUDENT. Credit Goes to Carlos De Niz
	#ifdef _WIN32
		cout << "_WIN32" << endl;
	#else
		cout << "not _WIN32" << endl;
	#endif
	#ifdef _MSC_VER
		cout << "_MSC_VER" << endl;
	#elif __APPLE__
		cout << "__APPLE__" << endl;
	#else
		cout << "Other" << endl;
	#endif

	//Repeat flag
	bool again;

	do
	{
		//Start object
		Photo codedPhoto;

		//Find the message and print it
		codedPhoto.findCipher();
		codedPhoto.print();

		//Flags
		bool twice(0);
		int count(0);

		do
		{
			codedPhoto.writeHistogram();

			//Prompt user on method
			cout << "1)Contrast Stretch\n2)Contrast Fix\n";
			int x;
			cin >> x;

			//switch method for method selection
			switch (x)
			{
			case 1:
				codedPhoto.contrastStretch();
				break;
			case 2:
				codedPhoto.contrastFix();
				break;
			default:
				cerr << "Sorry but that input was not recognized, please start over";
			}
			
			//Is the image flipped or negative, both or neither?
			codedPhoto.flipImage();
			codedPhoto.negative();
			codedPhoto.writeHistogram();

			//If count is 1, it means they have already edited the photo twice so they won't be allowed to do it a 3rd time
			//Save or edit again
			if (!count)
			{
				cout << "1)Save the current image\n2)Continue Processing (could lead to poor picture quality)\n";
				int y;
				cin >> y;

				switch (y)
				{
				case 1:
					codedPhoto.writePhoto();
					break;
				case 2:
					twice = 1;
					count++;
					break;
				default:
				{
					"Sorry that input was not recognized, we'll go ahead and save the photo for you.";
					codedPhoto.writePhoto();
				}
				}
			}
			else
			{
				codedPhoto.writePhoto();
				twice = 0;
			}
		} while (twice);
		
		//Do they want run another photo
		again = newPhoto();
		cout << endl;

	} while (again);

	return 0;
}