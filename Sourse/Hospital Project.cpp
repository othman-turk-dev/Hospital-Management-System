#pragma warning (disable : 4996)

#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <windows.h>
#include <thread>
#include <chrono>

using namespace std;


void ShowMainMenue();

///*                         Department Login                                *///

struct sUser
{
	string Username;
	string Password;
	double AllMoneyInBank;
	bool MarkForDelete = false;

};

sUser GeneralUser;

const string UserNameFile = "Data\\Users.txt";

vector<string> SplitString(string S1, string Delim)
{
	vector<string> vString;
	short pos = 0;
	string sWord; // define a string variable  

	// use find() function to get the position of the delimiters  
	while ((pos = S1.find(Delim)) != std::string::npos)
	{
		sWord = S1.substr(0, pos); // store the word   
		if (sWord != "")
		{
			vString.push_back(sWord);
		}

		S1.erase(0, pos + Delim.length());  /* erase() until positon and move to next word. */
	}

	if (S1 != "")
	{
		vString.push_back(S1); // it adds last word of the string.
	}

	return vString;

}

sUser ConvertLinetoRecordForUser(string Line, string Seperator = "-")
{

	sUser User;
	vector<string> vUserData;

	vUserData = SplitString(Line, Seperator);

	User.Username = vUserData[0];
	User.Password = vUserData[1];
	User.AllMoneyInBank = stod(vUserData[2]);

	return User;
}

vector <sUser> LoadUsersDataFromFile(string FileName)
{

	vector <sUser> vUsers;

	fstream MyFile;
	MyFile.open(FileName, ios::in);//read Mode

	if (MyFile.is_open())
	{

		string Line;
		sUser User;

		while (getline(MyFile, Line))
		{

			User = ConvertLinetoRecordForUser(Line);

			vUsers.push_back(User);
		}

		MyFile.close();

	}

	return vUsers;

}

bool FindUserByUsernameAndPassword(string Username, string Password, sUser& User)
{

	vector <sUser> vUsers = LoadUsersDataFromFile(UserNameFile);

	for (sUser U : vUsers)
	{

		if (U.Username == Username && U.Password == Password)
		{
			User = U;
			return true;
		}

	}
	return false;

}

bool  LoadUserInfo(string Username, string Password)
{
	sUser User;
	if (FindUserByUsernameAndPassword(Username, Password, GeneralUser))
		return true;
	else
		return false;

}

string ConvertRecordToLineUser(sUser User, string Seperator = "-")
{

	string stUserRecord = "";

	stUserRecord += User.Username + Seperator;
	stUserRecord += User.Password + Seperator;
	stUserRecord += to_string(User.AllMoneyInBank);

	return stUserRecord;

}

vector <sUser> SaveUsersDataToFile(string FileName, vector <sUser> vUsers)
{

	fstream MyFile;
	MyFile.open(FileName, ios::out);//overwrite

	string DataLine;

	if (MyFile.is_open())
	{

		for (sUser U : vUsers)
		{

			if (U.MarkForDelete == false)
			{
				//we only write records that are not marked for delete.  
				DataLine = ConvertRecordToLineUser(U);
				MyFile << DataLine << endl;

			}

		}

		MyFile.close();

	}

	return vUsers;

}

bool AddMoneyToBankByPassword(string Username, double Amount, vector <sUser>& vUsers)
{
	for (sUser& U : vUsers)
	{
		if (U.Username == Username)
		{
			U.AllMoneyInBank += Amount;
			SaveUsersDataToFile(UserNameFile, vUsers);
			return true;
		}

	}

}

///*                      Department Employee                       *///

const string EmployeesFileName = "Data\\Employees.txt";

void ShowMenueOfEmployee();

enum enMenueOfEmployeeOptions
{
	eListEmployees = 1, eAddNewEmployee = 2,
	eDeleteEmployee = 3, eUpdateEmployee = 4,
	eFindEmployee = 5, eTransaction = 6, eReturnMainMenue = 7
};

struct sEmployee
{
	string AccountNumber;
	string PinCode;
	string Name;
	string Phone;
	double AccountBalance;
	bool MarkForDelete = false;

};

short ReadMenueOfEmployeeOption(short From, short End)
{
	cout << "Choose what do you want to do From " << From << " To " << End << " : ";
	short Choice = 0;
	cin >> Choice;

	return Choice;
}

sEmployee ConvertLinetoRecord(string Line, string Seperator = "-")
{
	sEmployee Employee;
	vector<string> vEmployeeData;
	vEmployeeData = SplitString(Line, Seperator);

	Employee.AccountNumber = vEmployeeData[0];
	Employee.PinCode = vEmployeeData[1];
	Employee.Name = vEmployeeData[2];
	Employee.Phone = vEmployeeData[3];
	Employee.AccountBalance = stod(vEmployeeData[4]);//cast string to double
	return Employee;

}

vector <sEmployee> LoadEmployeeDataFromFile(string FileName)
{
	vector <sEmployee> vEmployees;
	fstream MyFile;

	MyFile.open(FileName, ios::in);//read Mode

	if (MyFile.is_open())
	{
		string Line;
		sEmployee Employee;

		while (getline(MyFile, Line))
		{
			Employee = ConvertLinetoRecord(Line);
			vEmployees.push_back(Employee);
		}
		MyFile.close();
	}
	return vEmployees;
}

void PrintEmployeeRecordLine(sEmployee Employee)
{
	cout << "| " << setw(15) << left << Employee.AccountNumber;
	cout << "| " << setw(40) << left << Employee.Name;
	cout << "| " << setw(12) << left << Employee.AccountBalance;

}

void ShowAllEmployeesScreen()
{
	vector <sEmployee> vEmployees = LoadEmployeeDataFromFile(EmployeesFileName);

	cout << "\n\t\t\t\tEmployee List (" << vEmployees.size() << ") Employee(s).";
	cout << "\n_____________________________________________________";
	cout << "________________________________________\n" << endl;

	cout << "| " << left << setw(15) << "Accout Number";
	cout << "| " << left << setw(40) << "Employee Name";
	cout << "| " << left << setw(12) << "Balance";
	cout << "\n_____________________________________________________";
	cout << "________________________________________\n" << endl;

	if (vEmployees.size() == 0)
		cout << "\t\t\t\tNo Employees Available In the Hospital..!";
	else

		for (sEmployee Employee : vEmployees)
		{

			PrintEmployeeRecordLine(Employee);
			cout << endl;
		}

	cout << "\n_____________________________________________________";
	cout << "________________________________________\n" << endl;


}

void GoBackToMenueOfEmployee()
{
	cout << "\n\n\tPress any key to go back to Menue Of Employee...";
	system("pause>0");
	ShowMenueOfEmployee();

}

bool EmployeeExistsByAccountNumber(string AccountNumber, string FileName)
{

	vector <sEmployee> vEmployees;
	fstream MyFile;

	MyFile.open(FileName, ios::in);//read Mode

	if (MyFile.is_open())
	{
		string Line;
		sEmployee Employee;

		while (getline(MyFile, Line))
		{
			Employee = ConvertLinetoRecord(Line);
			if (Employee.AccountNumber == AccountNumber)
			{
				MyFile.close();
				return true;
			}
			vEmployees.push_back(Employee);
		}

		MyFile.close();

	}
	return false;
}

sEmployee ReadNewEmployee()
{
	sEmployee Employee;

	cout << "\tEnter Account Number? ";

	// Usage of std::ws will extract allthe whitespace character
	getline(cin >> ws, Employee.AccountNumber);

	while (EmployeeExistsByAccountNumber(Employee.AccountNumber, EmployeesFileName))
	{
		cout << "\n\tEmployee with [" << Employee.AccountNumber << "] already exists, Enter another Account Number? ";
		getline(cin >> ws, Employee.AccountNumber);
	}

	cout << "\tEnter PinCode? ";
	getline(cin, Employee.PinCode);

	cout << "\tEnter Name? ";
	getline(cin, Employee.Name);

	cout << "\tEnter Phone? ";
	getline(cin, Employee.Phone);

	cout << "\tEnter Account Balance? ";
	cin >> Employee.AccountBalance;

	return Employee;
}

string ConvertRecordToLine(sEmployee Employee, string Seperator = "-")
{
	string stEmployeeRecord = "";
	stEmployeeRecord += Employee.AccountNumber + Seperator;
	stEmployeeRecord += Employee.PinCode + Seperator;
	stEmployeeRecord += Employee.Name + Seperator;
	stEmployeeRecord += Employee.Phone + Seperator;
	stEmployeeRecord += to_string(Employee.AccountBalance);
	return stEmployeeRecord;

}

void AddDataLineToFile(string FileName, string stDataLine)
{
	vector<string> lines;
	fstream MyFile;

	MyFile.open(FileName, ios::in);

	if (MyFile.is_open())
	{
		string line;
		while (getline(MyFile, line))
		{
			if (!line.empty()) {
				lines.push_back(line);
			}
		}
		MyFile.close();
	}

	lines.push_back(stDataLine);

	sort(lines.begin(), lines.end(), [](const string& a, const string& b)
		{return stoi(a.substr(1)) < stoi(b.substr(1));  });

	MyFile.open(FileName, ios::out | ios::trunc);

	if (MyFile.is_open())
	{
		for (const auto& line : lines)
		{
			MyFile << line << endl;
		}
		MyFile.close();
	}
}

void AddDataLineToFileTheSrting(string FileName, string stDataLine)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out | ios::app);

	if (MyFile.is_open())
	{

		MyFile << stDataLine << endl;

		MyFile.close();
	}
}

void AddNewEmployee()
{
	sEmployee Employee;
	Employee = ReadNewEmployee();
	AddDataLineToFile(EmployeesFileName, ConvertRecordToLine(Employee));

}

void AddNewEmployees()
{
	char AddMore = 'Y';
	do
	{
		cout << "\tAdding New Employee:\n\n";

		AddNewEmployee();
		cout << "\n\tEmployee Added Successfully, do you want to add more Employees? Y/N? ";
		cin >> AddMore;

	} while (toupper(AddMore) == 'Y');

}

void ShowAddNewEmployeesScreen()
{
	cout << "  ============================================================================================  \n";
	cout << "                                      Add New Employees                                         \n";
	cout << "  ============================================================================================  \n";

	AddNewEmployees();
}

string ReadEmployeeAccountNumber()
{
	string AccountNumber = "";

	cout << "\n\tPlease enter Account Number? ";
	cin >> AccountNumber;

	return AccountNumber;
}

bool FindEmployeeByAccountNumber(string AccountNumber, vector <sEmployee> vEmployees, sEmployee& Employee)
{
	for (sEmployee C : vEmployees)
	{

		if (C.AccountNumber == AccountNumber)
		{
			Employee = C;
			return true;
		}

	}

	return false;
}

void PrintEmployeeCard(sEmployee Employee)
{
	cout << "\n\tThe following are the Employee details:\n";
	cout << "\t-------------------------------------------------------";
	cout << "\n\tAccout Number  : " << Employee.AccountNumber;
	cout << "\n\tPin Code       : " << Employee.PinCode;
	cout << "\n\tName           : " << Employee.Name;
	cout << "\n\tPhone          : " << Employee.Phone;
	cout << "\n\tAccount Balance: " << Employee.AccountBalance;
	cout << "\n\t-----------------------------------------------------\n";

}

bool MarkEmployeeForDeleteByAccountNumber(string AccountNumber, vector <sEmployee>& vEmployees)
{

	for (sEmployee& C : vEmployees)
	{

		if (C.AccountNumber == AccountNumber)
		{
			C.MarkForDelete = true;
			return true;
		}

	}

	return false;
}

vector <sEmployee> SaveEmployeeDataToFile(string FileName, vector <sEmployee> vEmployees)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);//overwrite

	string DataLine;

	if (MyFile.is_open())
	{
		for (sEmployee C : vEmployees)
		{

			if (C.MarkForDelete == false)
			{
				//we only write records that are not marked for delete.  
				DataLine = ConvertRecordToLine(C);
				MyFile << DataLine << endl;
			}

		}

		MyFile.close();
	}

	return vEmployees;
}

bool DeleteEmployeeByAccountNumber(string AccountNumber, vector <sEmployee>& vEmployees)
{
	sEmployee Employee;
	char Answer = 'n';

	if (FindEmployeeByAccountNumber(AccountNumber, vEmployees, Employee))
	{

		PrintEmployeeCard(Employee);

		cout << "\n\n\tAre you sure you want delete this Employee? y/n ? ";
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y')
		{
			MarkEmployeeForDeleteByAccountNumber(AccountNumber, vEmployees);
			SaveEmployeeDataToFile(EmployeesFileName, vEmployees);

			//Refresh Employees 
			vEmployees = LoadEmployeeDataFromFile(EmployeesFileName);

			cout << "\n\tEmployee Deleted Successfully.";
			return true;
		}

	}
	else
	{
		cout << "\n\tEmployee with Account Number (" << AccountNumber << ") is Not Found!";
		return false;
	}

}

void ShowDeleteEmployeeScreen()
{
	cout << "  ============================================================================================  \n";
	cout << "                                        Delete Employee                                         \n";
	cout << "  ============================================================================================  \n";

	char AddMore = 'Y';
	do
	{
		vector <sEmployee> vEmployees = LoadEmployeeDataFromFile(EmployeesFileName);
		string AccountNumber = ReadEmployeeAccountNumber();

		DeleteEmployeeByAccountNumber(AccountNumber, vEmployees);

		cout << "\n\tDo you want to delet other Employees? Y/N? ";
		cin >> AddMore;

	} while (toupper(AddMore) == 'Y');

}

sEmployee ChangeEmployeeRecord(string AccountNumber)
{
	sEmployee Employee;

	Employee.AccountNumber = AccountNumber;

	cout << "\n\n\tEnter PinCode? ";
	getline(cin >> ws, Employee.PinCode);

	cout << "\tEnter Name? ";
	getline(cin, Employee.Name);

	cout << "\tEnter Phone? ";
	getline(cin, Employee.Phone);

	cout << "\tEnter Account Balance? ";
	cin >> Employee.AccountBalance;

	return Employee;
}

bool UpdateEmployeeByAccountNumber(string AccountNumber, vector <sEmployee>& vEmployees)
{

	sEmployee Employee;
	char Answer = 'n';

	if (FindEmployeeByAccountNumber(AccountNumber, vEmployees, Employee))
	{

		PrintEmployeeCard(Employee);
		cout << "\n\n\tAre you sure you want update this Employee? y/n ? ";
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y')
		{
			for (sEmployee& C : vEmployees)
			{
				if (C.AccountNumber == AccountNumber)
				{
					C = ChangeEmployeeRecord(AccountNumber);
					break;
				}
			}

			SaveEmployeeDataToFile(EmployeesFileName, vEmployees);

			cout << "\n\tEmployee Updated Successfully.\n";
			return true;
		}

	}
	else
	{
		cout << "\n\tEmployee with Account Number (" << AccountNumber << ") is Not Found!";
		return false;
	}

}

void ShowUpdateEmployeeScreen()
{
	cout << "  ============================================================================================  \n";
	cout << "                                   Update Employee Info                                         \n";
	cout << "  ============================================================================================  \n";

	char AddMore = 'Y';
	do
	{
		vector <sEmployee> vEmployees = LoadEmployeeDataFromFile(EmployeesFileName);
		string AccountNumber = ReadEmployeeAccountNumber();

		UpdateEmployeeByAccountNumber(AccountNumber, vEmployees);

		cout << "\n\tDo you want to update other Employees? Y/N? ";
		cin >> AddMore;

	} while (toupper(AddMore) == 'Y');

}

void ShowFindEmployeeScreen()
{
	cout << "  ============================================================================================  \n";
	cout << "                                       Find Employee                                            \n";
	cout << "  ============================================================================================  \n";

	char AddMore = 'Y';
	do
	{
		vector <sEmployee> vEmployees = LoadEmployeeDataFromFile(EmployeesFileName);
		sEmployee Employee;
		string AccountNumber = ReadEmployeeAccountNumber();

		if (FindEmployeeByAccountNumber(AccountNumber, vEmployees, Employee))
			PrintEmployeeCard(Employee);
		else
			cout << "\n\tEmployee with Account Number[" << AccountNumber << "] is not found!";

		cout << "\n\tDo you want to find other Employees? Y/N? ";
		cin >> AddMore;

	} while (toupper(AddMore) == 'Y');

}

///*                 Department Transactions On Employees                     *///

void ShowTransactionsMenue();

enum enTransactionsMenueOption
{
	Deposit = 1, Withdraw = 2,
	TotalBalance = 3, MenueOfEmployee = 4
};

void PrintDepositOnScreen()
{
	system("cls");
	cout << "  ============================================================================================  \n";
	cout << "                                         Deposit:                                               \n";
	cout << "  ============================================================================================  \n";

}

bool DepositBalanceToEmployeeByAccountNumber(string AccountNumber, double Amount, vector <sEmployee>& vEmployees)
{


	char Answer = 'n';


	cout << "\n\n\tAre you sure you want perfrom this transaction? y/n ? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
	{

		for (sEmployee& C : vEmployees)
		{
			if (C.AccountNumber == AccountNumber)
			{
				C.AccountBalance += Amount;
				SaveEmployeeDataToFile(EmployeesFileName, vEmployees);
				cout << "\n\n\tDone Successfully. New balance is: " << C.AccountBalance;

				return true;
			}

		}


		return false;
	}

}

void ShowDepositScreen()
{
	sEmployee Employee;

	vector <sEmployee> vEmployees = LoadEmployeeDataFromFile(EmployeesFileName);
	string AccountNumber = ReadEmployeeAccountNumber();


	while (!FindEmployeeByAccountNumber(AccountNumber, vEmployees, Employee))
	{
		cout << "\n\tEmployee with [" << AccountNumber << "] does not exist.\n";
		AccountNumber = ReadEmployeeAccountNumber();
	}


	PrintEmployeeCard(Employee);

	double Amount = 0;
	cout << "\n\tPlease enter deposit amount? ";
	cin >> Amount;

	DepositBalanceToEmployeeByAccountNumber(AccountNumber, Amount, vEmployees);

}

void GoToTransactionsMenue()
{
	cout << "\n\n\tPress any key to go back to Transactions Menue...";
	system("pause>0");
	ShowTransactionsMenue();

}

void PrintWithdrawOnScreen()
{
	system("cls");
	cout << "  ============================================================================================  \n";
	cout << "                                     Withdraw Screen :                                          \n";
	cout << "  ============================================================================================  \n";

}

void ShowWithDrawScreen()
{
	sEmployee Employee;

	vector <sEmployee> vEmployees = LoadEmployeeDataFromFile(EmployeesFileName);
	string AccountNumber = ReadEmployeeAccountNumber();


	while (!FindEmployeeByAccountNumber(AccountNumber, vEmployees, Employee))
	{
		cout << "\n\tEmployee with [" << AccountNumber << "] does not exist.\n";
		AccountNumber = ReadEmployeeAccountNumber();
	}
	PrintEmployeeCard(Employee);

	double Amount = 0;
	cout << "\n\tPlease enter deposit amount? ";
	cin >> Amount;

	//Validate that the amount does not exceeds the balance
	while (Amount > Employee.AccountBalance)
	{
		cout << "\n\tAmount Exceeds the balance, you can withdraw up to : " << Employee.AccountBalance << endl;
		cout << "\tPlease enter another amount? ";
		cin >> Amount;
	}

	DepositBalanceToEmployeeByAccountNumber(AccountNumber, Amount * -1, vEmployees);

}

void PrintBalanceRecordLine(sEmployee Employee)
{
	cout << "| " << setw(15) << left << Employee.AccountNumber;
	cout << "| " << setw(40) << left << Employee.Name;
	cout << "| " << setw(12) << left << Employee.AccountBalance;

}

void ShowAllBalanceScreen()
{
	system("cls");

	vector <sEmployee> vEmployees = LoadEmployeeDataFromFile(EmployeesFileName);

	cout << "\n\t\t\t\t\tEmployee List (" << vEmployees.size() << ") Employee(s).";
	cout << "\n_____________________________________________________";
	cout << "_______________________________________________\n" << endl;

	cout << "| " << left << setw(15) << "Accout Number";
	cout << "| " << left << setw(40) << "Employee Name";
	cout << "| " << left << setw(12) << "Balance";
	cout << "\n_____________________________________________________";
	cout << "_______________________________________________\n" << endl;

	if (vEmployees.size() == 0)
		cout << "\t\t\t\tNo Employees Available In the Hospital..!";
	else
	{
		double Sum = 0.0;

		for (sEmployee Employee : vEmployees)
		{
			Sum += Employee.AccountBalance;
			PrintBalanceRecordLine(Employee);
			cout << endl;
		}

		cout << "\n_____________________________________________________";
		cout << "_______________________________________________\n" << endl;

		cout << "\n\t\t\t\tSalary of all employees = " << Sum << "$";

	}

}

void PerfromTransactionsMenueOption(enTransactionsMenueOption TransactionsMenueOption)
{

	switch (TransactionsMenueOption)
	{
	case Deposit:
	{
		if (GeneralUser.Username == "Admin")
		{
			PrintDepositOnScreen();
			ShowDepositScreen();
			GoToTransactionsMenue();
			break;
		}
		else
		{
			cout << "\n\n\tYou Cannot access this Choice..!";
			GoToTransactionsMenue();
		}
	}
	case Withdraw:
	{
		if (GeneralUser.Username == "Admin")
		{
			PrintWithdrawOnScreen();
			ShowWithDrawScreen();
			GoToTransactionsMenue();
			break;
		}
		else
		{
			cout << "\n\n\tYou Cannot access this Choice..!";
			GoToTransactionsMenue();
		}
	}
	case TotalBalance:
	{
		if (GeneralUser.Username == "Admin")
		{
			ShowAllBalanceScreen();
			GoToTransactionsMenue();
			break;
		}
		else
		{
			cout << "\n\n\tYou Cannot access this Choice..!";
			GoToTransactionsMenue();
		}
	}
	case MenueOfEmployee:
	{
		ShowMenueOfEmployee();

		break;
	}
	}

}

void ShowTransactionsMenue()
{
	system("cls");

	cout << "  ============================================================================================  \n";
	cout << "                                     Transactions Menue                                         \n";
	cout << "  ============================================================================================  \n";
	cout << "        [1] Deposit.                                                                            \n";
	cout << "        [2] Withdraw.                                                                           \n";
	cout << "        [3] Total Balance.                                                                      \n";
	cout << "        [4] Menue Of Employee.                                                                  \n";
	cout << "  ============================================================================================  \n";

	PerfromTransactionsMenueOption((enTransactionsMenueOption)ReadMenueOfEmployeeOption(1, 4));

}

void PerfromMenueOfEmployeeOption(enMenueOfEmployeeOptions MenueOfEmployeeOption)
{
	switch (MenueOfEmployeeOption)
	{
	case eListEmployees:
	{
		system("cls");
		ShowAllEmployeesScreen();
		GoBackToMenueOfEmployee();
		break;
	}
	case eAddNewEmployee:
	{
		system("cls");
		ShowAddNewEmployeesScreen();
		GoBackToMenueOfEmployee();
		break;
	}
	case eDeleteEmployee:
	{
		system("cls");
		ShowDeleteEmployeeScreen();
		GoBackToMenueOfEmployee();
		break;
	}
	case eUpdateEmployee:
	{
		system("cls");
		ShowUpdateEmployeeScreen();
		GoBackToMenueOfEmployee();
		break;
	}
	case eFindEmployee:
	{
		system("cls");
		ShowFindEmployeeScreen();
		GoBackToMenueOfEmployee();
		break;
	}
	case eTransaction:
	{
		system("cls");
		ShowTransactionsMenue();

		break;
	}
	case eReturnMainMenue:
	{
		system("cls");
		ShowMainMenue();
		break;
	}
	}

}

void ShowMenueOfEmployee()
{
	system("cls");
	cout << "  ============================================================================================  \n";
	cout << "                                    Employees of Hospital                                       \n";
	cout << "  ============================================================================================  \n";
	cout << "                 [1] Show Employees List.		                             \n";
	cout << "                 [2] Add New Employee.			                             \n";
	cout << "                 [3] Delete Employee.			                             \n";
	cout << "                 [4] Update Employee Info.		                             \n";
	cout << "                 [5] Find Employee.			                             \n";
	cout << "                 [6] Transactiond.				                             \n";
	cout << "                 [7] Main Menue.				                             \n";
	cout << "  ============================================================================================  \n\n";

	PerfromMenueOfEmployeeOption((enMenueOfEmployeeOptions)ReadMenueOfEmployeeOption(1, 7));

}

///*                        Department Patients                             *///

const string PatientsFileName = "Data\\Patients.txt";

void ShowMenueOfPatient();

enum enMenueOfPatientOptions
{
	eListPatients = 1, eAddNewPatient = 2,
	eDeletePatient = 3, eFindPatient = 4,
	eReturnToMainMenie = 5
};

struct sPatient
{
	string Name;
	string RoomNumber;
	string Phone;
	string DateEntertoHospital;
	bool MarkForDelete = false;

};

short ReadMenueOfPatientOption(short From, short End)
{
	short Choice = 0;
	cout << "Choose what do you want to do From " << From << " To " << End << " : ";
	cin >> Choice;

	return Choice;
}

sPatient ConvertLinetoRecordPatient(string Line, string Seperator = "-")
{
	sPatient Patient;
	vector<string> vPatientData;
	vPatientData = SplitString(Line, Seperator);

	Patient.RoomNumber = vPatientData[0];
	Patient.Name = vPatientData[1];
	Patient.Phone = vPatientData[2];
	Patient.DateEntertoHospital = vPatientData[3];

	return Patient;

}

vector <sPatient> LoadPatientsDataFromFile(string FileName)
{
	vector <sPatient> vPatients;
	fstream MyFile;

	MyFile.open(FileName, ios::in);//read Mode

	if (MyFile.is_open())
	{
		string Line;
		sPatient Patient;

		while (getline(MyFile, Line))
		{
			Patient = ConvertLinetoRecordPatient(Line);
			vPatients.push_back(Patient);
		}
		MyFile.close();
	}
	return vPatients;
}

void PrintPatientRecordLine(sPatient Patient)
{
	cout << "| " << setw(15) << left << Patient.RoomNumber;
	cout << "| " << setw(40) << left << Patient.Name;
	cout << "| " << setw(12) << left << Patient.Phone;
	cout << "| " << setw(12) << left << Patient.DateEntertoHospital;

}

void ShowAllPatientsScreen()
{
	vector <sPatient> vPatients = LoadPatientsDataFromFile(PatientsFileName);

	cout << "\n\t\t\t\t\tPatient List (" << vPatients.size() << ") Patient(s).";
	cout << "\n_____________________________________________________";
	cout << "_______________________________________________\n" << endl;

	cout << "| " << left << setw(15) << "Room Number";
	cout << "| " << left << setw(40) << "Patient Name";
	cout << "| " << left << setw(12) << "Phone";
	cout << "| " << left << setw(12) << "Date";
	cout << "\n_____________________________________________________";
	cout << "_______________________________________________\n" << endl;

	if (vPatients.size() == 0)
		cout << "\t\t\t\tNo Patients Available In the Hospital..!";
	else

		for (sPatient Patient : vPatients)
		{

			PrintPatientRecordLine(Patient);
			cout << endl;
		}

	cout << "\n_____________________________________________________";
	cout << "_______________________________________________\n" << endl;


}

void GoBackToMenueOfPatient()
{
	cout << "\n\n\tPress any key to go back to Menue Of Patient...";
	system("pause>0");
	ShowMenueOfPatient();

}

///*                        Get On The Date For Patient                             *///

struct stDate {
	short Year;
	short Month;
	short Day;
};

short ReadYear()
{
	short y;
	cout << "\tPLease enter a year : ";
	cin >> y;
	return y;
}

short ReadMonth()
{
	short m;
	cout << "\tPlease Enter a month: ";
	cin >> m;
	return m;
}

short ReadDay()
{
	short d;
	cout << "\tPlease Enter a day : ";
	cin >> d;
	return d;
}

stDate ReadFullDate()
{
	stDate Date;

	Date.Day = ReadDay();
	Date.Month = ReadMonth();
	Date.Year = ReadYear();
	return Date;
}

stDate GetSystemDate()
{
	stDate Date;
	time_t t = time(0);
	tm* now = localtime(&t);
	Date.Year = now->tm_year + 1900;
	Date.Month = now->tm_mon + 1;
	Date.Day = now->tm_mday;
	return Date;
}

bool IsDat1BeforDate2(stDate Date1, stDate Date2)
{
	return (Date1.Year < Date2.Year) ? true : ((Date1.Year == Date2.Year) ?
		(Date1.Month < Date2.Month ? true : (Date1.Month == Date2.Month ? Date1.Day <
			Date2.Day : false)) : false);
}

bool IsLeapYear(short year)
{
	return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0) ? true : false;
}

short NumberDaysInMonth(short year, short month)
{
	if (month < 1 || month > 12)
		return 0;
	short ArrMon[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	return (month == 2) ? (IsLeapYear(year) ? 29 : 28) : ArrMon[month - 1];
}

bool IsLastDayInMonth(stDate Date)
{
	return Date.Day == NumberDaysInMonth(Date.Year, Date.Month);
}

bool IsLastMonthInYear(short month)
{
	return (month == 12);
}

bool IsValideDate(stDate Date)
{
	return Date.Day > 0 && Date.Day <= (NumberDaysInMonth(Date.Year, Date.Month));
}

stDate IncreaseDateByOneDay(stDate Date)
{
	if (IsLastDayInMonth(Date))
	{
		if (IsLastMonthInYear(Date.Month))
		{
			Date.Day = 1;
			Date.Month = 1;
			Date.Year++;
		}
		else
		{
			Date.Day = 1;
			Date.Month++;
		}
	}
	else
	{
		Date.Day++;
	}
	return Date;
}

short GetDifferenceInDays(stDate Date1, stDate Date2, bool includeEndDay = false)
{
	short days = 0;
	while (IsDat1BeforDate2(Date1, Date2))
	{
		days++;
		Date1 = IncreaseDateByOneDay(Date1);
	}
	return includeEndDay ? ++days : days;
}

stDate GetValidDate()
{
	stDate Date;

	short IsFail = 0;

	do
	{
		if (IsFail > 0)
		{
			cout << "\n\tNO, Date is a NOT valide date. Try Again: \n\n";
		}

		Date = ReadFullDate();
		IsFail++;

	} while (!(IsValideDate(Date)));

	return Date;

}

string DateToString(stDate Date, string Delimi)
{
	return to_string(Date.Day) + Delimi + to_string(Date.Month) + Delimi +
		to_string(Date.Year);
}

stDate StringToDate(string SD)
{
	vector<string>vString;
	stDate Date;
	vString = SplitString(SD, "/");

	Date.Day = stoi(vString[0]);
	Date.Month = stoi(vString[1]);
	Date.Year = stoi(vString[2]);

	return Date;
}

bool PatientExistsByRoomNumber(string RoomNumber, string FileName)
{

	vector <sPatient> vPatients;
	fstream MyFile;

	MyFile.open(FileName, ios::in);//read Mode

	if (MyFile.is_open())
	{
		string Line;
		sPatient Patient;

		while (getline(MyFile, Line))
		{
			Patient = ConvertLinetoRecordPatient(Line);
			if (Patient.RoomNumber == RoomNumber)
			{
				MyFile.close();
				return true;
			}
			vPatients.push_back(Patient);
		}

		MyFile.close();

	}
	return false;
}

sPatient ReadNewPatient()

{
	sPatient Patient;
	cout << "\tEnter Room Number? ";

	// Usage of std::ws will extract allthe whitespace character
	getline(cin >> ws, Patient.RoomNumber);

	while (PatientExistsByRoomNumber(Patient.RoomNumber, PatientsFileName))
	{
		cout << "\n\tRoom Number with [" << Patient.RoomNumber << "] already exists, Enter another Room Number? ";
		getline(cin >> ws, Patient.RoomNumber);
	}

	cout << "\tEnter Patients Name? ";
	getline(cin, Patient.Name);

	cout << "\tEnter Phone Number? ";
	getline(cin, Patient.Phone);

	cout << "\n\tEnter Date: \n";
	Patient.DateEntertoHospital = DateToString(GetValidDate(), "/");

	return Patient;
}

string ConvertRecordToLinePatient(sPatient Patient, string Seperator = "-")
{
	string stPatientRecord = "";
	stPatientRecord += Patient.RoomNumber + Seperator;
	stPatientRecord += Patient.Name + Seperator;
	stPatientRecord += Patient.Phone + Seperator;
	stPatientRecord += Patient.DateEntertoHospital;


	return stPatientRecord;

}

void AddNewPatient()
{
	sPatient Patient;
	Patient = ReadNewPatient();
	AddDataLineToFile(PatientsFileName, ConvertRecordToLinePatient(Patient));

}

void AddNewPatients()
{
	char AddMore = 'Y';
	do
	{
		cout << "\tAdding New Patient:\n\n";

		AddNewPatient();
		cout << "\n\tPatient Added Successfully, do you want to add more Patients? Y/N? ";
		cin >> AddMore;

	} while (toupper(AddMore) == 'Y');

}

void ShowAddNewPatientsScreen()
{

	cout << "  ============================================================================================  \n";
	cout << "                                       Add New Patients                                         \n";
	cout << "  ============================================================================================  \n";

	AddNewPatients();
}

string ReadPatientRoomNumber()
{
	string RoomNumber = "";

	cout << "\n\tPlease enter Room Number? ";
	cin >> RoomNumber;

	return RoomNumber;
}

bool FindPatientByRoomNumber(string RoomNumber, vector <sPatient> vPatients, sPatient& Patient)
{
	for (sPatient C : vPatients)
	{

		if (C.RoomNumber == RoomNumber)
		{
			Patient = C;
			return true;
		}

	}

	return false;
}

void PrintPatientCard(sPatient Patient)
{
	cout << "\n\tThe following are the Patient details:\n";
	cout << "\t-------------------------------------------------------";
	cout << "\n\tName         : " << Patient.Name;
	cout << "\n\tRoom Number  : " << Patient.RoomNumber;
	cout << "\n\tPhone        : " << Patient.Phone;
	cout << "\n\tDate         : " << Patient.DateEntertoHospital;
	cout << "\n\t-----------------------------------------------------\n";

}

bool MarkPatientForDeleteByRoomNumber(string RoomNumber, vector <sPatient>& vPatients)
{

	for (sPatient& C : vPatients)
	{

		if (C.RoomNumber == RoomNumber)
		{
			C.MarkForDelete = true;
			return true;
		}

	}

	return false;
}

vector <sPatient> SavePatientsDataToFile(string FileName, vector <sPatient>& vPatients)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);//overwrite

	string DataLine;

	if (MyFile.is_open())
	{
		for (sPatient C : vPatients)
		{

			if (C.MarkForDelete == false)
			{
				//we only write records that are not marked for delete.  
				DataLine = ConvertRecordToLinePatient(C);
				MyFile << DataLine << endl;
			}

		}

		MyFile.close();
	}

	return vPatients;
}

bool DeletePatientByRoomNumber(string RoomNumber, vector <sPatient>& vPatients)
{
	sPatient Patient;
	char Answer = 'n';

	if (FindPatientByRoomNumber(RoomNumber, vPatients, Patient))
	{

		PrintPatientCard(Patient);

		cout << "\n\n\tAre you sure you want delete this Patient? y/n ? ";
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y')
		{
			MarkPatientForDeleteByRoomNumber(RoomNumber, vPatients);
			SavePatientsDataToFile(PatientsFileName, vPatients);

			//Refresh Patients 
			vPatients = LoadPatientsDataFromFile(PatientsFileName);

			cout << "\n\n\tPatient Deleted Successfully.";
			return true;
		}

	}
	else
	{
		cout << "\n\tPatient with Account Number (" << RoomNumber << ") is Not Found!";
		return false;
	}

}

void ShowDeletePatientScreen()
{
	cout << "  ============================================================================================  \n";
	cout << "                                         Delete Patient                                         \n";
	cout << "  ============================================================================================  \n";

	vector <sPatient> vPatients = LoadPatientsDataFromFile(PatientsFileName);
	string RoomNumber = ReadPatientRoomNumber();
	DeletePatientByRoomNumber(RoomNumber, vPatients);

}

void ShowFindPatientScreen()
{
	cout << "  ============================================================================================  \n";
	cout << "                                      Find Patient                                              \n";
	cout << "  ============================================================================================  \n";

	vector <sPatient> vPatients = LoadPatientsDataFromFile(PatientsFileName);
	sPatient Patient;
	string RoomNumber = ReadPatientRoomNumber();
	if (FindPatientByRoomNumber(RoomNumber, vPatients, Patient))
		PrintPatientCard(Patient);
	else
		cout << "\n\tPatient with Room Number[" << RoomNumber << "] is not found!";

}

void PerfromMenueOfPatientOption(enMenueOfPatientOptions MenueOfPatientOption)
{
	switch (MenueOfPatientOption)
	{
	case eListPatients:
	{
		system("cls");
		ShowAllPatientsScreen();
		GoBackToMenueOfPatient();
		break;
	}
	case eAddNewPatient:
	{
		system("cls");
		ShowAddNewPatientsScreen();
		GoBackToMenueOfPatient();
		break;
	}
	case eDeletePatient:
	{
		system("cls");
		ShowDeletePatientScreen();
		GoBackToMenueOfPatient();
		break;
	}
	case eFindPatient:
	{
		system("cls");
		ShowFindPatientScreen();
		GoBackToMenueOfPatient();
		break;
	}
	case eReturnToMainMenie:
	{
		system("cls");
		ShowMainMenue();
		break;
	}
	}

}

void ShowMenueOfPatient()
{
	system("cls");
	cout << "  ============================================================================================  \n";
	cout << "                                             The Patients                                       \n";
	cout << "  ============================================================================================  \n";
	cout << "                   [1] Show Patient List.                        \n";
	cout << "                   [2] Add New Patient.                          \n";
	cout << "                   [3] Delete Patient.                           \n";
	cout << "                   [4] Find Patient.                             \n";
	cout << "                   [5] Main Menue.                               \n";
	cout << "  ============================================================================================  \n\n";

	PerfromMenueOfPatientOption((enMenueOfPatientOptions)ReadMenueOfPatientOption(1, 5));

}

///*                       Department Operations                                 *///

const string OperationsFileName = "Data\\TheOperations.txt";
const string FinisedOperationsFileName = "Data\\FinisedOperations.txt";

const float TheCostOfOneDayInTheHospital = 100.000;
const float OpetationCostInHospital = 3000.000;

void ShowMenueOfOperations();

void CalculationOfTheBill(string);

enum enMenueOfOperations
{
	eAddOperation = 1, eShowAllOperations = 2, eShowFinishedOperations = 3,
	eRetuenToMainFromOptation = 4
};

struct sOperation
{
	string NumberOfRoom;
	string DoctorName;
	string PatientsName;
	string NameOfOperation;
	string DateOfOperation;

	bool MarkForDelete = false;
};

short ReadMenueOfOperationsOption(short From, short End)
{
	short Choice = 0;
	cout << "Choose what do you want to do From " << From << " To " << End << " : ";
	cin >> Choice;

	return Choice;
}

string ReadDoctorByAccountNumber()
{
	cout << "\n";
	cout << "\t_________________________________________\n\n";
	cout << "\t[A150] Othman      |	 [A151] Omran   \n";
	cout << "\t[A152] Hassan      |	 [A153] Ali     \n";
	cout << "\t[A154] Ahmad       |	 [A155] Mohammad\n";
	cout << "\t[A156] Kaser       |	 [A157] Monir   \n";
	cout << "\t[A158] Anas        |	 [A159] Rami    \n";
	cout << "\t_________________________________________\n";

	string s;
	cout << "\n\tPlease Enter Account Number For Doctor From (A150 to A159) : ";
	cin >> s;

	return s;
}

sOperation ReadNewOperation(bool& IsNotEmpty)
{
	sOperation Operation;

	vector <sPatient> vPatients = LoadPatientsDataFromFile(PatientsFileName);
	sPatient Patient;
	string RoomNumber = ReadPatientRoomNumber();

	if (FindPatientByRoomNumber(RoomNumber, vPatients, Patient))
	{
		if (PatientExistsByRoomNumber(RoomNumber, OperationsFileName))
		{
			cout << "\n\tRoom Number with [" << Patient.RoomNumber << "] Realy Full, Enter another Room Number? ";
			getline(cin >> ws, Patient.RoomNumber);
		}
		Operation.NumberOfRoom = Patient.RoomNumber;
		Operation.PatientsName = Patient.Name;
		Operation.DateOfOperation = Patient.DateEntertoHospital;

		cout << "\tEnter Name Of Operation: ";
		getline(cin >> ws, Operation.NameOfOperation);


		vector <sEmployee> vEmployees = LoadEmployeeDataFromFile(EmployeesFileName);
		sEmployee Employee;
		string AccountNumber;

		do
		{
			AccountNumber = ReadDoctorByAccountNumber();

		} while (!FindEmployeeByAccountNumber(AccountNumber, vEmployees, Employee));

		Operation.DoctorName = Employee.Name;

		cout << "\n\n\n\t\t\tThe Addition Was Successful :-)\n";
		IsNotEmpty = true;

	}
	else
	{
		IsNotEmpty = false;
		cout << "\n\tThis Room is Empty..!  , The Addition Failed :-(";
	}

	return Operation;
}

string ConvertRecordToLineOperation(sOperation Operation, string Seperator = "-")
{
	string stOperationRecord = "";
	stOperationRecord += Operation.NumberOfRoom + Seperator;
	stOperationRecord += Operation.DoctorName + Seperator;
	stOperationRecord += Operation.PatientsName + Seperator;
	stOperationRecord += Operation.NameOfOperation + Seperator;
	stOperationRecord += Operation.DateOfOperation;

	return stOperationRecord;
}

void AddNewOperation()
{
	bool IsNotEmpty;

	sOperation Operation;
	Operation = ReadNewOperation(IsNotEmpty);

	if (IsNotEmpty)
		AddDataLineToFileTheSrting(OperationsFileName, ConvertRecordToLineOperation(Operation));
	else
		cout << "\n";
}

void AddNewOperations()
{

	cout << "\tAdding New Operation:\n\n";
	AddNewOperation();
}

void ShowAddNewOperationScreen()
{
	cout << "  ============================================================================================  \n";
	cout << "                               Add New Operation                                                \n";
	cout << "  ============================================================================================  \n";

	AddNewOperations();
}

void GoBackToMenueOfOperation()
{
	cout << "\n\n\tPress any key to go back to Menue Of Operations...";
	system("pause>0");
	ShowMenueOfOperations();

}

sOperation ConvertLinetoRecordOperation(string Line, string Seperator = "-")
{
	sOperation Operation;
	vector<string> vOperationData;
	vOperationData = SplitString(Line, Seperator);

	Operation.NumberOfRoom = vOperationData[0];
	Operation.DoctorName = vOperationData[1];
	Operation.PatientsName = vOperationData[2];
	Operation.NameOfOperation = vOperationData[3];
	Operation.DateOfOperation = vOperationData[4];

	return Operation;

}

vector <sOperation> LoadOperationDataFromFile(string FileName)
{
	vector <sOperation> vOperation;
	fstream MyFile;

	MyFile.open(FileName, ios::in);//read Mode

	if (MyFile.is_open())
	{
		string Line;
		sOperation Operation;

		while (getline(MyFile, Line))
		{
			Operation = ConvertLinetoRecordOperation(Line);
			vOperation.push_back(Operation);
		}
		MyFile.close();
	}
	return vOperation;
}

void PrintOperationRecordLine(sOperation Operation)
{
	cout << "| " << setw(15) << left << Operation.NumberOfRoom;
	cout << "| " << setw(20) << left << Operation.DoctorName;
	cout << "| " << setw(20) << left << Operation.PatientsName;
	cout << "| " << setw(30) << left << Operation.NameOfOperation;

}

void PrintRecordLineForFinishedOfOperation(sOperation Operation)
{
	cout << "| " << setw(15) << left << Operation.NumberOfRoom;
	cout << "| " << setw(20) << left << Operation.DoctorName;
	cout << "| " << setw(20) << left << Operation.PatientsName;
	cout << "| " << setw(30) << left << Operation.NameOfOperation;
	cout << "| " << setw(11) << left << Operation.DateOfOperation;

}

bool FindOperationByDoctorNameOfOperation(string Name, sOperation& Operation)
{

	vector <sOperation> vOperation = LoadOperationDataFromFile(OperationsFileName);

	for (sOperation O : vOperation)
	{

		if (O.DoctorName == Name)
		{
			Operation = O;
			return true;
		}

	}
	return false;

}

bool FindOperationByDoctorNameOfOperation(string Name, vector <sOperation> vOperation, sOperation& Operation)
{
	for (sOperation O : vOperation)
	{

		if (O.DoctorName == Name)
		{
			Operation = O;
			return true;
		}

	}

	return false;
}

bool MarkOperationForDeleteByDoctorNameOfOperation(string Name, vector <sOperation>& vOperation)
{

	for (sOperation& O : vOperation)
	{

		if (O.DoctorName == Name)
		{
			O.MarkForDelete = true;
			return true;
		}

	}

	return false;
}

vector <sOperation> SaveOperationDataToFile(string FileName, vector <sOperation> vOperation)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);//overwrite

	string DataLine;

	if (MyFile.is_open())
	{
		for (sOperation O : vOperation)
		{

			if (O.MarkForDelete == false)
			{
				//we only write records that are not marked for delete.  
				DataLine = ConvertRecordToLineOperation(O);
				MyFile << DataLine << endl;
			}

		}

		MyFile.close();
	}

	return vOperation;
}

bool RemoveOperationByDoctorNameOfOperation(string Name, vector <sOperation>& vOperation)
{
	sOperation Operation;
	char Answer = 'n';

	if (FindOperationByDoctorNameOfOperation(Name, vOperation, Operation))
	{

		MarkOperationForDeleteByDoctorNameOfOperation(Name, vOperation);
		SaveOperationDataToFile(OperationsFileName, vOperation);

		//Refresh Employees 
		vOperation = LoadOperationDataFromFile(OperationsFileName);

		return true;

	}


}

void IsOperationFinished()
{
	sOperation Operation;
	string DoctorNameOfOperation;


	char Are = 'n';

	cout << "\n\tDo there Any Operation Over? (y/n) ? ";
	cin >> Are;
	if (toupper(Are) == 'Y')
	{
		cout << "\n\tPlease Enter Doctor's Name Of Operation: ";
		getline(cin >> ws, DoctorNameOfOperation);

		if (FindOperationByDoctorNameOfOperation(DoctorNameOfOperation, Operation))
		{

			AddDataLineToFileTheSrting(FinisedOperationsFileName, ConvertRecordToLineOperation(Operation));

			vector <sOperation> vOperation = LoadOperationDataFromFile(OperationsFileName);

			RemoveOperationByDoctorNameOfOperation(DoctorNameOfOperation, vOperation);
			cout << "\n\n\tNice, I wish you good gealth... " << char(3) << endl;


			CalculationOfTheBill(Operation.NumberOfRoom);

		}
		else
		{
			cout << "\n\tOperation (" << DoctorNameOfOperation << ") is Not Found!";
		}
	}


}

void ShowAllOperationsScreen()
{
	vector <sOperation> vOperation = LoadOperationDataFromFile(OperationsFileName);

	cout << "\n\t\t\t\tOperation List (" << vOperation.size() << ") Operation(s).";
	cout << "\n_____________________________________________________";
	cout << "__________________________________________\n" << endl;

	cout << "| " << left << setw(15) << "Room Number";
	cout << "| " << left << setw(20) << "Doctor's Name";
	cout << "| " << left << setw(20) << "Patient's Name";
	cout << "| " << left << setw(30) << "Name Of The Operation";
	cout << "\n_____________________________________________________";
	cout << "__________________________________________\n" << endl;

	if (vOperation.size() == 0)
		cout << "\t\t\t  No Operations Available In the Hospital..!";
	else

		for (sOperation Operation : vOperation)
		{

			PrintOperationRecordLine(Operation);
			cout << endl;
		}

	cout << "\n_____________________________________________________";
	cout << "__________________________________________\n" << endl;

	if (vOperation.size() != 0 && GeneralUser.Username == "Admin")
		 IsOperationFinished();


}

void ShowAllOperationsFinishedScreen()
{
	vector <sOperation> vOperation = LoadOperationDataFromFile(FinisedOperationsFileName);

	cout << "\n\t\t\t  Operations Finished List (" << vOperation.size() << ") Operation(s).";
	cout << "\n_____________________________________________________";
	cout << "_____________________________________________________\n" << endl;

	cout << "| " << left << setw(15) << "Room Number";
	cout << "| " << left << setw(20) << "Doctor's Name";
	cout << "| " << left << setw(20) << "Patient's Name";
	cout << "| " << left << setw(30) << "Name Of The Operation";
	cout << "| " << left << setw(11) << "The Date";
	cout << "\n_____________________________________________________";
	cout << "_____________________________________________________\n" << endl;

	if (vOperation.size() == 0)
		cout << "\t\t\tNo Operations Available In the Hospital..!";
	else

		for (sOperation Operation : vOperation)
		{

			PrintRecordLineForFinishedOfOperation(Operation);
			cout << endl;
		}

	cout << "\n_____________________________________________________";
	cout << "_____________________________________________________\n" << endl;

}

void PerfromMenueOfOperatinsOption(enMenueOfOperations MenueOfOperation)
{
	switch (MenueOfOperation)
	{
	case eAddOperation:
	{
		system("cls");
		ShowAddNewOperationScreen();
		GoBackToMenueOfOperation();
		break;
	}
	case eShowAllOperations:
	{
		system("cls");
		ShowAllOperationsScreen();
		GoBackToMenueOfOperation();
		break;
	}
	case eShowFinishedOperations:
	{
		system("cls");
		ShowAllOperationsFinishedScreen();
		GoBackToMenueOfOperation();
		break;
	}
	case eRetuenToMainFromOptation:
	{
		system("cls");
		ShowMainMenue();
		break;
	}

	}
}

void ShowMenueOfOperations()
{
	system("cls");
	cout << "  ============================================================================================  \n";
	cout << "                                Operations Department                                           \n";
	cout << "  ============================================================================================  \n";
	cout << "                   [1] Add New Operation.                                                       \n";
	cout << "                   [2] Show All Operations.                                                     \n";
	cout << "                   [3] Show Finised Operations.                                                 \n";
	cout << "                   [4] Main Menue.                                                              \n";
	cout << "  ============================================================================================  \n";
	PerfromMenueOfOperatinsOption((enMenueOfOperations)ReadMenueOfOperationsOption(1, 4));

}

///*                         Department Pharmaceutical                              *///

const string PharmaceuticalFileName = "Data\\Pharmaceutical.txt";
const string ShortagePharmaceuticalFileName = "Data\\ShortagePharmaceutical.txt";

void ShowMenueOfPharmaceutical();

enum enMenueOfPharmaceuticalOptions
{
	eListOfShortageOfMedication = 1, eListPharmaceutical = 2,
	eAddNewPharmaceutical = 3, eAddingAmountToMedicine = 4,
	eTotalDrugPrice = 5, eReturnToMainMenue = 6
};

struct sPharmaceutical
{
	string MedicamentName;
	double MedicinePrice;
	int TheQuantity;

	bool MarkForDelete = false;
};

short ReadMenueOfPharmaceuticalOption(short From, short End)
{
	short Choice = 0;
	cout << "Choose what do you want to do From " << From << " To " << End << " : ";
	cin >> Choice;

	return Choice;
}

sPharmaceutical ConvertLinetoRecordPharmaceutical(string Line, string Seperator = "-")
{
	sPharmaceutical Pharmaceutical;
	vector<string> vPharmaceuticalData;
	vPharmaceuticalData = SplitString(Line, Seperator);

	Pharmaceutical.MedicamentName = vPharmaceuticalData[0];
	Pharmaceutical.MedicinePrice = stod(vPharmaceuticalData[1]);
	Pharmaceutical.TheQuantity = stoi(vPharmaceuticalData[2]);

	return Pharmaceutical;

}

vector <sPharmaceutical> LoadPharmaceuticalDataFromFile(string FileName)
{
	vector <sPharmaceutical> vPharmaceutical;
	fstream MyFile;

	MyFile.open(FileName, ios::in);//read Mode

	if (MyFile.is_open())
	{
		string Line;
		sPharmaceutical Pharmaceutical;

		while (getline(MyFile, Line))
		{
			Pharmaceutical = ConvertLinetoRecordPharmaceutical(Line);
			vPharmaceutical.push_back(Pharmaceutical);
		}
		MyFile.close();
	}
	return vPharmaceutical;
}

string ConvertRecordToLinePharmaceutical(sPharmaceutical Pharmaceutical, string Seperator = "-")
{
	string stPharmaceuticalRecord = "";
	stPharmaceuticalRecord += Pharmaceutical.MedicamentName + Seperator;
	stPharmaceuticalRecord += to_string(Pharmaceutical.MedicinePrice) + Seperator;
	stPharmaceuticalRecord += to_string(Pharmaceutical.TheQuantity);


	return stPharmaceuticalRecord;

}

void PrintBalanceForPharmaceuticalRecordLine(sPharmaceutical Pharmaceutical)
{
	cout << "| " << setw(25) << left << Pharmaceutical.MedicamentName;
	cout << "| " << setw(15) << left << Pharmaceutical.TheQuantity;
	cout << "| " << setw(20) << left << Pharmaceutical.MedicinePrice;

}

void ShowAllShortagePharmaceuticalScreen()
{
	vector <sPharmaceutical> vPharmaceutical = LoadPharmaceuticalDataFromFile(PharmaceuticalFileName);
	short NumberOfSmallQuantityDrugs = 0;
	for (sPharmaceutical Pharmaceutical : vPharmaceutical)
	{
		if (Pharmaceutical.TheQuantity < 10)
		{
			AddDataLineToFileTheSrting(ShortagePharmaceuticalFileName, ConvertRecordToLinePharmaceutical(Pharmaceutical));
			NumberOfSmallQuantityDrugs++;
		}
	}

	system("cls");

	cout << "\n\t\t\tMedications List (" << NumberOfSmallQuantityDrugs << ") Medication(s).";
	cout << "\n_____________________________________________________";
	cout << "_______________________________________\n" << endl;

	cout << "| " << left << setw(25) << "Medicament Name";
	cout << "| " << left << setw(15) << "The Quantity";
	cout << "| " << left << setw(20) << "Medicine Price";
	cout << "\n_____________________________________________________";
	cout << "_______________________________________\n" << endl;

	if (NumberOfSmallQuantityDrugs == 0)
		cout << "\t\t  No Pharmaceutical Available In the Hospital..!";
	else
	{
		for (sPharmaceutical Pharmaceutical : vPharmaceutical)
		{
			if (Pharmaceutical.TheQuantity < 10)
			{
				cout << "\a"; system("color 47");

				PrintBalanceForPharmaceuticalRecordLine(Pharmaceutical);
				cout << endl;
			}
		}
	}

	cout << "\n_____________________________________________________";
	cout << "_______________________________________\n" << endl;

}

void GoBackToMenueOfPharmaceutical()
{
	cout << "\n\n\tPress any key to go back to Menue Of Pharmaceutical...";
	system("pause>0");
	system("color 0F");
	ShowMenueOfPharmaceutical();

}

void PrintPharmaceuticalRecordLine(sPharmaceutical Pharmaceutical)
{
	cout << "| " << setw(20) << left << Pharmaceutical.MedicamentName;
	cout << "| " << setw(8) << left << Pharmaceutical.TheQuantity;
	cout << "| " << setw(15) << left << Pharmaceutical.MedicinePrice;

}

void ShowAllPharmaceuticalScreen()
{
	vector <sPharmaceutical> vPharmaceutical = LoadPharmaceuticalDataFromFile(PharmaceuticalFileName);


	cout << "\n\t\t\t\tPharmaceutical List (" << vPharmaceutical.size() << ") Pharmaceutical(s).";
	cout << "\n_____________________________________________________";
	cout << "_______________________________________________\n" << endl;

	cout << "| " << left << setw(20) << "Medicament Name";
	cout << "| " << left << setw(8) << "The Quantity";
	cout << "| " << left << setw(15) << "Medicine Price";
	cout << "\n_____________________________________________________";
	cout << "_______________________________________________\n" << endl;

	if (vPharmaceutical.size() == 0)
		cout << "\t\t  No Pharmaceutical Available In the Hospital..!";
	else

		for (sPharmaceutical Pharmaceutical : vPharmaceutical)
		{

			PrintPharmaceuticalRecordLine(Pharmaceutical);
			cout << endl;
		}

	cout << "\n_____________________________________________________";
	cout << "_______________________________________________\n" << endl;


}

bool PharmaceuticalExistsByMedicamentName(string MedicamentName, string FileName)
{

	vector <sPharmaceutical> vPharmaceutical;
	fstream MyFile;

	MyFile.open(FileName, ios::in);//read Mode

	if (MyFile.is_open())
	{
		string Line;
		sPharmaceutical Pharmaceutical;

		while (getline(MyFile, Line))
		{
			Pharmaceutical = ConvertLinetoRecordPharmaceutical(Line);
			if (Pharmaceutical.MedicamentName == MedicamentName)
			{
				MyFile.close();
				return true;
			}
			vPharmaceutical.push_back(Pharmaceutical);
		}

		MyFile.close();

	}
	return false;
}

sPharmaceutical ReadNewPharmaceutical()
{
	sPharmaceutical Pharmaceutical;

	cout << "\tEnter Medicament Name? ";

	// Usage of std::ws will extract allthe whitespace character
	getline(cin >> ws, Pharmaceutical.MedicamentName);

	while (PharmaceuticalExistsByMedicamentName(Pharmaceutical.MedicamentName, PharmaceuticalFileName))
	{
		cout << "\n\tMedicine with [" << Pharmaceutical.MedicamentName << "] already exists, Enter another Medicament Name? ";
		getline(cin >> ws, Pharmaceutical.MedicamentName);
	}

	cout << "\tEnter Medicine Price? ";
	cin >> Pharmaceutical.MedicinePrice;

	cout << "\tEnter The Quantity? ";
	cin >> Pharmaceutical.TheQuantity;

	double TheBill = Pharmaceutical.MedicinePrice * Pharmaceutical.TheQuantity;

	vector<sUser> vUser = LoadUsersDataFromFile(UserNameFile);
	AddMoneyToBankByPassword(GeneralUser.Username, TheBill * (-1), vUser);
	GeneralUser.AllMoneyInBank -= TheBill;


	return Pharmaceutical;
}

void AddNewMedicine()
{
	sPharmaceutical Pharmaceutical;
	Pharmaceutical = ReadNewPharmaceutical();
	AddDataLineToFileTheSrting(PharmaceuticalFileName, ConvertRecordToLinePharmaceutical(Pharmaceutical));
}

void AddNewPharmaceutical()
{
	char AddMore = 'Y';
	do
	{
		cout << "\tAdding New Medicine:\n\n";

		AddNewMedicine();
		cout << "\n\tMedicine Added Successfully, Do you want to add more Medicine? Y/N? ";
		cin >> AddMore;

	} while (toupper(AddMore) == 'Y');
}

void ShowAddNewPharmaceuticalScreen()
{
	cout << "  ============================================================================================  \n";
	cout << "                               Add New Pharmaceutical                                           \n";
	cout << "  ============================================================================================  \n";

	AddNewPharmaceutical();
}

string ReadPharmaceutical()
{
	string s;
	cout << "\n\tPlease enter name of Medicine: ";
	cin >> s;

	return s;
}

bool FindPharmaceuticalByName(string Medicine, vector <sPharmaceutical> vPharmaceutical, sPharmaceutical& Pharmaceutical)
{
	for (sPharmaceutical P : vPharmaceutical)
	{

		if (P.MedicamentName == Medicine)
		{
			Pharmaceutical = P;
			return true;
		}

	}

	return false;
}

void PrintPharmaceuticalCard(sPharmaceutical Pharmaceutical)
{
	cout << "\n\tThe following are the Medicine details:\n";
	cout << "\t-----------------------------------------------------";
	cout << "\n\tMedicine     : " << Pharmaceutical.MedicamentName;
	cout << "\n\tThe Quantity : " << Pharmaceutical.TheQuantity;
	cout << "\n\tMedicinePrice : " << Pharmaceutical.MedicinePrice;
	cout << "\n\t-----------------------------------------------------\n";

}

vector <sPharmaceutical> SavePharmaceuticalDataToFile(string FileName, vector <sPharmaceutical> vPharmaceutical)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);//overwrite

	string DataLine;

	if (MyFile.is_open())
	{
		for (sPharmaceutical P : vPharmaceutical)
		{

			if (P.MarkForDelete == false)
			{
				//we only write records that are not marked for delete.  
				DataLine = ConvertRecordToLinePharmaceutical(P);
				MyFile << DataLine << endl;
			}

		}

		MyFile.close();
	}

	return vPharmaceutical;
}

bool DepositQuantityToPharmaceuticalByName(string Medicine, double Amount, vector <sPharmaceutical>& vPharmaceutical)
{


	char Answer = 'n';


	cout << "\n\tAre you sure you want perfrom this transaction? y/n ? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
	{

		for (sPharmaceutical& P : vPharmaceutical)
		{
			if (P.MedicamentName == Medicine)
			{
				double TheBill = Amount * P.MedicinePrice;

				vector<sUser> vUser = LoadUsersDataFromFile(UserNameFile);
				AddMoneyToBankByPassword(GeneralUser.Username, TheBill * (-1), vUser);
				GeneralUser.AllMoneyInBank -= TheBill;

				P.TheQuantity += Amount;
				SavePharmaceuticalDataToFile(PharmaceuticalFileName, vPharmaceutical);
				cout << "\n\n\tDone Successfully. New quantity is: " << P.TheQuantity;

				return true;
			}

		}


		return false;
	}

}

void PrintAddingAmountToMedicine()
{
	system("cls");
	cout << "  ============================================================================================  \n";
	cout << "                               Adding Amount To Medicine                                        \n";
	cout << "  ============================================================================================  \n";

}

void ShoweAddingAmountToMedicine()
{
	sPharmaceutical Pharmaceutical;

	vector <sPharmaceutical> vPharmaceutical = LoadPharmaceuticalDataFromFile(PharmaceuticalFileName);

	PrintAddingAmountToMedicine();
	ShowAllPharmaceuticalScreen();

	string Medicine = ReadPharmaceutical();

	while (!FindPharmaceuticalByName(Medicine, vPharmaceutical, Pharmaceutical))
	{
		cout << "\n\tMedicine with [" << Medicine << "] does not exist.\n";
		Medicine = ReadPharmaceutical();
	}
	PrintAddingAmountToMedicine();

	PrintPharmaceuticalCard(Pharmaceutical);

	double Amount = 0;
	cout << "\n\tPlease enter the quantity? ";
	cin >> Amount;

	DepositQuantityToPharmaceuticalByName(Medicine, Amount, vPharmaceutical);
}

void ShowAllBalanceForPharmaceuticalScreen()
{
	system("cls");

	vector <sPharmaceutical> vPharmaceutical = LoadPharmaceuticalDataFromFile(PharmaceuticalFileName);

	cout << "\n\t\t\tMedications List (" << vPharmaceutical.size() << ") Medication(s).";
	cout << "\n_____________________________________________________";
	cout << "_______________________________________\n" << endl;

	cout << "| " << left << setw(25) << "Medicament Name";
	cout << "| " << left << setw(15) << "The Quantity";
	cout << "| " << left << setw(20) << "Medicine Price";
	cout << "\n_____________________________________________________";
	cout << "_______________________________________\n" << endl;

	if (vPharmaceutical.size() == 0)
		cout << "\t\t\t\tNo Pharmaceutical Available In the Hospital..!";
	else
	{
		double Sum = 0.0;

		for (sPharmaceutical Pharmaceutical : vPharmaceutical)
		{
			Sum += Pharmaceutical.MedicinePrice * Pharmaceutical.TheQuantity;
			PrintBalanceForPharmaceuticalRecordLine(Pharmaceutical);
			cout << endl;
		}

		cout << "\n_____________________________________________________";
		cout << "_______________________________________\n" << endl;

		cout << "\n\t\t\tSalary of all Pharmaceutical = " << Sum << "$" << endl;

	}

}

void PerfromMenueOfPharmaceuticalOption(enMenueOfPharmaceuticalOptions MenueOfPharmaceuticalOption)
{
	switch (MenueOfPharmaceuticalOption)
	{
	case eListOfShortageOfMedication:
	{
		system("cls");
		ShowAllShortagePharmaceuticalScreen();
		GoBackToMenueOfPharmaceutical();
		break;
	}
	case eListPharmaceutical:
	{
		system("cls");
		ShowAllPharmaceuticalScreen();
		GoBackToMenueOfPharmaceutical();
		break;
	}
	case eAddNewPharmaceutical:
	{
		if (GeneralUser.Username == "Admin")
		{
			system("cls");
			ShowAddNewPharmaceuticalScreen();
			GoBackToMenueOfPharmaceutical();
			break;
		}
		else
		{
			cout << "\n\n\tYou Cannot access this Choice..!";
			GoBackToMenueOfPharmaceutical();
		}
	}
	case eAddingAmountToMedicine:
	{
		if (GeneralUser.Username == "Admin")
		{
			system("cls");
			ShoweAddingAmountToMedicine();
			GoBackToMenueOfPharmaceutical();
			break;
		}
		else
		{
			cout << "\n\n\tYou Cannot access this Choice..!";
			GoBackToMenueOfPharmaceutical();
		}
	}
	case eTotalDrugPrice:
	{
		if (GeneralUser.Username == "Admin")
		{
			system("cls");
			ShowAllBalanceForPharmaceuticalScreen();
			GoBackToMenueOfPharmaceutical();
			break;
		}
		else
		{
			cout << "\n\n\tYou Cannot access this Choice..!";
			GoBackToMenueOfPharmaceutical();
		}
	}
	case eReturnToMainMenue:
	{
		system("cls");
		ShowMainMenue();
		break;
	}
	}
}

void ShowMenueOfPharmaceutical()
{
	system("cls");
	cout << "  ============================================================================================  \n";
	cout << "                                       The Pharmaceutical                                       \n";
	cout << "  ============================================================================================  \n";
	cout << "                   [1] Show List Of Shortage Of Medication.                                     \n";
	cout << "                   [2] Show Pharmaceutical List.                                                \n";
	cout << "                   [3] Add New Pharmaceutical.                                                  \n";
	cout << "                   [4] Adding An Amount To The Medicine.                                        \n";
	cout << "                   [5] Total Drug Price.                                                        \n";
	cout << "                   [6] Main Menue.                                                              \n";
	cout << "  ============================================================================================  \n";
	PerfromMenueOfPharmaceuticalOption((enMenueOfPharmaceuticalOptions)ReadMenueOfPharmaceuticalOption(1, 6));
}

///*                         Department Complaint                                *///

const string ComplaintFileName = "Data\\Complaint.txt";
const string ComplaintReviewedFileName = "Data\\ComplaintReviewed.txt";

void ShowMenueOfComplain();

enum enMenueOfComplaint
{
	eAddNewComplaint = 1, eListComplaintUnderReview = 2,
	eListComplaintReviewed = 3, eRetuenFromComlaintToMain = 4
};

struct sComplain
{
	string NameOfTheComplainant;
	string TextMessage;
	string ComplaintDate;
	bool MarkForDelete = false;
};

sComplain ReadNewComplain()
{
	sComplain Complain;

	cout << "\tEnter Name Of The Complainant? ";
	getline(cin >> ws, Complain.NameOfTheComplainant);

	cout << "\tEnter Text Message? ";
	getline(cin, Complain.TextMessage);

	cout << "\n\tEnter Complaint Date? \n";
	Complain.ComplaintDate = DateToString(GetValidDate(), "/");


	return Complain;
}

string ConvertRecordToLineComplain(sComplain Complain, string Seperator = "-")
{
	string stComplainRecord = "";
	stComplainRecord += Complain.NameOfTheComplainant + Seperator;
	stComplainRecord += Complain.TextMessage + Seperator;
	stComplainRecord += Complain.ComplaintDate;

	return stComplainRecord;
}

void AddNewComplain()
{
	sComplain Complain;
	Complain = ReadNewComplain();
	AddDataLineToFileTheSrting(ComplaintFileName, ConvertRecordToLineComplain(Complain));
}

void AddNewComplaint()
{

	cout << "\tAdding New Complain:\n\n";

	AddNewComplain();
	cout << "\n\tThis Message Has Been Sent To The Administrator..! \n"
		<< "\tA Quick Solution Will Be Found... \n"
		<< "\tThanks For Noticing.\n";

}

void ShowAddNewComplaintScreen()
{
	cout << "  ============================================================================================  \n";
	cout << "                               Add New Complaint                                                \n";
	cout << "  ============================================================================================  \n";

	AddNewComplaint();
}

void GoBackToMenueOfComplaint()
{
	cout << "\n\n\tPress any key to go back to Menue Of Complaint...";
	system("pause>0");
	ShowMenueOfComplain();

}

sComplain ConvertLinetoRecordComplaint(string Line, string Seperator = "-")
{
	sComplain Complain;
	vector<string> vComplainData;
	vComplainData = SplitString(Line, Seperator);

	Complain.NameOfTheComplainant = vComplainData[0];
	Complain.TextMessage = vComplainData[1];
	Complain.ComplaintDate = vComplainData[2];

	return Complain;

}

vector <sComplain> LoadComplaintDataFromFile(string FileName)
{
	vector <sComplain> vComplain;
	fstream MyFile;

	MyFile.open(FileName, ios::in);//read Mode

	if (MyFile.is_open())
	{
		string Line;
		sComplain Complain;

		while (getline(MyFile, Line))
		{
			Complain = ConvertLinetoRecordComplaint(Line);
			vComplain.push_back(Complain);
		}
		MyFile.close();
	}
	return vComplain;
}

void PrintComplaintRecordLine(sComplain Complain)
{
	cout << "| " << setw(25) << left << Complain.NameOfTheComplainant;
	cout << "| " << setw(50) << left << Complain.TextMessage;
	cout << "| " << setw(10) << left << Complain.ComplaintDate;

}

bool FindComplainByNameOfTheComplainant(string Name, sComplain& Complain)
{

	vector <sComplain> vComplain = LoadComplaintDataFromFile(ComplaintFileName);

	for (sComplain C : vComplain)
	{

		if (C.NameOfTheComplainant == Name)
		{
			Complain = C;
			return true;
		}

	}
	return false;

}

bool FindComplainByNameOfTheComplainant(string Name, vector <sComplain> vComplain, sComplain& Complain)
{
	for (sComplain C : vComplain)
	{

		if (C.NameOfTheComplainant == Name)
		{
			Complain = C;
			return true;
		}

	}

	return false;
}

bool MarkComplaintForDeleteByNameOfTheComplainant(string Name, vector <sComplain>& vComplain)
{

	for (sComplain& C : vComplain)
	{

		if (C.NameOfTheComplainant == Name)
		{
			C.MarkForDelete = true;
			return true;
		}

	}

	return false;
}

vector <sComplain> SaveComplaintDataToFile(string FileName, vector <sComplain> vComplain)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);//overwrite

	string DataLine;

	if (MyFile.is_open())
	{
		for (sComplain C : vComplain)
		{

			if (C.MarkForDelete == false)
			{
				//we only write records that are not marked for delete.  
				DataLine = ConvertRecordToLineComplain(C);
				MyFile << DataLine << endl;
			}

		}

		MyFile.close();
	}

	return vComplain;
}

bool RemoveComplainByNameOfTheComplainant(string Name, vector <sComplain>& vComplain)
{
	sComplain Complain;
	char Answer = 'n';

	if (FindComplainByNameOfTheComplainant(Name, vComplain, Complain))
	{

		MarkComplaintForDeleteByNameOfTheComplainant(Name, vComplain);
		SaveComplaintDataToFile(ComplaintFileName, vComplain);

		//Refresh Employees 
		vComplain = LoadComplaintDataFromFile(ComplaintFileName);

		return true;

	}


}

void IsComplaintReviewed()
{
	sComplain Complain;
	string NameOfTheComplainant;
	char Are = 'n';

	cout << "\tDo You Want To Solution Any Complaint? (y/n) ? ";
	cin >> Are;
	if (toupper(Are) == 'Y')
	{
		cout << "\n\tPlease Enter Name Of The Complainant: ";
		getline(cin >> ws, NameOfTheComplainant);

		if (FindComplainByNameOfTheComplainant(NameOfTheComplainant, Complain))
		{
			AddDataLineToFileTheSrting(ComplaintReviewedFileName, ConvertRecordToLineComplain(Complain));

			vector <sComplain> vComplain = LoadComplaintDataFromFile(ComplaintFileName);

			RemoveComplainByNameOfTheComplainant(NameOfTheComplainant, vComplain);
			cout << "\n\tDone, This Problem Has Been Resolved..";
		}
		else
		{
			cout << "\n\tComplaint (" << NameOfTheComplainant << ") is Not Found!";
		}
	}
}

void ShowAllComplaintScreen()
{
	vector <sComplain> vComplaint = LoadComplaintDataFromFile(ComplaintFileName);

	cout << "\n\t\t\t\Complaint List (" << vComplaint.size() << ") Complaint(s).";
	cout << "\n_____________________________________________________";
	cout << "_______________________________________________\n" << endl;

	cout << "| " << left << setw(25) << "Name Of The Complainant";
	cout << "| " << left << setw(50) << "Text Message";
	cout << "| " << left << setw(10) << "Complaint Date";
	cout << "\n_____________________________________________________";
	cout << "_______________________________________________\n" << endl;

	if (vComplaint.size() == 0)
		cout << "\t\t\t\tNo Complaint Available In the Hospital..!";
	else

		for (sComplain Complain : vComplaint)
		{

			PrintComplaintRecordLine(Complain);
			cout << endl;
		}

	cout << "\n_____________________________________________________";
	cout << "_______________________________________________\n" << endl;

	if (GeneralUser.Username == "Admin" && vComplaint.size() != 0)
		IsComplaintReviewed();

}

void ShowAllComplaintReviewedScreen()
{
	vector <sComplain> vComplaint = LoadComplaintDataFromFile(ComplaintReviewedFileName);

	cout << "\n\t\t\t\Complaint List (" << vComplaint.size() << ") Complaint(s).";
	cout << "\n_____________________________________________________";
	cout << "_______________________________________________\n" << endl;

	cout << "| " << left << setw(25) << "Name Of The Complainant";
	cout << "| " << left << setw(50) << "Text Message";
	cout << "| " << left << setw(10) << "Complaint Date";
	cout << "\n_____________________________________________________";
	cout << "_______________________________________________\n" << endl;

	if (vComplaint.size() == 0)
		cout << "\t\t\t\tNo Complaint Available In the Hospital..!";
	else

		for (sComplain Complain : vComplaint)
		{

			PrintComplaintRecordLine(Complain);
			cout << endl;
		}

	cout << "\n_____________________________________________________";
	cout << "_______________________________________________\n" << endl;

}

void PerfromMenueOfComplaintOption(enMenueOfComplaint MenueOfComplaint)
{
	switch (MenueOfComplaint)
	{
	case eAddNewComplaint:
	{
		system("cls");
		ShowAddNewComplaintScreen();
		GoBackToMenueOfComplaint();
		break;
	}
	case eListComplaintUnderReview:
	{
		system("cls");
		ShowAllComplaintScreen();
		GoBackToMenueOfComplaint();
		break;
	}
	case eListComplaintReviewed:
	{
		system("cls");
		ShowAllComplaintReviewedScreen();
		GoBackToMenueOfComplaint();
		break;
	}
	case eRetuenFromComlaintToMain:
	{
		system("cls");
		ShowMainMenue();
		break;
	}

	}
}

void ShowMenueOfComplain()
{
	system("cls");
	cout << "  ============================================================================================  \n";
	cout << "                                  Complain Department                                           \n";
	cout << "  ============================================================================================  \n";
	cout << "                   [1] Add New Complaint.                                                       \n";
	cout << "                   [2] Show Complaint Under Review.                                             \n";
	cout << "                   [3] Show Complaint Reviewed.                                                 \n";
	cout << "                   [4] Main Menue.                                                              \n";
	cout << "  ============================================================================================  \n";
	PerfromMenueOfComplaintOption((enMenueOfComplaint)ReadMenueOfPharmaceuticalOption(1, 4));

}

///*                      Hospital Bank                       *///

void ShowHospitalBank();

enum enHospitalBank
{
	eShowHospitalBank = 1, AccountingForEmployee = 2,
	eReturnFromBankToMainMenue = 3
};

short ReadMenueOfHospitalBankOption(short From, short End)
{
	cout << "Choose what do you want to do From " << From << " To " << End << " : ";
	short Choice = 0;
	cin >> Choice;

	return Choice;
}

string NumberToText(int Num)
{
	string Arr1To19[] =
	{ "", " One", " Two", " Three", " Four", " Five", " Six",
	 " Seven", " Eight", " Nine", " Ten", " Eleven", " Twelve", " Thirteen",
	 " Fourteen", " Fifteen", " Sixteen", " Seventeen", " Eighteen",
	 " Nineteen"
	};

	string Arr20To99[] =
	{ "", "", " Twenty", " Thirty", " Forty", " Fifty",
	 " Sixty", " Seventy", " Eighty", " Ninety"
	};

	if (Num == 0)
	{
		return "";
	}
	if (Num >= 1 && Num <= 19)
	{
		return Arr1To19[Num];
	}
	if (Num >= 20 && Num <= 99)
	{
		return Arr20To99[Num / 10] + " -" + NumberToText(Num % 10);
	}
	if (Num >= 100 && Num <= 999)
	{
		return NumberToText(Num / 100) + " Hundred" + NumberToText(Num % 100);
	}
	if (Num >= 1000 && Num <= 999999)
	{
		return NumberToText(Num / 1000) + " Thousand" + NumberToText(Num % 1000);
	}
	if (Num >= 1000000 && Num <= 999999999)
	{
		return NumberToText(Num / 1000000) + " Million" + NumberToText(Num % 1000000);
	}
	if (Num >= 1000000000 && Num <= 999999999999)
	{
		return NumberToText(Num / 1000000000) + " Billion" + NumberToText(Num % 1000000000);
	}
	else
	{
		return NumberToText(Num / 1000000000000) + " Trillion" + NumberToText(Num % 1000000000000);
	}
}

void ShowAllTheMoneyInHospital()
{
	system("color 2F");
	cout << "  ============================================================================================  \n";
	cout << "                                      Hospital Bank                                             \n";
	cout << "  ============================================================================================  \n";


	cout << "\n\n\t You Have $" << GeneralUser.AllMoneyInBank << ".\n";
	cout << "\t" << NumberToText(GeneralUser.AllMoneyInBank) << " Dollar(s)." << endl;

}

void GoBackToMenueOfHospitalBank()
{
	cout << "\n\n\tPress any key to go back to Menue Of Hospital Bank...";
	system("pause>0");
	system("color 0F");
	ShowHospitalBank();

}

bool DrugWithdrawal(string Medicine, int Quantity, vector <sPharmaceutical>& vPharmaceutical)
{

	for (sPharmaceutical& P : vPharmaceutical)
	{
		if (P.MedicamentName == Medicine)
		{
			P.TheQuantity -= Quantity;
			SavePharmaceuticalDataToFile(PharmaceuticalFileName, vPharmaceutical);
			return true;
		}

	}

}

float DidTakeAnyMedicine()
{
	char Are = 'y';

	cout << "\n\tDid the patient take any medicine? (Y/N)? ";
	cin >> Are;
	if (toupper(Are) == 'Y')
	{
		string Medicine;
		sPharmaceutical Pharmaceutical;
		vector<sPharmaceutical> vPharmaceutical = LoadPharmaceuticalDataFromFile(PharmaceuticalFileName);

		ShowAllPharmaceuticalScreen();
		do
		{
			Medicine = ReadPharmaceutical();

		} while (!FindPharmaceuticalByName(Medicine, vPharmaceutical, Pharmaceutical));

		DrugWithdrawal(Pharmaceutical.MedicamentName, 1, vPharmaceutical);

		return Pharmaceutical.MedicinePrice;
	}
	return 0;

}

bool DeletePatientByRoomNumberWithoutQuestion(string RoomNumber, vector <sPatient>& vPatients)
{
	sPatient Patient;

	MarkPatientForDeleteByRoomNumber(RoomNumber, vPatients);
	SavePatientsDataToFile(PatientsFileName, vPatients);

	//Refresh Patients 
	vPatients = LoadPatientsDataFromFile(PatientsFileName);

	cout << "\n\n\tSuccessfully Done!..";
	return true;
}

void CalculationOfTheBill(string NumberOfRoom)
{
	system("cls");
	cout << "  ============================================================================================  \n";
	cout << "                                          The Bill                                              \n";
	cout << "  ============================================================================================  \n";


	vector <sPatient> vPatients = LoadPatientsDataFromFile(PatientsFileName);
	sPatient Patient;
	string RoomNumber = NumberOfRoom;

	if (FindPatientByRoomNumber(RoomNumber, vPatients, Patient))
	{
		PrintPatientCard(Patient);

		stDate ExitDate = GetSystemDate();
		short Days = GetDifferenceInDays(StringToDate(Patient.DateEntertoHospital), ExitDate, true);
		double MedicalPrescription = DidTakeAnyMedicine();

		double TheBill = Days * TheCostOfOneDayInTheHospital + MedicalPrescription + OpetationCostInHospital;

		cout << "\n\tThe Patient stayed in the hospital : " << Days << " day(s).\n"
			<< "\tNote:\n\tCost one day in Hospital = " << TheCostOfOneDayInTheHospital
			<< "\n\tOpetation Cost = " << OpetationCostInHospital << endl;
		cout << "\n\n\tThe Total Bill is : " << TheBill << "$";

		vector<sUser> vUser = LoadUsersDataFromFile(UserNameFile);
		AddMoneyToBankByPassword(GeneralUser.Username, TheBill, vUser);
		GeneralUser.AllMoneyInBank += TheBill;

		DeletePatientByRoomNumberWithoutQuestion(RoomNumber, vPatients);
	}
}

void PrintAccountingDepartment()
{
	system("cls");
	cout << "  ============================================================================================  \n";
	cout << "                                   Accounting Department                                        \n";
	cout << "  ============================================================================================  \n\n";

}

void ShowAccountingForEmployee()
{
	PrintAccountingDepartment();

	vector <sEmployee> vEmployees = LoadEmployeeDataFromFile(EmployeesFileName);
	sEmployee Employee;

	ShowAllEmployeesScreen();

	string AccountNumber = ReadEmployeeAccountNumber();

	if (FindEmployeeByAccountNumber(AccountNumber, vEmployees, Employee))
	{
		PrintAccountingDepartment();

		PrintEmployeeCard(Employee);

		double Amount = Employee.AccountBalance;

		char Are;
		cout << "\n\tAre You Sure? (y/n)? ";
		cin >> Are;
		if (toupper(Are) == 'Y')
		{
			system("color 2F");

			vector<sUser> vUser = LoadUsersDataFromFile(UserNameFile);
			AddMoneyToBankByPassword(GeneralUser.Username, Amount * (-1), vUser);
			GeneralUser.AllMoneyInBank -= Amount;

			cout << "\n\t\aCongratulations..." << char(3);
		}
		else
		{
			GoBackToMenueOfHospitalBank();
		}
	}
	else
		cout << "\n\tEmployee with Account Number[" << AccountNumber << "] is not found!";


}

void PerfromHospitalBank(enHospitalBank HospitalBank)
{
	switch (HospitalBank)
	{
	case eShowHospitalBank:
	{
		if (GeneralUser.Username == "Admin")
		{
			system("cls");
			ShowAllTheMoneyInHospital();
			GoBackToMenueOfHospitalBank();
			break;
		}
		else
		{
			cout << "\n\n\tYou Cannot access this Choice..!";
			GoBackToMenueOfHospitalBank();
		}
	}
	case AccountingForEmployee:
	{
		if (GeneralUser.Username == "Admin")
		{
			system("cls");
			ShowAccountingForEmployee();
			GoBackToMenueOfHospitalBank();
			break;
		}
		else
		{
			cout << "\n\n\tYou Cannot access this Choice..!";
			GoBackToMenueOfHospitalBank();
		}
	}
	case eReturnFromBankToMainMenue:
	{
		system("cls");
		ShowMainMenue();
		break;
	}
	}

}

void ShowHospitalBank()
{
	system("cls");
	cout << "  ============================================================================================  \n";
	cout << "                             Hospital Bank Department                                           \n";
	cout << "  ============================================================================================  \n";
	cout << "                   [1] Show All The Money.                                                      \n";
	cout << "                   [2] Accounting For an Employee.                                              \n";
	cout << "                   [3] Main Menue.                                                              \n";
	cout << "  ============================================================================================  \n";
	PerfromHospitalBank((enHospitalBank)ReadMenueOfHospitalBankOption(1, 3));

}

///*                          Department Colore                                 *///

void SetColor(int textColor, int bgColor)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, (bgColor << 4) | textColor);
}

void SetCursorPosition(int x, int y)
{
	COORD coord = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void TypeEffect(const string& text, int delay = 50)
{
	for (char c : text) {
		cout << c;
		this_thread::sleep_for(chrono::milliseconds(delay));
	}
}

void ShowHeader()
{
	SetColor(14, 0); // ·Ê‰ √’›—
	int centerX = 40; // „Êﬁ⁄ X ›Ì «·„‰ ’›  ﬁ—Ì»«
	SetCursorPosition(centerX, 1);
	TypeEffect("WELCOME TO THE HOSPITAL SYSTEM\n", 50);
	SetCursorPosition(centerX, 2);
	TypeEffect("Committed to Your Health & Care\n", 50);
}

void PrintHeart()
{
	string heart[9] =
	{
		"   ******       ******   ",
		"  *********   *********  ",
		" *********** *********** ",
		"  *********************  ",
		"   *******************   ",
		"     ***************     ",
		"       ***********       ",
		"          *****          ",
		"            *            "
	};

	SetColor(12, 0); // «··Ê‰ «·√Õ„—
	int heartX = 10; // „Êﬁ⁄ «·ﬁ·» ⁄·Ï «·Ì”«—
	int heartY = 5;  // „Êﬁ⁄ Y ·»œ¡ «·ﬁ·»
	for (int i = 0; i < 9; i++) {
		SetCursorPosition(heartX, heartY + i);
		cout << heart[i] << endl;
	}
}

void ShowFinalMessages()
{
	int textX = 50; // „Êﬁ⁄ «·‰’ ⁄·Ï Ì„Ì‰ «·ﬁ·»
	int textY = 8;  // „Êﬁ⁄ »œ«Ì… «·‰’

	SetColor(12, 0);
	SetCursorPosition(textX, textY);
	TypeEffect("Thank you for using our Hospital System!\n", 50);

	SetColor(2, 0);
	SetCursorPosition(textX, textY + 1);
	TypeEffect("We wish you a speedy recovery and good health.\n", 50);

	SetColor(11, 0);
	SetCursorPosition(textX, textY + 2);
	TypeEffect("Take care, and don't hesitate to contact us.\n", 50);
}

void Countdown(int seconds)
{
	int countdownX = 50; // ‰›” „Êﬁ⁄ «·‰’Ê’
	int countdownY = 11; // √”›· «·‰’Ê’ »À·«À… √”ÿ—

	for (int i = seconds; i > 0; i--) {
		int color = 5; // √·Ê«‰ „ €Ì—… ·ﬂ· À«‰Ì…
		SetColor(color, 0);
		SetCursorPosition(countdownX, countdownY);
		cout << "Closing in: " << i << " seconds...   ";
		this_thread::sleep_for(chrono::seconds(1));
	}

	SetColor(10, 0); //  À»Ì  «··Ê‰ «·√Œ÷— ⁄‰œ «·‰Â«Ì…
	SetCursorPosition(countdownX, countdownY);
	cout << "Closing in: 0 seconds...   " << endl;

}

void ShowEndOfProjectScreen()
{
	system("cls"); // „”Õ «·‘«‘…

	// ⁄—÷ «·‘⁄«—
	ShowHeader();

	// ÿ»«⁄… «·ﬁ·» «·√Õ„—
	PrintHeart();

	// ⁄—÷ «·‰’Ê’ «·‰Â«∆Ì…
	ShowFinalMessages();

	// ⁄—÷ «·⁄œ «· ‰«“·Ì  Õ  «·‰’Ê’
	Countdown(5);

	// —”«·… «·Êœ«⁄ «·‰Â«∆Ì…
	SetColor(3, 0); // ·Ê‰ ”„«ÊÌ ··‰’ «·√”«”Ì
	SetCursorPosition(0, 15);
	cout << "\n  Goodbye and take care! We hope to see you again soon." << endl;

	SetColor(12, 0); // ·Ê‰ √Õ„— ··«”„ "Othman Turk."
	SetCursorPosition(40, 16);
	cout << "\t\t\t\t\t\t" << char(174) << "Othman Turk." << char(175) << endl;

	SetColor(7, 0); // «·⁄Êœ… ··Ê‰ «·«› —«÷Ì
}

void ShowEndScreen()
{
	ShowEndOfProjectScreen();

	cout << "\n\n\tPress any key to end the program..." << char(3) << endl << endl;
	system("pause>0");

}

///*                       program Start                               *///

enum enMainMenue
{
	eHospitalBank = 1, eEmployyes = 2, ePatients = 3,
	eOperations = 4, ePharmaceutical = 5, eComplain = 6,
	eExit = 7
};

void PerfromMainMenue(enMainMenue MainMenue)
{
	switch (MainMenue)
	{
	case eHospitalBank:
	{
		ShowHospitalBank();
		break;
	}
	case eEmployyes:
	{
		ShowMenueOfEmployee();
		break;
	}
	case ePatients:
	{
		ShowMenueOfPatient();
		break;
	}
	case eOperations:
	{
		ShowMenueOfOperations();
		break;
	}
	case ePharmaceutical:
	{
		ShowMenueOfPharmaceutical();
		break;
	}
	case eComplain:
	{
		ShowMenueOfComplain();
		break;
	}
	default:
	{
		ShowEndScreen();
		break;
	}
	}

}

void ShowMainMenue()
{
	system("cls");
	cout << "  ============================================================================================  \n";
	cout << "                                          Main Menue                                            \n";
	cout << "  ============================================================================================  \n";
	cout << "                       [1] Hospital Bank.                                                \n";
	cout << "                       [2] Employees Department.                                         \n";
	cout << "                       [3] Patients Department.                                          \n";
	cout << "                       [4] Operations Department.                                        \n";
	cout << "                       [5] Pharmaceutical Department.                                    \n";
	cout << "                       [6] Complain Department.                                          \n";
	cout << "                       [7] Exit.                                                         \n";
	cout << "  ============================================================================================  \n\n";

	PerfromMainMenue((enMainMenue)ReadMenueOfEmployeeOption(1, 7));
}

void Login()
{

	bool LoginFaild = false;
	system("color 0F");

	string Username, Password;
	do
	{
		system("cls");

		cout << "  ============================================================================================  \n";
		cout << "                                         Login Screen                                           \n";
		cout << "  ============================================================================================  \n";

		if (LoginFaild)
		{
			cout << "\tInvlaid Username/Password!\n";
		}

		cout << "\tEnter Username? ";
		cin >> Username;

		cout << "\tEnter Password? ";
		cin >> Password;

		LoginFaild = !LoadUserInfo(Username, Password);

	} while (LoginFaild);

	ShowMainMenue();

}

int main()
{
	Login();

	system("pause>0");
	return 0;
}


/*

  Paracetamol		Disrespect           Meninges
  Metformin         Inexperience		 Broken Foot
  Insulin							     Broken Head
  Augmentin							     Open Heart
  Tramadol							     Asthma
  Midazolam							     Impotence
  Fluorescein							 Amnesia
  Lidocaine

*/