#include <bits/stdc++.h>
#include <cstdlib>
using namespace std;

const string ClientsFileName = "Clients.txt";
const string UsersFileName = "Users.txt";

struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;
};

struct sUsers {
    string UserName;
    string Password;
    int Permissions;
    bool MarkForDelete=false;
};

enum Permission {
    enShowClients  = 1 << 0,  // 00000001
    enAddNewClient = 1 << 1,  // 00000010
    enDeleteClient = 1 << 2,  // 00000100
    enUpdateClient = 1 << 3,  // 00001000
    enFindClient   = 1 << 4,  // 00010000
    enTransactions = 1 << 5,  // 00100000
    enMangeUsers   = 1 << 6   // 01000000
};

enum enMainMenueOptions
{
    eListClients = 1, eAddNewClient = 2,
    eDeleteClient = 3, eUpdateClient = 4,
    eFindClient = 5, eTransiaction = 6,
    eMangeUsers = 7, eExit = 8
};

enum enMangeUsersMenueOptions
{
    eListUsers = 1, eAddNewUser = 2,
    eDeleteUser = 3, eUpdateUser = 4,
    eFindUser = 5, eMainMenue = 6
};

enum enTransactionOptions {
    endeposit = 1, enWithdrawal = 2,
    enTotalBalance = 3, enMainMenue = 4
};

// ─── Forward Declarations ────────────────────────────────────────────────────
void ShowMainMenue(sUsers &user);
void Login();
void TransaiactionScreenOperation(sUsers &user);
void GoBackToMainMenue(sUsers &user);
void GoBackToMainMenueOfTransactions(sUsers &user);
void ManageUsersScreenOperation(sUsers &user);

// ─── String Utilities ────────────────────────────────────────────────────────
vector<string> SplitString(string S1, string Delim)
{
    vector<string> vString;
    int pos = 0;
    string sWord;

    while ((pos = S1.find(Delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos);
        if (sWord != "")
            vString.push_back(sWord);
        S1.erase(0, pos + Delim.length());
    }

    if (S1 != "")
        vString.push_back(S1);

    return vString;
}

// ─── Client Record Conversion ─────────────────────────────────────────────────
sClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{
    sClient Client;
    vector<string> vClientData = SplitString(Line, Seperator);

    Client.AccountNumber  = vClientData[0];
    Client.PinCode        = vClientData[1];
    Client.Name           = vClientData[2];
    Client.Phone          = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);
    return Client;
}

string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{
    string stClientRecord = "";
    stClientRecord += Client.AccountNumber  + Seperator;
    stClientRecord += Client.PinCode        + Seperator;
    stClientRecord += Client.Name           + Seperator;
    stClientRecord += Client.Phone          + Seperator;
    stClientRecord += to_string(Client.AccountBalance);
    return stClientRecord;
}

string ConvertRecordToLineUserw(sUsers User, string Seperator = "#//#")
{
    string stUserRecord = "";
    stUserRecord += User.UserName  + Seperator;
    stUserRecord += User.Password        + Seperator;
    stUserRecord += to_string(User.Permissions);
    return stUserRecord;
}

sUsers ConvertLinetoRecordUser(string Line, string Seperator = "#//#")
{
    sUsers user;
    vector<string> vUser = SplitString(Line, Seperator);
    user.UserName    = vUser[0];
    user.Password    = vUser[1];
    user.Permissions = stoi(vUser[2]);

    return user;
}






// ─── User Record Conversion ───────────────────────────────────────────────────
sUsers ConvertLinetoRecordUsers(string Line, string Seperator = "#//#")
{
    sUsers User;
    vector<string> vUserdata = SplitString(Line, Seperator);

    User.UserName    = vUserdata[0];
    User.Password    = vUserdata[1];
    User.Permissions = stoi(vUserdata[2]);
    return User;
}

// ─── File Operations ──────────────────────────────────────────────────────────
vector<sClient> LoadCleintsDataFromFile(string FileName)
{
    vector<sClient> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        sClient Client;
        while (getline(MyFile, Line))
        {
            Client = ConvertLinetoRecord(Line);
            vClients.push_back(Client);
        }
        MyFile.close();
    }
    return vClients;
}

vector<sUsers> LoadUsersDataFromFileUsers(string FileName)
{
    vector<sUsers> vUsers;
    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        sUsers User;
        while (getline(MyFile, Line))
        {
            User = ConvertLinetoRecordUsers(Line);
            vUsers.push_back(User);
        }
        MyFile.close();
    }
    return vUsers;
}

vector<sClient> SaveCleintsDataToFile(string FileName, vector<sClient> vClients)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);
    string DataLine;

    if (MyFile.is_open())
    {
        for (sClient C : vClients)
        {
            if (C.MarkForDelete == false)
            {
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;
            }
        }
        MyFile.close();
    }
    return vClients;
}


vector<sUsers> SaveUsersDataToFile(string FileName, vector<sUsers> vUsers)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);
    string DataLine;

    if (MyFile.is_open())
    {
        for (sUsers U : vUsers)
        {
            if (U.MarkForDelete == false)
            {
                DataLine = ConvertRecordToLineUserw(U);
                MyFile << DataLine << endl;
            }
        }
        MyFile.close();
    }
    return vUsers;
}





void AddDataLineToFile(string FileName, string stDataLine)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);
    if (MyFile.is_open())
    {
        MyFile << stDataLine << endl;
        MyFile.close();
    }
}

// ─── Client Lookup ────────────────────────────────────────────────────────────
bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{
    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        sClient Client;
        while (getline(MyFile, Line))
        {
            Client = ConvertLinetoRecord(Line);
            if (Client.AccountNumber == AccountNumber)
            {
                MyFile.close();
                return true;
            }
        }
        MyFile.close();
    }
    return false;
}

bool FindClientByAccountNumber(string AccountNumber, vector<sClient> vClients, sClient &Client)
{
    for (sClient C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }
    }
    return false;
}

bool FindUsertByUserName(string UserName, vector<sUsers> vUser, sUsers &user)
{
    for (sUsers U : vUser)
    {
        if (U.UserName == UserName)
        {
            user = U;
            return true;
        }
    }
    return false;
}





// ─── User Lookup ──────────────────────────────────────────────────────────────
bool FindUserByUsernameandpass(string UserName, string Password, vector<sUsers> vUsers, sUsers &user)
{
    for (sUsers U : vUsers)
    {
        if (U.UserName == UserName && U.Password == Password)
        {
            user = U;
            return true;
        }
    }
    return false;
}

bool Isvaliduser(string username, string password, sUsers &user)
{
    vector<sUsers> vUsers = LoadUsersDataFromFileUsers(UsersFileName);
    return FindUserByUsernameandpass(username, password, vUsers, user);
}

// ─── Client Display ───────────────────────────────────────────────────────────
void PrintClientCard(sClient Client)
{
    cout << "\nThe following are the client details:\n";
    cout << "-----------------------------------";
    cout << "\nAccount Number : " << Client.AccountNumber;
    cout << "\nPin Code       : " << Client.PinCode;
    cout << "\nName           : " << Client.Name;
    cout << "\nPhone          : " << Client.Phone;
    cout << "\nAccount Balance: " << Client.AccountBalance;
    cout << "\n-----------------------------------\n";
}
void PrintClientRecordLine(sClient Client)
{
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}
void PrintUserRecordLine(sUsers user)
{
    cout << "| " << setw(15) << left << user.UserName;
    cout << "| " << setw(15) << left << user.Password;
    cout << "| " << setw(10) << left << user.Permissions;

}
double ShowAllClientsScreen()
{
    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    cout << "\n___________________________________________________________________________\n\n";

    cout << "| " << left << setw(15) << "Account Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n___________________________________________________________________________\n\n";

    double TotalAmount = 0;

    if (vClients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else
    {
        for (sClient Client : vClients)
        {
            PrintClientRecordLine(Client);
            TotalAmount += Client.AccountBalance;
            cout << endl;
        }
    }

    cout << "\n___________________________________________________________________________\n";
    cout << "\t\t\t\tTotal Balance = " << TotalAmount << endl;
    return TotalAmount;
}
// ─── Client CRUD ──────────────────────────────────────────────────────────────
sClient ReadNewClient()
{
    sClient Client;
    cout << "Enter Account Number? ";
    getline(cin >> ws, Client.AccountNumber);

    while (ClientExistsByAccountNumber(Client.AccountNumber, ClientsFileName))
    {
        cout << "\nClient with [" << Client.AccountNumber << "] already exists, Enter another Account Number? ";
        getline(cin >> ws, Client.AccountNumber);
    }

    cout << "Enter PinCode? ";
    getline(cin, Client.PinCode);
    cout << "Enter Name? ";
    getline(cin, Client.Name);
    cout << "Enter Phone? ";
    getline(cin, Client.Phone);
    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;

    return Client;
}

sClient ChangeClientRecord(string AccountNumber)
{
    sClient Client;
    Client.AccountNumber = AccountNumber;

    cout << "\nEnter PinCode? ";
    getline(cin >> ws, Client.PinCode);
    cout << "Enter Name? ";
    getline(cin, Client.Name);
    cout << "Enter Phone? ";
    getline(cin, Client.Phone);
    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;

    return Client;
}

sUsers ChangeUserRecord(string username) {
    sUsers User;
    User.UserName = username;
    string answer;
    cout<<"Enter New Password: ";
    cin>>User.Password;
    cout<<"do you want to give him full acess [y/n]? ";
    getline(cin >> ws, answer);
    if (answer == "y" || answer == "Y") {
        User.Permissions=-1;
    }
    else {
        int x=0;
        cout<<"Do you want to give acess for \n";
        cout<<"Show client list [y/n]? ";
        getline(cin >> ws, answer);
        if (answer == "y" || answer == "Y") {
            x= x|enShowClients;
        }
        cout<<"Show delete client  [y/n]? ";
        getline(cin >> ws, answer);
        if (answer == "y" || answer == "Y") {
            x=x|enDeleteClient;
        }
        cout<<"Show Add Client [y/n]? ";
        getline(cin >> ws, answer);
        if (answer == "y" || answer == "Y") {
            x=x|enAddNewClient;
        }
        cout<<"Show Update client [y/n]? ";
        getline(cin >> ws, answer);
        if (answer == "y" || answer == "Y") {
            x=x|enUpdateClient;
        }
        cout<<"Show Find client [y/n]? ";
        getline(cin >> ws, answer);
        if (answer == "y" || answer == "Y") {
            x=x|enFindClient;
        }
        cout<<"Show Transactions clients [y/n]? ";
        getline(cin >> ws, answer);
        if (answer == "y" || answer == "Y") {
            x=x|enTransactions;
        }
        cout<<"Show Mange users [y/n]? ";
        getline(cin >> ws, answer);
        if (answer == "y" || answer == "Y") {
            x=x|enMangeUsers;
        }

        User.Permissions = x;
    }

    return User;
}



bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector<sClient> &vClients)
{
    for (sClient &C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            C.MarkForDelete = true;
            return true;
        }
    }
    return false;
}

void AddNewClient()
{
    sClient Client = ReadNewClient();
    AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));
}


sUsers ReadNewUser()
{
    sUsers User;
    vector<sUsers> vUsers=LoadUsersDataFromFileUsers(UsersFileName);
    cout << "Enter Username: ";
    getline(cin >> ws, User.UserName);
    while (FindUsertByUserName(User.UserName, vUsers, User))
    {
        cout << "\nUser with [" << User.UserName << "] already exists, Enter another UserName? ";
        getline(cin >> ws, User.UserName);
    }
    User=ChangeUserRecord(User.UserName);
    return User;


}

void AddNewUser() {
    sUsers User=ReadNewUser();
    AddDataLineToFile(UsersFileName, ConvertRecordToLineUserw(User));
}




void AddNewClients()
{
    char AddMore = 'Y';
    do
    {
        cout << "Adding New Client:\n\n";
        AddNewClient();
        cout << "\nClient Added Successfully, do you want to add more clients? Y/N? ";
        cin >> AddMore;
    } while (toupper(AddMore) == 'Y');
}

bool DeleteClientByAccountNumber(string AccountNumber, vector<sClient> &vClients)
{
    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);
        cout << "\n\nAre you sure you want to delete this client? y/n? ";
        cin >> Answer;

        if (toupper(Answer) == 'Y')
        {
            MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
            SaveCleintsDataToFile(ClientsFileName, vClients);
            vClients = LoadCleintsDataFromFile(ClientsFileName);
            cout << "\n\nClient Deleted Successfully.";
            return true;
        }
    }
    else
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";

    return false;
}

bool UpdateClientByAccountNumber(string AccountNumber, vector<sClient> &vClients)
{
    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);
        cout << "\n\nAre you sure you want to update this client? y/n? ";
        cin >> Answer;

        if (toupper(Answer) == 'Y')
        {
            for (sClient &C : vClients)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C = ChangeClientRecord(AccountNumber);
                    break;
                }
            }
            SaveCleintsDataToFile(ClientsFileName, vClients);
            cout << "\n\nClient Updated Successfully.";
            return true;
        }
    }
    else
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";

    return false;
}


bool UpdateUserByUserName(string UserName, vector<sUsers> &vUsers)
{
    sUsers User;
    char Answer = 'n';

    if (FindUsertByUserName(UserName, vUsers, User))
    {
        PrintUserRecordLine(User);
        cout << "\n\nAre you sure you want to update this User? y/n? ";
        cin >> Answer;

        if (toupper(Answer) == 'Y')
        {
            for (sUsers &U: vUsers)
            {
                if ( U.UserName== UserName)
                {
                    U = ChangeUserRecord(UserName);
                    break;
                }
            }
            SaveUsersDataToFile(UsersFileName, vUsers);
            cout << "\n\nClient Updated Successfully.";
            return true;
        }
    }
    else
        cout << "\nUser with UserName (" << UserName << ") is Not Found!";

    return false;
}

bool DeleteUserByUserName(string UserName, vector<sUsers> &vUsers)
{
    sUsers User;
    char Answer = 'n';

    if (FindUsertByUserName(UserName, vUsers, User))
    {
        PrintUserRecordLine(User);
        cout << "\n\nAre you sure you want to delete this User? y/n? ";
        cin >> Answer;

        if (toupper(Answer) == 'Y')
        {
            for (sUsers &U: vUsers)
            {
                if ( U.UserName== UserName)
                {
                    U.MarkForDelete=true;
                    break;
                }
            }
            SaveUsersDataToFile(UsersFileName, vUsers);
            cout << "\n\nClient Deleted Successfully.";
            return true;
        }
    }
    else
        cout << "\nUser with UserName (" << UserName << ") is Not Found!";

    return false;
}

bool FindUsertByUserNameScreen(string UserName, vector<sUsers> vUser)
{
    for (sUsers U : vUser)
    {
        if (U.UserName == UserName)
        {
            PrintUserRecordLine(U);
            return true;
        }
    }
    return false;
}



// ─── Screen Helpers ───────────────────────────────────────────────────────────
string ReadClientAccountNumber()
{
    string AccountNumber = "";
    cout << "\nPlease enter AccountNumber? ";
    cin >> AccountNumber;
    return AccountNumber;
}
string ReadUserName()
{
    string username = "";
    cout << "\nPlease enter UserName? ";
    cin >> username;
    return username;
}


void NoPermissions()
{
    cout << "\n-----------------------------------------------\n";
    cout << "\t\t No permissions on this account.\n";
    cout << "Please contact your Admin.\n";
    cout << "-----------------------------------------------\n";
}

// ─── Client Screens ───────────────────────────────────────────────────────────
void ShowDeleteClientScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDelete Client Screen";
    cout << "\n-----------------------------------\n";

    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    DeleteClientByAccountNumber(AccountNumber, vClients);
}

void ShowUpdateClientScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tUpdate Client Info Screen";
    cout << "\n-----------------------------------\n";

    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    UpdateClientByAccountNumber(AccountNumber, vClients);
}

void ShowAddNewClientsScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tAdd New Clients Screen";
    cout << "\n-----------------------------------\n";
    AddNewClients();
}

void ShowFindClientScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tFind Client Screen";
    cout << "\n-----------------------------------\n";

    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    sClient Client;
    string AccountNumber = ReadClientAccountNumber();

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
        PrintClientCard(Client);
    else
        cout << "\nClient with Account Number [" << AccountNumber << "] is not found!";
}

// ─── Transactions ─────────────────────────────────────────────────────────────
double ShowClientCard(string ACC)
{
    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    sClient Client;

    if (FindClientByAccountNumber(ACC, vClients, Client))
    {
        PrintClientCard(Client);
        return Client.AccountBalance;
    }
    else
        cout << "\nClient with Account Number [" << ACC << "] is not found!";

    return 0.0;
}

void DepositOperation(double amount, string AccountNumber)
{
    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    fstream file;
    file.open(ClientsFileName, ios::out);

    for (sClient &C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
            C.AccountBalance += amount;

        file << ConvertRecordToLine(C) << endl;
    }
    file.close();
}

void DepositClientsScreen()
{
    cout << "---------------------------------------------\n";
    cout << "\t Deposit Screen\n";
    cout << "---------------------------------------------\n\n";

    cout << "Please enter Account Number? ";
    string AccountNumber;
    cin >> AccountNumber;

    while (!ClientExistsByAccountNumber(AccountNumber, ClientsFileName))
    {
        cout << "There is no such account number! Please Enter a Valid Account Number: ";
        cin >> AccountNumber;
    }
    cin.ignore();

    ShowClientCard(AccountNumber);

    double amount;
    do {
        cout << "Please enter the Amount: ";
        cin >> amount;
    } while (amount < 0);
    cin.ignore();

    cout << "Are you sure you want to perform this transaction? y/n: ";
    string option;
    getline(cin, option);

    if (toupper(option[0]) == 'Y')
        DepositOperation(amount, AccountNumber);
}

void WithdrawClientsScreen()
{
    cout << "---------------------------------------------\n";
    cout << "\t Withdraw Screen\n";
    cout << "---------------------------------------------\n\n";

    cout << "Please enter Account Number? ";
    string AccountNumber;
    cin >> AccountNumber;

    while (!ClientExistsByAccountNumber(AccountNumber, ClientsFileName))
    {
        cout << "There is no such account number! Please Enter a Valid Account Number: ";
        cin >> AccountNumber;
    }
    cin.ignore();

    double balance = ShowClientCard(AccountNumber);

    double amount;
    do {
        cout << "Please enter a valid Amount (max " << balance << "): ";
        cin >> amount;
    } while (amount < 0 || amount > balance);
    cin.ignore();

    cout << "Are you sure you want to perform this transaction? y/n: ";
    string option;
    getline(cin, option);

    if (toupper(option[0]) == 'Y')
        DepositOperation(amount * -1, AccountNumber);
}


void GoBackToMangeMenue(sUsers user)
{
    cout << "\n\nPress any key to go back to Mange Menu...";
    system("pause>0");
    ManageUsersScreenOperation(user);
}


// ─── Navigation ───────────────────────────────────────────────────────────────
void GoBackToMainMenue(sUsers &user)
{
    cout << "\n\nPress any key to go back to Main Menu...";
    system("pause>0");
    ShowMainMenue(user);
}

void GoBackToMainMenueOfTransactions(sUsers &user)
{
    cout << "\n\nPress any key to go back to Transactions Menu...";
    system("pause>0");
    TransaiactionScreenOperation(user);
}

// ─── Transaction Screen ───────────────────────────────────────────────────────
void TransaiactioOperation(enTransactionOptions Option, sUsers &user)
{
    switch (Option)
    {
        case enTransactionOptions::endeposit:
            system("cls");
            DepositClientsScreen();
            GoBackToMainMenueOfTransactions(user);
            break;

        case enTransactionOptions::enWithdrawal:
            system("cls");
            WithdrawClientsScreen();
            GoBackToMainMenueOfTransactions(user);
            break;

        case enTransactionOptions::enTotalBalance:
            system("cls");
            ShowAllClientsScreen();
            GoBackToMainMenueOfTransactions(user);
            break;

        case enTransactionOptions::enMainMenue:
            system("cls");
            GoBackToMainMenue(user);
            break;
    }
}

short ReadMainMenueTransactionOption()
{
    cout << "Choose what do you want to do? [1 to 4]: ";
    short Choice = 0;
    cin >> Choice;
    return Choice;
}

void TransaiactionScreenOperation(sUsers &user)
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t[1] Deposit.\n";
    cout << "\t[2] Withdraw.\n";
    cout << "\t[3] Total Balance.\n";
    cout << "\t[4] Main Menu.\n";
    cout << "===========================================\n";
    TransaiactioOperation((enTransactionOptions)ReadMainMenueTransactionOption(), user);
}

// ----------------------User---------------
void ListUsersScreenOperation() {

    vector<sUsers> Users = LoadUsersDataFromFileUsers(UsersFileName);

    cout << "\n\t\t\t\t\tUsers List (" << Users.size() << ") User(s).";
    cout << "\n___________________________________________________________________________\n\n";

    cout << "| " << left << setw(15) << "User Number";
    cout << "| " << left << setw(15) << "Paswword";
    cout << "| " << left << setw(10) << "permissions";

    cout << "\n___________________________________________________________________________\n\n";

    if (Users.size() == 0)
        cout << "\t\t\t\tNo User Available In the System!";
    else
    {
        for (sUsers User : Users)
        {
            PrintUserRecordLine(User);
            cout << endl;
        }
    }

}

void UpdateUsersScreenOperation() {

    cout << "\n-----------------------------------\n";
    cout << "\tUpdate User Info Screen";
    cout << "\n-----------------------------------\n";

    vector<sUsers> Vuser = LoadUsersDataFromFileUsers(UsersFileName);
    string UserName = ReadUserName();
    UpdateUserByUserName(UserName, Vuser);

}

void DeleteUsersScreenOperation() {
    cout << "\n-----------------------------------\n";
    cout << "\tDelete User Info Screen";
    cout << "\n-----------------------------------\n";

    vector<sUsers> Vuser = LoadUsersDataFromFileUsers(UsersFileName);
    string UserName = ReadUserName();
    DeleteUserByUserName(UserName, Vuser);
}


void FindUserByUserName() {
    cout << "\n-----------------------------------\n";
    cout << "\tFind User Info Screen";
    cout << "\n-----------------------------------\n";
    vector<sUsers> Vuser = LoadUsersDataFromFileUsers(UsersFileName);
    string UserName = ReadUserName();
  if (!FindUsertByUserNameScreen(UserName, Vuser)) {
      cout<<"Not found";
  }

}


void AddUsersScreenOperation() {
    cout << "\n-----------------------------------\n";
    cout << "\tAdd User Info Screen";
    cout << "\n-----------------------------------\n";
    string username, password;
    sUsers user;
    bool valid = false;
    vector<sUsers> vUsers = LoadUsersDataFromFileUsers(UsersFileName);


    char AddMore = 'Y';
    do
    {
        cout << "Adding New Client:\n\n";
        AddNewUser();
        cout << "\nClient Added Successfully, do you want to add more clients? Y/N? ";
        cin >> AddMore;
    } while (toupper(AddMore) == 'Y');


}


// ─── Manage Users Screen ──────────────────────────────────────────────────────
void PerfromMangeUserMenueOption(enMangeUsersMenueOptions enUserOption,sUsers &user)
{
    switch (enUserOption)
    {
        case enMangeUsersMenueOptions::eListUsers:
            system("cls");
            ListUsersScreenOperation();
            GoBackToMangeMenue(user);
            break;

        case enMangeUsersMenueOptions::eAddNewUser:
            system("cls");
            AddUsersScreenOperation();
            GoBackToMangeMenue(user);
            break;

        case enMangeUsersMenueOptions::eDeleteUser:
            system("cls");
            DeleteUsersScreenOperation();
            GoBackToMangeMenue(user);
            break;

        case enMangeUsersMenueOptions::eUpdateUser:
            system("cls");
            UpdateUsersScreenOperation();
            GoBackToMangeMenue(user);
            break;

        case enMangeUsersMenueOptions::eFindUser:
            system("cls");
            FindUserByUserName();
            GoBackToMangeMenue(user);
            break;

        case enMangeUsersMenueOptions::eMainMenue:
            system("cls");
            GoBackToMainMenue(user);
            break;
    }
}

short ReadMangeMenueOption()
{
    cout << "Choose what do you want to do? [1 to 6]: ";
    short Choice = 0;
    cin >> Choice;
    return Choice;
}

void ManageUsersScreenOperation(sUsers &user)
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tManage Users Menu\n";
    cout << "===========================================\n";
    cout << "\t[1] Show Users List.\n";
    cout << "\t[2] Add New User.\n";
    cout << "\t[3] Delete User.\n";
    cout << "\t[4] Update User Info.\n";
    cout << "\t[5] Find User.\n";
    cout << "\t[6] Main Menu.\n";
    cout << "===========================================\n";
    PerfromMangeUserMenueOption((enMangeUsersMenueOptions)ReadMangeMenueOption(),user);
}

// ─── Main Menu ────────────────────────────────────────────────────────────────
short ReadMainMenueOption()
{
    cout << "Choose what do you want to do? [1 to 8]: ";
    short Choice = 0;
    cin >> Choice;
    return Choice;
}

void ShowEndScreen()
{
    Login();
}

void PerfromMainMenueOption(enMainMenueOptions MainMenueOption, sUsers &user)
{
    switch (MainMenueOption)
    {
        case enMainMenueOptions::eListClients:
            system("cls");
            if (user.Permissions == -1 || user.Permissions & enShowClients)  // ✅ & not &&
                ShowAllClientsScreen();
            else
                NoPermissions();
            GoBackToMainMenue(user);
            break;

        case enMainMenueOptions::eAddNewClient:
            system("cls");
            if (user.Permissions == -1 || user.Permissions & enAddNewClient)  // ✅
                ShowAddNewClientsScreen();
            else
                NoPermissions();
            GoBackToMainMenue(user);
            break;

        case enMainMenueOptions::eDeleteClient:
            system("cls");
            if (user.Permissions == -1 || user.Permissions & enDeleteClient)  // ✅
                ShowDeleteClientScreen();
            else
                NoPermissions();
            GoBackToMainMenue(user);
            break;

        case enMainMenueOptions::eUpdateClient:
            system("cls");
            if (user.Permissions == -1 || user.Permissions & enUpdateClient)  // ✅
                ShowUpdateClientScreen();
            else
                NoPermissions();
            GoBackToMainMenue(user);
            break;

        case enMainMenueOptions::eFindClient:
            system("cls");
            if (user.Permissions == -1 || user.Permissions & enFindClient)
                ShowFindClientScreen();
            else
                NoPermissions();
            GoBackToMainMenue(user);
            break;

        case enMainMenueOptions::eTransiaction:
            system("cls");
            if (user.Permissions == -1 || user.Permissions & enTransactions)
                TransaiactionScreenOperation(user);
            else
                NoPermissions();
            GoBackToMainMenue(user);
            break;

        case enMainMenueOptions::eMangeUsers:
            system("cls");
            if (user.Permissions == -1 || user.Permissions & enMangeUsers)
                ManageUsersScreenOperation(user);
            else
                NoPermissions();
            GoBackToMainMenue(user);
            break;

        case enMainMenueOptions::eExit:
            system("cls");
            ShowEndScreen();
            break;
    }
}

void ShowMainMenue(sUsers &user)
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tMain Menu Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Transactions.\n";
    cout << "\t[7] Manage Users.\n";
    cout << "\t[8] Logout.\n";
    cout << "===========================================\n";
    PerfromMainMenueOption((enMainMenueOptions)ReadMainMenueOption(), user);
}

// ─── Login ────────────────────────────────────────────────────────────────────
void Login()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tLogin Screen\n";
    cout << "===========================================\n";

    string username, password;
    sUsers user;
    bool valid = false;

    do {
        cout << "Enter UserName? "; cin >> username;
        cout << "Enter Password? "; cin >> password;

        valid = Isvaliduser(username, password, user);

        if (!valid)
        {
            system("cls");
            cout << "===========================================\n";
            cout << "\t\tLogin Screen\n";
            cout << "===========================================\n";
            cout << "\n Wrong username or password! Try again.\n\n";
        }

    } while (!valid);

    ShowMainMenue(user);
}

// ─── Main ─────────────────────────────────────────────────────────────────────
int main()
{
    Login();
    return 0;
}
