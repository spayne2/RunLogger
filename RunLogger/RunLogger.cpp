#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;
//run class to represent a run object.
class Run
{
private:
	double distanceInKm; //overall distnace of the run
	int timeMins; // time in minutes 
	int timeSeconds; //time in seconds
	string terrain; //the terrain type of the run e.g. road, track, trail

public:
	//function to ask user to add data to run, called from wherever a new run object is created
	void addData()
	{ //get data from user input
		cout << "Type in distance" << endl;
		cin >> distanceInKm;
		cout << "Type in minutes" << endl;
		cin >> timeMins;
		cout << "Type in seconds" << endl;
		cin >> timeSeconds;
		cout << "Type in terrain type" << endl;
		cin.ignore(1000, '\n');
		cin.clear();
		getline(cin, terrain);
	}
	//function to caclculate the average pace of the run, to be called when the runs are displayed
	string calculateAvgPace() 
	{
		int fullSeconds = (timeMins * 60) + timeSeconds;

		int secondsPerKm = fullSeconds / distanceInKm;

		double minutesPerKm = secondsPerKm / 60;
		int leftOverSeconds = secondsPerKm % 60;

		return to_string(minutesPerKm) + "'" + to_string(leftOverSeconds)+"\"";
	}
	//function to display the run information
	void display()
	{
		cout << "Disance: " << distanceInKm << "Kms, Time " << timeMins << "mm:" << timeSeconds << "ss, Pace: " << calculateAvgPace() << " Terain: " << terrain << endl;
	}

	string getDataForFile()
	{
		return to_string(distanceInKm) + " " + to_string(timeMins) + " " + to_string(timeSeconds) + " " + terrain;
	}

	void loadDataFromFile(ifstream& file)
	{
		file >> distanceInKm; //get the distance from the file
		file >> timeMins; //get the minutes from the file
		file >> timeSeconds; //get the time in seconds from the file
		getline(file, terrain); //get the terrain (getline so it can have spaces)
	}
};

//class to represent a person object
class Person
{
private:
	string name; //store the name
	int age; //store the age
	vector<Run> runs; //store a list of runs; //composition
public:

	void addRun() //function to add run for a user
	{
		cout << "Add info of most recent run" << endl;
		Run run; //initiliase
		run.addData(); //add data
		runs.push_back(run); //add to the vector
	}

	void addData() //function to add data from the user to the person object
	{
		cout << "Type in persons name" << endl;
		cin.ignore(1000, '\n');
		cin.clear();
		getline(cin, name);
		cout << "Type in persons age" << endl;
		cin >> age;
		addRun();		
	}
	//function to return the name of the person
	string getName() 
	{
		return name;
	}
	//function to return the age of the person
	int getAge() 
	{
		return age;
	}
	//function to return the runs that the person has stored
	vector<Run> getRuns() 
	{
		return runs;
	}
	//function to print the runs to the console;
	void printRuns()
	{
		for (int i = 0; i < runs.size(); i++)
		{
			cout << "Run " << (i + 1) << endl;
			runs[i].display();
		}
	}
	//display function to show all user infor
	void display() 
	{
		cout << "Name: " << name << " Age: " << age << endl;
		cout << "Runs: " << endl;
		printRuns();
	}
	//edit function to edit the users info
	void edit()
	{
		cout << "Type in new name" << endl;
		cin.ignore(1000, '\n'); //clears the input buffer before using getline
		cin.clear();
		getline(cin, name);

		cout << "Type in new age" << endl;
		cin >> age;
	}

	void loadDataFromFile(ifstream& file)
	{
		getline(file, name); //get the name, allows for spaces
		file >> age; //get the age

		Run run; //create  run object
		bool getRuns = true; //control variable for the loop
		while (getRuns)
		{
			//assume there will always be one run
			run.loadDataFromFile(file); //call the load data from file for run
			runs.push_back(run); //add the run to the vector
			//need to check if the line is END_RECORD, if not need to wind back as to not miss data
			streampos oldpos = file.tellg(); //get the current file position
			string checkEndOfRecord;
			getline(file, checkEndOfRecord); //get the next line
			if (checkEndOfRecord == "END_RECORD") //if its end record, then stop the loop
			{
				getRuns = false; //set loop control to false
			}
			else {
				file.seekg(oldpos); //otherwise, wind back in the file and let the loop continue
			}			
		}		
	}
};

//Menu class, controls the program running, holds a vector of persons
class Menu
{
private:
	vector<Person> persons; //vector to hold all the persons
	bool keepMenuGoing = true;
	//function to print person record
	void printPersonVector()
	{
		for (int i = 0;i<persons.size();i++)
		{
			cout << (i + 1) << " " << persons[i].getName() << endl;
		}
	}
	//function to add a new person
	void addNewPerson()
	{
		Person person; //initialize a person object
		person.addData(); //call the add data function;
		persons.push_back(person); // add to the vector stored in the menu object
	}

	//function to add a new run for a user, called from the main menu
	void addNewRun() 
	{
		cout << "Add run for which person?" << endl;

		printPersonVector();

		int input;
		cin >> input;

		persons[input - 1].addRun();
	}

	//function to show person, called from the main menu
	void showPerson() {
		cout << "Display which person" << endl;

		printPersonVector();

		int input;
		cin >> input;

		persons[input - 1].display();
	}
	//function to edit person, called from the main menu
	void editPerson() {
		cout << "Edit which person" << endl;

		printPersonVector();

		int input;
		cin >> input;

		persons[input - 1].edit();
	}
	//function for writing all the info to a file
	void writeToFile()
	{
		ofstream outPutFile; //create output file objec
		outPutFile.open("run_logger.txt"); //open the file wanted
		for (int i = 0; i < persons.size(); i++) //loop over all the person records
		{
			outPutFile << persons[i].getName() << endl; //send the name to the file 
			outPutFile << persons[i].getAge() << endl; //send the age to the file
			for (auto person : persons[i].getRuns()) //send all the runs to the file
			{
				outPutFile << person.getDataForFile() << endl;;
			}
			outPutFile << "END_RECORD";
			if (i != (persons.size() - 1))//if its not the last record add an endl for the next record
			{
				outPutFile << endl;
			}
		}
	}

	void loadFromFile()
	{
		ifstream inputFile;
		inputFile.open("run_logger.txt"); //open  file needed
		if (inputFile)
		{
			while (!inputFile.eof())
			{
				Person person; //create a person object
				person.loadDataFromFile(inputFile);  //load data from file
				persons.push_back(person); //add the person to the vector
			}
		}
		
	}

	void saveAndQuit()
	{
		writeToFile();
		keepMenuGoing = false;
		cout << "Goodbye" << endl;
	}

public:
	//function to display the start menu
	void start()
	{
		loadFromFile();
		
		while (keepMenuGoing)
		{
			cout << "Welcome to the run logger, please choose an option" << endl;
			cout << "1. Add a new person" << endl;
			cout << "2. Add a new run" << endl;
			cout << "3. Show Person" << endl;
			cout << "4. Edit Person" << endl;
			cout << "5. Write to File" << endl;
			cout << "6. Load from file" << endl;
			cout << "7. Save a Quit" << endl;

			int input;
			cin >> input;

			switch (input)
			{
			case 1:
				addNewPerson();
				break;
			case 2:
				addNewRun();
				break;
			case 3:
				showPerson();
				break;
			case 4:
				editPerson();
				break;
			case 5:
				writeToFile();
				break;
			case 6:
				loadFromFile();
				break;
			case 7:
				saveAndQuit();
				break;
			}
		}
	}
};

int main()
{
	//initilize menu object
	Menu m;
	m.start(); //start the menu;
}


