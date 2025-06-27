#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <ctime>
#include<windows.h>
using namespace std;
const int purple = 13;
const int white = 7;
const int yellow = 14;

// Constants & Globals for user management:-
#define USERS 1000
#define MaxName 50
#define MaxPass 10
int index_of_user = -1; // Current logged-in user index
//Starts from one as admin is index 0
int idCount = 1;
int usersCount = 1;

// Constants for subscriptions:-
const int general = 0;
const int scholar = 1;
const int smart_wallet = 2;
//for subscription type
const int monthly = 0;
const int yearly = 1;
const int three_months = 2;
//for type plan
const int zone_one = 1;
const int zone_two = 2;
const int zone_three = 3;
const int zone_four = 4;
//for the chosen zone
float prices_monthly[4] = { 310, 365, 425, 600 };
float prices_yearly[4] = { 3500, 4000, 4500, 5000 };
float prices_3months[4] = { 150, 200, 250, 300 }; //arrays for prices of each zone in each plan

// Global & Constants variables for Ride Management:-
const int max_stations = 100; // Max number of stations can be stored.
const int max_rides = 1000; // Max number of rides can be stored.
int stations_num[3] = { 35, 20, 34 }; // Num of every single line stations 'MAY be used for admin functions coding'.
int total_stations = 89; // Num of total stored stations.
int rides_counter = 0; // Number of rides booked so far '0-Based'.
float zones_fare[4] = { 8.00, 10.00, 15.00, 20.00 }; // Every single zone fare (Zone 1 -> Zone 2 -> Zone 3 -> Zone 4).

struct dates {
    int day, month, year;
};// Struct for expiry, activation and birth date 

struct User {
    string user_ID = "U";
    char username[MaxName];
    char password[MaxPass];
    float balance;
    dates user_date;
    string phone_num;
    int age;
    bool LoggedIn;
} users[USERS] = { {"U000", "admin", "12345"} };//array of struct for users & admin is first element in array;

// Structs definitions:-
struct subscriptions {
    int subscription_type = -1;
    int plan_type = -1;
    int remaining_trips = 0;
    int zones = -1;
    int wallet_balance = 0;
    dates activation_day, expiry_day;
    string user_id; //id small in subscription struct but ID capital in user struct
} the_subscription[USERS];

// Structs for Ride Management:-
// Storing every single station details:-
struct Stations {
    string name;
    int line, position;// Station's position in its line.
    bool is_transition;
    int connected_lines[2];
    int branch_ID; // For line 3 (0 -> Main branch, 1 -> Northern Branch, 2 -> Southern Branch).
};

// Storing every single ride details:-
struct Ride {
    string ride_ID = "R";
    string user_ID, entry_station, exit_station;
    int ride_date[3] = { 0, 0, 0 };
    float ride_fare = 0;
} all_rides[max_rides];

// Storing all stations details:-
Stations all_stations[max_stations] = {
    // Line 1 stations
    {"New El Marg", 1, 1, false, {-1, -1}, 0},
    {"El Marg", 1, 2, false, {-1, -1}, 0},
    {"Ezbet El Nakhl", 1, 3, false, {-1, -1}, 0},
    {"Ain Shams", 1, 4, false, {-1, -1}, 0},
    {"El Matariya", 1, 5, false, {-1, -1}, 0},
    {"Helmiet El Zaitoun", 1, 6, false, {-1, -1}, 0},
    {"Hadayek El Zaitoun", 1, 7, false, {-1, -1}, 0},
    {"Saray El Qobba", 1, 8, false, {-1, -1}, 0},
    {"Hammamat El Qobba", 1, 9, false, {-1, -1}, 0},
    {"Kobri El Qobba", 1, 10, false, {-1, -1}, 0},
    {"Manshiet El Sadr", 1, 11, false, {-1, -1}, 0},
    {"El Demerdash", 1, 12, false, {-1, -1}, 0},
    {"Ghamra", 1, 13, false, {-1, -1}, 0},
    {"El Shohadaa", 1, 14, true, {1, 2}, 0},
    {"Ahmed Orabi", 1, 15, false, {-1, -1}, 0},
    {"Nasser", 1, 16, true, {1, 3}, 0},
    {"El Sadat", 1, 17, false, {-1, -1}, 0},
    {"Saad Zaghloul", 1, 18, false, {-1, -1}, 0},
    {"El Sayeda Zeinab", 1, 19, false, {-1, -1}, 0},
    {"El Malek El Saleh", 1, 20, false, {-1, -1}, 0},
    {"Mar Girgis", 1, 21, false, {-1, -1}, 0},
    {"El Zahraa", 1, 22, false, {-1, -1}, 0},
    {"Dar El Salam", 1, 23, false, {-1, -1}, 0},
    {"Hadayek El Maadi", 1, 24, false, {-1, -1}, 0},
    {"El Maadi", 1, 25, false, {-1, -1}, 0},
    {"Thakanat El Maadi", 1, 26, false, {-1, -1}, 0},
    {"Tora El Balad", 1, 27, false, {-1, -1}, 0},
    {"Kotsika", 1, 28, false, {-1, -1}, 0},
    {"Tora El Asmant", 1, 29, false, {-1, -1}, 0},
    {"El Maasara", 1, 30, false, {-1, -1}, 0},
    {"Hadayek Helwan", 1, 31, false, {-1, -1}, 0},
    {"Wadi Hof", 1, 32, false, {-1, -1}, 0},
    {"Helwan University", 1, 33, false, {-1, -1}, 0},
    {"Ain Helwan", 1, 34, false, {-1, -1}, 0},
    {"Helwan", 1, 35, false, {-1, -1}, 0},
    // Line 2 stations
    {"Shubra El Kheima", 2, 1, false, {-1, -1}, 0},
    {"Kolleyet El Zeraa", 2, 2, false, {-1, -1}, 0},
    {"El Mazallat", 2, 3, false, {-1, -1}, 0},
    {"El Khalafawi", 2, 4, false, {-1, -1}, 0},
    {"Saint Teresa", 2, 5, false, {-1, -1}, 0},
    {"Rod El Farag", 2, 6, false, {-1, -1}, 0},
    {"Masarra", 2, 7, false, {-1, -1}, 0},
    {"El Shohadaa", 2, 8, true, {2, 1}, 0},
    {"Attaba", 2, 9, true, {2, 3}, 0},
    {"Mohamed Naguib", 2, 10, false, {-1, -1}, 0},
    {"Jehan Sadat", 2, 11, false, {-1, -1}, 0},
    {"Opera", 2, 12, false, {-1, -1}, 0},
    {"Dokki", 2, 13, false, {-1, -1}, 0},
    {"El Behoos", 2, 14, false, {-1, -1}, 0},
    {"Hosni Mubarak", 2, 15, false, {-1, -1}, 0},
    {"Faisal", 2, 16, false, {-1, -1}, 0},
    {"Giza", 2, 17, false, {-1, -1}, 0},
    {"Om El Masryeen", 2, 18, false, {-1, -1}, 0},
    {"Sakiat Mekki", 2, 19, false, {-1, -1}, 0},
    {"El Moneeb", 2, 20, false, {-1, -1}, 0},
    // Main branch of line 3
    {"Adly Mansour", 3, 1, false, {-1, -1}, 0},
    {"El Haykestep", 3, 2, false, {-1, -1}, 0},
    {"Omar Ibn El Khattab", 3, 3, false, {-1, -1}, 0},
    {"Qebaa", 3, 4, false, {-1, -1}, 0},
    {"Hesham Barakat", 3, 5, false, {-1, -1}, 0},
    {"El Nozha", 3, 6, false, {-1, -1}, 0},
    {"Nadi El Shams", 3, 7, false, {-1, -1}, 0},
    {"Alf Maskan", 3, 8, false, {-1, -1}, 0},
    {"Heliopolis", 3, 9, false, {-1, -1}, 0},
    {"Haroun", 3, 10, false, {-1, -1}, 0},
    {"El Ahram", 3, 11, false, {-1, -1}, 0},
    {"Koleyet El Banat", 3, 12, false, {-1, -1}, 0},
    {"Cairo Stadium", 3, 13, false, {-1, -1}, 0},
    {"Fair Zone", 3, 14, false, {-1, -1}, 0},
    {"El Abbassiya", 3, 15, false, {-1, -1}, 0},
    {"Abdo Pasha", 3, 16, false, {-1, -1}, 0},
    {"El Geish", 3, 17, false, {-1, -1}, 0},
    {"Bab El Shaariya", 3, 18, false, {-1, -1}, 0},
    {"Attaba", 3, 19, true, {3, 2}, 0},
    {"Nasser", 3, 20, true, {3, 1}, 0},
    {"Maspero", 3, 21, false, {-1, -1}, 0},
    {"Zamalek", 3, 22, false, {-1, -1}, 0},
    {"Kit Kat", 3, 23, true, {0, 0}, 0}, // Special (0) in connected lines as it linkes between branches not lines.
    // Northern Branch of line 3
    {"Sudan", 3, 24, false, {-1, -1}, 1},
    {"Imbaba", 3, 25, false, {-1, -1}, 1},
    {"El Bohi", 3, 26, false, {-1, -1}, 1},
    {"El Qawmia El Arabiya", 3, 27, false, {-1, -1}, 1},
    {"Ring Road", 3, 28, false, {-1, -1}, 1},
    {"Rod El Farag Corr", 3, 29, false, {-1, -1}, 1},
    // Southern Branch of line 3
    {"Tawfiqia", 3, 30, false, {-1, -1}, 2},
    {"Wadi El Nile", 3, 31, false, {-1, -1}, 2},
    {"Gamaet El Dowal", 3, 32, false, {-1, -1}, 2},
    {"Boulaq El Dakrour", 3, 33, false, {-1, -1}, 2},
    {"Cairo University", 3, 34, false, {-1, -1}, 2}
};

// User Management functions:-
void createAcc();
void signIn();
void signOut();
void UpdateProfile();
void calculate_birthday(int);

// Subscription Management functions:-
int choose_subscription(int);
void general_plan(int);
void scholar_plan(int);
void choose_zone(int);
void set_remaining_trips(int);
bool payment_subscription(int);
void subscription_date(int);
bool wallet_charging(int);
void view_subscription(int);
void renew_subscription(int);
void upgrade_subscription(int);
bool is_subscription_active(int);
int validation(int, int); // To prevent the invalid input -> talking about int not char.
bool deduct_price(int, int);
void set_expiry_date(int);
bool try_again();

// Ride Management functions declaration:-
void view_stations(int);
int find_station(int, int);
int find_tStation(int, int);
int calculate_stations(int, int, int);
float calculate_fare(int);
void generate_ride_ID(string&);
void save_date();
void book_a_trip(int, int, int, int, string);
void view_ride_history(string);
void view_fare_price();

// File streaming functions:-
void reading_process();
void writing_process();

// Admin Functions:-
void renumering();
void Edit_Commuter();
void Viewuser(int user_Index);
void modifyUser(int user_Index);
void remove_commuter(int user_Index);
void edit_subscription();
void Discontinue_Subscription(int user_Index);
void StationManagement();
void AddStation();
void EditStation();
void RemoveStation();
void EditFare();
void viewRides();
void addSubscription(int user_Index);
void EditSubscriptionPrice();
void statistics();

void Setcolor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// User Management functions definition:-
void createAcc() {
    if (usersCount >= USERS) {
        Setcolor(purple);
        cout << "Maximum users reached!\n";  //technically never reached
        return;
    }
    Setcolor(white);
    cout << "Enter username: ";
    cin.getline(users[usersCount].username, MaxName); //getline reads the whole name even if it has spaces in it 
    for (int i = 0; i < usersCount; i++) {
        if (strcmp(users[i].username, users[usersCount].username) == 0) {   //strcmp compares the string content not the memory address 
            Setcolor(purple);
            cout << "Error: This username has already been taken..\n";
            return;
        }
    }
    Setcolor(white);
    cout << "Enter password: ";
    cin.getline(users[usersCount].password, MaxPass);
    cout << "Enter initial balance: ";
    float balance;
    while (!(cin >> balance) || balance <= 0) {
        Setcolor(purple);
        cout << "Error: Please enter a number that is greater than zero for balance: ";
        Setcolor(white);
        cin.clear();
        cin.ignore(10000, '\n');
    }
    cin.ignore();
    users[usersCount].balance = balance;
    Setcolor(white);
    cout << "Enter your birthdate:\n";
    time_t timestamp = time(NULL);
    struct tm current_time;
    localtime_s(&current_time, &timestamp);
    int current_year = current_time.tm_year + 1900;
    cout << "Year: ";
    users[usersCount].user_date.year = validation(current_year - 100, current_year - 15);
    //check if the user enter year in range (15 --> 100) years old
    cout << "Month: ";
    users[usersCount].user_date.month = validation(1, 12);
    cout << "Day: ";
    switch (users[usersCount].user_date.month) {
    case 1: case 3: case 5: case 7: case 8: case 10: case 12:
        users[usersCount].user_date.day = validation(1, 31);
        break;
    case 2:
        if ((current_year % 4 == 0 && current_year % 100 != 0) ||
            (current_year % 400 == 0)) {
            users[usersCount].user_date.day = validation(1, 29);
        }
        else {
            users[usersCount].user_date.day = validation(1, 28);
        }
        break;
    default:
        users[usersCount].user_date.day = validation(1, 30);
    }
    //taking the birthdate to check if the user can have scholar subs.
    cout << "Enter your phone number:\n";
    cin >> users[usersCount].phone_num;
    while (true) {
        // checking the length
        if (users[usersCount].phone_num.length() != 11) {
            Setcolor(purple);
            cout << "Phone number must be 11 digits!" << endl;
        }
        // checking that the user does not enter letters
        else {
            bool isDigits = true;
            for (char c : users[usersCount].phone_num) {
                if ((c < '0') || (c > '9')) {
                    isDigits = false;
                    break;
                }
            }
            if (!isDigits) {
                Setcolor(purple);
                cout << "Phone number must contain digits only!" << endl;
            }
            // checking the starts
            else {
                string prefix = users[usersCount].phone_num.substr(0, 3);
                if (prefix != "015" && prefix != "012" && prefix != "011" && prefix != "010") {
                    Setcolor(purple);
                    cout << "Phone number must start with 015, 012, 011, or 010!" << endl;
                }
                else {
                    break; //the number is valid break the loop
                }
            }
        }
        // enter again the number
        Setcolor(white);
        cout << "Please enter your phone number again:\n";
        cin >> users[usersCount].phone_num;
    }
    users[usersCount].LoggedIn = false;  //the user created an account but did not sign-in yet 
    string& userID = users[usersCount].user_ID;
    int needed_digits = to_string(USERS - 1).size();
    int counter_digits = to_string(idCount).size();
    if (counter_digits < needed_digits) {
        for (int counter = needed_digits - counter_digits; counter > 0; counter--)
            userID.append("0");
    }
    userID.append(to_string(idCount));  // generates IDs that look like this (U00-num)
    Setcolor(yellow);
    cout << "Account created! Your User ID is: " << userID << "\n\n";
    usersCount++;  //updates the counter
    idCount++;
}

void signIn() {
    char username[MaxName], password[MaxPass];
    Setcolor(white);
    cout << "Enter username: ";
    cin.getline(username, MaxName);
    cout << "Enter password: ";
    cin.getline(password, MaxPass);
    for (int i = 0; i < usersCount; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) { // checks if the user is logged-in already
            if (users[i].LoggedIn) {
                Setcolor(purple);
                cout << "Error: Already logged in.\n";
                return;
            }
            users[i].LoggedIn = true;
            index_of_user = i;
            Setcolor(yellow);
            cout << "Login successful! Welcome, " << username << "\n";
            return;
        }
    }
    Setcolor(purple);
    cout << "Invalid username or password.\n";
}

void signOut() {
    if (index_of_user == -1 || !users[index_of_user].LoggedIn) {
        Setcolor(purple);
        cout << "Error: Not logged in.\n"; // must be signed-in to sign-out
        return;
    }
    users[index_of_user].LoggedIn = false;
    index_of_user = -1;
    Setcolor(yellow);
    cout << "Logout successful.\n\n";
}

void UpdateProfile() {
    int choice;
    Setcolor(white);
    cout << "What would you like to update?\n1. Username\n2. Password\n3. Charge balance\n4. Phone number\n5. View user data\n6. Back\n";
    cout << "Enter choice: ";
    choice = validation(1, 6);
    if (choice == 1) {
        char newUsername[MaxName];
        bool taken = false;
        cout << "Enter new username: ";
        cin.getline(newUsername, MaxName);
        for (int i = 0;i < usersCount;i++) {
            if (i != index_of_user && strcmp(users[i].username, newUsername) == 0) {
                taken = true;
                break;
            }
        }
        if (taken) {
            Setcolor(purple);
            cout << "Username already taken Please choose another.\n";
        }
        else {
            Setcolor(yellow);
            strcpy_s(users[index_of_user].username, newUsername);
            cout << "Username updated successfully.\n";
        }
    }
    else if (choice == 2) {
        char oldPassword[MaxPass];
        char newPassword[MaxPass];
        Setcolor(white);
        cout << "Enter your current password:";
        cin.getline(oldPassword, MaxPass);
        if (strcmp(users[index_of_user].password, oldPassword) == 0) {
            cout << "Enter new password: ";
            cin.getline(newPassword, MaxPass);
            strcpy_s(users[index_of_user].password, newPassword);
            Setcolor(yellow);
            cout << "Password updated successfully.\n";
        }
        else {
            Setcolor(purple);
            cout << "Incorrect current password.\n";
        }
    }
    else if (choice == 3) {
        float chargeBalance = 0;
        Setcolor(white);
        cout << "Enter the amount you want to add to your account: ";
        cin >> chargeBalance;
        if (chargeBalance <= 0) {
            Setcolor(purple);
            cout << "Amount must be greater than zero." << endl;
            return;
        }
        users[index_of_user].balance += chargeBalance;
        Setcolor(yellow);
        cout << "Charged successfully! Current balance = " << users[index_of_user].balance << "\n";
    }
    else if (choice == 4) {
        string new_phone;
        Setcolor(white);
        cout << "Enter the new phone number:\n";
        cin >> new_phone;
        while (true) {
            // checking the length
            if (new_phone.length() != 11) {
                Setcolor(purple);
                cout << "Phone number must be 11 digits!" << endl;
            }
            // checking that the user does not enter letters
            else {
                bool isDigits = true;
                for (char c : new_phone) {
                    if ((c < '0') || (c > '9')) {
                        isDigits = false;
                        break;
                    }
                }
                if (!isDigits) {
                    Setcolor(purple);
                    cout << "Phone number must contain digits only!" << endl;
                }
                // checking the starts
                else {
                    string prefix = new_phone.substr(0, 3);
                    if (prefix != "015" && prefix != "012" && prefix != "011" && prefix != "010") {
                        Setcolor(purple);
                        cout << "Phone number must start with 015, 012, 011, or 010!" << endl;
                    }
                    else {
                        break; //the number is valid break the loop
                    }
                }
            }
            // enter again the number
            Setcolor(white);
            cout << "Please enter your phone number again:\n";
            cin >> new_phone;
        }
        users[index_of_user].phone_num = new_phone;
        Setcolor(yellow);
        cout << "Phone number changed successfully!\n";
    }
    else if (choice == 5) {
        Setcolor(white);
        cout << "__\n\n";
        Setcolor(yellow);
        cout << "User ID: ";
        Setcolor(white);
        cout << users[index_of_user].user_ID << endl;
        Setcolor(yellow);
        cout << "Username: ";
        Setcolor(white);
        cout << users[index_of_user].username << endl;
        Setcolor(yellow);
        cout << "Balance: ";
        Setcolor(white);
        cout << users[index_of_user].balance;
        Setcolor(yellow);
        cout << " LE" << endl;
        Setcolor(yellow);
        cout << "Phone number: ";
        Setcolor(white);
        cout << users[index_of_user].phone_num << endl;
        Setcolor(yellow);
        cout << "Birthdate: ";
        Setcolor(white);
        cout << users[index_of_user].user_date.day << "/" << users[index_of_user].user_date.month << "/"
            << users[index_of_user].user_date.year << endl;
        cout << "__\n\n";
    }

}

void calculate_birthday(int index_of_user) {
    time_t timestamp = time(NULL);
    struct tm current_time;
    localtime_s(&current_time, &timestamp);
    int current_year = current_time.tm_year + 1900;
    int current_month = current_time.tm_mon + 1;
    int current_day = current_time.tm_mday;
    users[index_of_user].age = current_year - users[index_of_user].user_date.year;
    if ((users[index_of_user].user_date.month > current_month) && (users[index_of_user].user_date.day >= current_day)) {
        users[index_of_user].age--;
    }
}

// Subscription Management functions definition:-
int choose_subscription(int index_of_user) { // To choose the subscription type.
    if (index_of_user == -1) {
        Setcolor(purple);
        cout << "Please sign in first!\n";
        return -1;
    }
    Setcolor(white);
    cout << "\n0. General\n1. Scholar\n2. Smart Wallet\n3. Back\n\n";
    cout << "Please enter your choice: ";
    int type = validation(0, 3);
    the_subscription[index_of_user].subscription_type = type;
    return type;
}

bool wallet_charging(int index_of_user) { //to charge the wallet 
    if (index_of_user == -1) {
        Setcolor(purple);
        cout << "Please sign in first!\n";
        return false;
    }
    const int max_balance_in_wallet = 400;
    int amount;
    Setcolor(white);
    cout << "Note:\n- Maximum amount allowed is " << max_balance_in_wallet << " LE.\n- Charge in multiples of 10 LE.\n";
    cout << "Enter amount to charge: ";
    cin >> amount;
    if (amount % 10 != 0) {
        Setcolor(purple);
        cout << "Amount must be a multiple of 10.\n";
        return false;
    }
    if (amount > users[index_of_user].balance) {
        Setcolor(purple);
        cout << "Not enough balance in your bank account.\n";
        return false;
    }
    if (amount + the_subscription[index_of_user].wallet_balance > max_balance_in_wallet) {
        Setcolor(purple);
        cout << "Charging this amount will exceed wallet limit.\n";
        return false;
    }
    if (amount <= 0) {
        Setcolor(purple);
        cout << "You can not enter a negative number or zero!\n";
        return false;
    }
    users[index_of_user].balance -= amount;
    the_subscription[index_of_user].wallet_balance += amount;
    the_subscription[index_of_user].subscription_type = smart_wallet;
    return true;
}

void general_plan(int index_of_user) { //to choose yearly or monthly plan and display the right prices
    //calling the other functions related to choosing general plan
    Setcolor(white);
    cout << "\n0 for Monthly\n1 for Yearly\n\n";
    cout << "Please enter the plan type: ";
    the_subscription[index_of_user].plan_type = validation(0, 1);
    cout << "\nYou have 4 zones:\n\n";
    if (the_subscription[index_of_user].plan_type == monthly) {
        cout << "Zone 1 (1 - 9 stations): " << prices_monthly[0] << " LE\n";
        cout << "Zone 2 (10 - 16 stations): " << prices_monthly[1] << " LE\n";
        cout << "Zone 3 (17 - 23 stations): " << prices_monthly[2] << " LE\n";
        cout << "Zone 4 (More than 23 stations): " << prices_monthly[3] << " LE\n";
    }
    else {
        cout << "Zone 1 (1 - 9 stations): " << prices_yearly[0] << " LE\n";
        cout << "Zone 2 (10 - 16 stations): " << prices_yearly[1] << " LE\n";
        cout << "Zone 3 (17 - 23 stations): " << prices_yearly[2] << " LE\n";
        cout << "Zone 4 (More than 23 stations): " << prices_yearly[3] << " LE\n";
    }
    choose_zone(index_of_user);
    bool payment_success = false;
    while (!payment_success) {
        payment_success = payment_subscription(index_of_user);
        if (!payment_success) {
            Setcolor(purple);
            cout << "Payment failed: Insufficient balance! Current balance: " << users[index_of_user].balance << " LE\n";
            if (!try_again()) {
                the_subscription[index_of_user].subscription_type = -1;
                the_subscription[index_of_user].plan_type = -1;
                the_subscription[index_of_user].zones = -1;
                break;
            }
            choose_zone(index_of_user);
        }
    }
    if (payment_success) {
        the_subscription[index_of_user].user_id = users[index_of_user].user_ID;
        Setcolor(yellow);
        cout << "Subscription purchased successfully!\n";
        cout << "For the user id: ";
        Setcolor(white);
        cout << the_subscription[index_of_user].user_id << "\n";
        Setcolor(yellow);
        cout << "Activation date: ";
        Setcolor(white);
        cout << the_subscription[index_of_user].activation_day.day << "/"
            << the_subscription[index_of_user].activation_day.month << "/" << the_subscription[index_of_user].activation_day.year << "\n";
        Setcolor(yellow);
        cout << "Expiry date: ";
        Setcolor(white);
        cout << the_subscription[index_of_user].expiry_day.day << "/"
            << the_subscription[index_of_user].expiry_day.month << "/" << the_subscription[index_of_user].expiry_day.year << "\n";
    }
}

void scholar_plan(int index_of_user) { //to put the right plan in the suitable variable and display the right prices
    //calling the other functions related to choosing scholar plan
    the_subscription[index_of_user].plan_type = three_months;
    Setcolor(white);
    cout << "\nYou have 4 zones:\n";
    cout << "Zone 1 (1 - 9 stations): " << prices_3months[0] << " LE\n";
    cout << "Zone 2 (10 - 16 stations): " << prices_3months[1] << " LE\n";
    cout << "Zone 3 (17 - 23 stations): " << prices_3months[2] << " LE\n";
    cout << "Zone 4 (More than 23 stations): " << prices_3months[3] << " LE\n";
    choose_zone(index_of_user);
    bool payment_success = false;
    while (!payment_success) {
        payment_success = payment_subscription(index_of_user);
        if (!payment_success) {
            Setcolor(purple);
            cout << "Payment failed: Insufficient balance! Current balance: " << users[index_of_user].balance << " LE\n";
            if (!try_again()) {
                the_subscription[index_of_user].subscription_type = -1;
                the_subscription[index_of_user].plan_type = -1;
                the_subscription[index_of_user].zones = -1;
                break;
            }
            choose_zone(index_of_user);
        }
    }
    if (payment_success) {
        the_subscription[index_of_user].user_id = users[index_of_user].user_ID;
        Setcolor(yellow);
        cout << "Subscription purchased successfully!\n";
        cout << "For the user id: ";
        Setcolor(white);
        cout << the_subscription[index_of_user].user_id << "\n";
        Setcolor(yellow);
        cout << "Activation date: ";
        Setcolor(white);
        cout << the_subscription[index_of_user].activation_day.day << "/"
            << the_subscription[index_of_user].activation_day.month << "/" << the_subscription[index_of_user].activation_day.year << "\n";
        Setcolor(yellow);
        cout << "Expiry date: ";
        Setcolor(white);
        cout << the_subscription[index_of_user].expiry_day.day << "/"
            << the_subscription[index_of_user].expiry_day.month << "/" << the_subscription[index_of_user].expiry_day.year << "\n";
    }
}

void choose_zone(int index_of_user) { //to choose the zone
    Setcolor(white);
    cout << "\nEnter zone you want: ";
    the_subscription[index_of_user].zones = validation(1, 4);
}

bool payment_subscription(int index_of_user) {
    if (index_of_user == -1) {
        Setcolor(purple);
        cout << "Please sign in first!\n";
        return false;
    } // we wont reach here but to protect the code from any possible errors
    int price = 0;
    if (the_subscription[index_of_user].subscription_type == general) {
        price = (the_subscription[index_of_user].plan_type == monthly) ?
            prices_monthly[the_subscription[index_of_user].zones - 1] :
            prices_yearly[the_subscription[index_of_user].zones - 1];
    }
    else if (the_subscription[index_of_user].subscription_type == scholar) {
        price = prices_3months[the_subscription[index_of_user].zones - 1];
    }
    if (deduct_price(price, index_of_user)) {
        set_remaining_trips(index_of_user);
        subscription_date(index_of_user);
        return true;
    }
    return false;
}

bool deduct_price(int price, int index_of_user) { //to deduct the balance of the user 
    if (index_of_user == -1) {
        Setcolor(purple);
        cout << "Error: Please sign in first.\n";
        return false;
    }
    if (users[index_of_user].balance >= price) {
        users[index_of_user].balance -= price;
        Setcolor(yellow);
        cout << "\nYour Bank Balance: ";
        Setcolor(white);
        cout << users[index_of_user].balance;
        Setcolor(yellow);
        cout << " LE" << endl;
        return true;
    }
    return false;
}

void set_remaining_trips(int index_of_user) { //to put the intial number of trips according to the type plan
    if (the_subscription[index_of_user].plan_type == monthly) {
        the_subscription[index_of_user].remaining_trips = 60;
    }
    else if (the_subscription[index_of_user].plan_type == yearly) {
        the_subscription[index_of_user].remaining_trips = 730;
    }
    else if (the_subscription[index_of_user].plan_type == three_months) {
        the_subscription[index_of_user].remaining_trips = 180;
    }
}

void subscription_date(int index_of_user) {
    time_t timestamp = time(NULL);
    struct tm current_time;
    localtime_s(&current_time, &timestamp);
    the_subscription[index_of_user].activation_day.day = current_time.tm_mday;
    the_subscription[index_of_user].activation_day.month = current_time.tm_mon + 1;
    the_subscription[index_of_user].activation_day.year = current_time.tm_year + 1900;
    the_subscription[index_of_user].expiry_day.day = the_subscription[index_of_user].activation_day.day;
    //taking the date from the device
    set_expiry_date(index_of_user);
}

void set_expiry_date(int index_of_user) { //to set the expiry date after taking the activation date from the device
    int adding_months = 0;
    if (the_subscription[index_of_user].subscription_type == scholar) {
        adding_months = 3;
    }
    else if (the_subscription[index_of_user].plan_type == monthly) {
        adding_months = 1;
    }
    else if (the_subscription[index_of_user].plan_type == yearly) {
        adding_months = 12;
    }
    the_subscription[index_of_user].expiry_day.month = the_subscription[index_of_user].activation_day.month + adding_months;
    the_subscription[index_of_user].expiry_day.year = the_subscription[index_of_user].activation_day.year;
    while (the_subscription[index_of_user].expiry_day.month > 12) { //if the number of months exceed 12 month
        the_subscription[index_of_user].expiry_day.month -= 12;
        the_subscription[index_of_user].expiry_day.year++;
    }
    int max_day;
    switch (the_subscription[index_of_user].expiry_day.month) { //handle the 30 , 31 days in months
    case 1: case 3: case 5: case 7: case 8: case 10: case 12: max_day = 31; break;
    case 2: //for leap years
        max_day = ((the_subscription[index_of_user].expiry_day.year % 4 == 0 && the_subscription[index_of_user].expiry_day.year % 100 != 0) ||
            (the_subscription[index_of_user].expiry_day.year % 400 == 0)) ? 29 : 28; break;
    default: max_day = 30;
    }
    if (the_subscription[index_of_user].expiry_day.day > max_day) {
        the_subscription[index_of_user].expiry_day.day = max_day;
    }
}

void view_subscription(int index_of_user) { //to view the activated subscription
    if (index_of_user == -1) {
        Setcolor(purple);
        cout << "Please sign in first!\n";
        return;
    }
    if (the_subscription[index_of_user].subscription_type == -1) {
        Setcolor(purple);
        cout << "No active subscription.\n";
        return;
    }
    Setcolor(yellow);
    cout << "Subscription Type: ";
    Setcolor(white);
    if (the_subscription[index_of_user].subscription_type == general) cout << "General\n";
    else if (the_subscription[index_of_user].subscription_type == scholar) cout << "Scholar\n";
    else cout << "Smart Wallet\n";
    if (the_subscription[index_of_user].subscription_type == smart_wallet) {
        Setcolor(yellow);
        cout << "Wallet Balance: ";
        Setcolor(white);
        cout << the_subscription[index_of_user].wallet_balance;
        Setcolor(yellow);
        cout << " LE\n";
    }
    else {
        Setcolor(yellow);
        cout << "Plan Type: ";
        Setcolor(white);
        if (the_subscription[index_of_user].plan_type == monthly) cout << "Monthly\n";
        else if (the_subscription[index_of_user].plan_type == yearly) cout << "Yearly\n";
        else cout << "3 Months\n";
        Setcolor(yellow);
        cout << "Zone: ";
        Setcolor(white);
        cout << the_subscription[index_of_user].zones << "\n";
        Setcolor(yellow);
        cout << "Remaining Trips: ";
        Setcolor(white);
        cout << the_subscription[index_of_user].remaining_trips << "\n";
        Setcolor(yellow);
        cout << "Activation Date: ";
        Setcolor(white);
        cout << the_subscription[index_of_user].activation_day.day << "/"
            << the_subscription[index_of_user].activation_day.month << "/" << the_subscription[index_of_user].activation_day.year << "\n";
        Setcolor(yellow);
        cout << "Expiry Date: ";
        Setcolor(white);
        cout << the_subscription[index_of_user].expiry_day.day << "/"
            << the_subscription[index_of_user].expiry_day.month << "/" << the_subscription[index_of_user].expiry_day.year << "\n";
    }
}

void renew_subscription(int index_of_user) {
    if (index_of_user == -1) {
        Setcolor(purple);
        cout << "Please sign in first!\n";
        return;
    }
    if (the_subscription[index_of_user].subscription_type == -1) {
        Setcolor(purple);
        cout << "No active subscription to renew.\n";
        return;
    }
    if (the_subscription[index_of_user].subscription_type == smart_wallet) {
        bool charging_success = false;
        while (!charging_success) {
            charging_success = wallet_charging(index_of_user);
            if (!charging_success) {
                Setcolor(purple);
                cout << "Charging failed. Please try again or cancel.\n";
                if (!try_again()) {
                    Setcolor(purple);
                    cout << "Charging cancelled.\n";
                    break;
                }
            }
        }
        if (charging_success) {
            the_subscription[index_of_user].plan_type = -1;
            the_subscription[index_of_user].zones = -1;
            the_subscription[index_of_user].remaining_trips = 0;
            the_subscription[index_of_user].activation_day = { 0, 0, 0 };
            the_subscription[index_of_user].expiry_day = { 0, 0, 0 };
            Setcolor(yellow);
            cout << "Subscription set successfully to Smart Wallet with ";
            Setcolor(white);
            cout << the_subscription[index_of_user].wallet_balance;
            Setcolor(yellow);
            cout << " LE!\n";

        }
    }
    int price = 0;
    if (the_subscription[index_of_user].plan_type == monthly) {
        price = prices_monthly[the_subscription[index_of_user].zones - 1];
    }
    else if (the_subscription[index_of_user].plan_type == yearly) {
        price = prices_yearly[the_subscription[index_of_user].zones - 1];
    }
    else if (the_subscription[index_of_user].plan_type == three_months) {
        calculate_birthday(index_of_user);
        if ((users[index_of_user].age <= 26) && (users[index_of_user].age >= 16)) {
            price = prices_3months[the_subscription[index_of_user].zones - 1];
        }
        else {
            Setcolor(purple);
            cout << "You Are " << users[index_of_user].age << " years old.\n";
            cout << "You can not renew this subscription.. you are not in the range\n";
            return;
        }
    }
    if (deduct_price(price, index_of_user)) {
        if (the_subscription[index_of_user].subscription_type != smart_wallet) {
            Setcolor(yellow);
            cout << "Renewal succesful\n";
        }
        if (the_subscription[index_of_user].plan_type == monthly) {
            the_subscription[index_of_user].remaining_trips += 60;
        }
        else if (the_subscription[index_of_user].plan_type == yearly) {
            the_subscription[index_of_user].remaining_trips += 730;
        }
        else if (the_subscription[index_of_user].plan_type == three_months) {
            the_subscription[index_of_user].remaining_trips += 180;
        }
        if (is_subscription_active(index_of_user)) { //calculate the new expiry date from the old expiry date bec. the subs. is still active
            if (the_subscription[index_of_user].subscription_type == scholar) {
                the_subscription[index_of_user].expiry_day.month += 3;
            }
            else if (the_subscription[index_of_user].plan_type == monthly) {
                the_subscription[index_of_user].expiry_day.month += 1;
            }
            else {
                the_subscription[index_of_user].expiry_day.month += 12;
            }
            while (the_subscription[index_of_user].expiry_day.month > 12) {
                the_subscription[index_of_user].expiry_day.month -= 12;
                the_subscription[index_of_user].expiry_day.year++;
            }
            int max_day;
            switch (the_subscription[index_of_user].expiry_day.month) {
            case 1: case 3: case 5: case 7: case 8: case 10: case 12: max_day = 31; break;
            case 2: max_day = ((the_subscription[index_of_user].expiry_day.year % 4 == 0 && the_subscription[index_of_user].expiry_day.year % 100 != 0) ||
                (the_subscription[index_of_user].expiry_day.year % 400 == 0)) ? 29 : 28; break;
            default: max_day = 30;
            }
            if (the_subscription[index_of_user].expiry_day.day > max_day) {
                the_subscription[index_of_user].expiry_day.day = max_day;
            }
        }
        else { //calculate the new expiry date from the renewal day bec. the subs. is not active
            time_t timestamp = time(NULL);
            struct tm current_time;
            localtime_s(&current_time, &timestamp);
            the_subscription[index_of_user].activation_day.day = current_time.tm_mday;
            the_subscription[index_of_user].activation_day.month = current_time.tm_mon + 1;
            the_subscription[index_of_user].activation_day.year = current_time.tm_year + 1900;
            the_subscription[index_of_user].expiry_day.day = the_subscription[index_of_user].activation_day.day;
            the_subscription[index_of_user].expiry_day.month = the_subscription[index_of_user].activation_day.month;
            the_subscription[index_of_user].expiry_day.year = the_subscription[index_of_user].activation_day.year;
            if (the_subscription[index_of_user].subscription_type == scholar) {
                the_subscription[index_of_user].expiry_day.month += 3;
            }
            else if (the_subscription[index_of_user].plan_type == monthly) {
                the_subscription[index_of_user].expiry_day.month += 1;
            }
            else {
                the_subscription[index_of_user].expiry_day.month += 12;
            }
            while (the_subscription[index_of_user].expiry_day.month > 12) {
                the_subscription[index_of_user].expiry_day.month -= 12;
                the_subscription[index_of_user].expiry_day.year++;
            }
            int max_day;
            switch (the_subscription[index_of_user].expiry_day.month) {
            case 1: case 3: case 5: case 7: case 8: case 10: case 12: max_day = 31; break;
            case 2: max_day = ((the_subscription[index_of_user].expiry_day.year % 4 == 0 && the_subscription[index_of_user].expiry_day.year % 100 != 0) ||
                (the_subscription[index_of_user].expiry_day.year % 400 == 0)) ? 29 : 28; break;
            default: max_day = 30;
            }
            if (the_subscription[index_of_user].expiry_day.day > max_day) {
                the_subscription[index_of_user].expiry_day.day = max_day;
            }
        }
    }
    else {
        if (the_subscription[index_of_user].subscription_type != smart_wallet) {
            Setcolor(purple);
            cout << "insufficient balance to renew..!\n";
            return;
        }
    }
}

bool is_subscription_active(int index_of_user) { // Checking if the subs is active or not.
    if (index_of_user == -1) return false;
    time_t timestamp = time(NULL);
    struct tm current_time;
    localtime_s(&current_time, &timestamp);
    int current_year = current_time.tm_year + 1900;
    int current_month = current_time.tm_mon + 1;
    int current_day = current_time.tm_mday;
    if (the_subscription[index_of_user].expiry_day.year > current_year) return true;
    if (the_subscription[index_of_user].expiry_day.year == current_year) {
        if (the_subscription[index_of_user].expiry_day.month > current_month) return true;
        if (the_subscription[index_of_user].expiry_day.month == current_month) {
            if (the_subscription[index_of_user].expiry_day.day >= current_day) return true;
        }
    }
    return false;
}

void upgrade_subscription(int index_of_user)
{
    if (index_of_user == -1) {
        Setcolor(purple);
        cout << "Please sign in first!\n";
        return;
    }
    if (the_subscription[index_of_user].subscription_type == -1) {
        Setcolor(purple);
        cout << "No active subscription to upgrade.\n";
        return;
    }
    int old_subscription = the_subscription[index_of_user].subscription_type;
    int old_plan = the_subscription[index_of_user].plan_type;
    int old_zone = the_subscription[index_of_user].zones;
    int new_subscription_type = choose_subscription(index_of_user);
    if (new_subscription_type == 3) {
        the_subscription[index_of_user].subscription_type = old_subscription;
        return;
    }
    int new_plan_type = -1;
    int new_zone = -1;
    if (new_subscription_type == general) {
        Setcolor(white);
        cout << "\nPlease enter the plan type:\n";
        cout << "0 for monthly\n1 for yearly\n";
        new_plan_type = validation(0, 1);
        cout << "\nYou have 4 zones:\n";
        if (new_plan_type == monthly) {
            cout << "Zone 1 (1 - 9 stations): " << prices_monthly[0] << " LE\n";
            cout << "Zone 2 (10 - 16 stations): " << prices_monthly[1] << " LE\n";
            cout << "Zone 3 (17 - 23 stations): " << prices_monthly[2] << " LE\n";
            cout << "Zone 4 (More than 23 stations): " << prices_monthly[3] << " LE\n";
        }
        else {
            cout << "Zone 1 (1 - 9 stations): " << prices_yearly[0] << " LE\n";
            cout << "Zone 2 (10 - 16 stations): " << prices_yearly[1] << " LE\n";
            cout << "Zone 3 (17 - 23 stations): " << prices_yearly[2] << " LE\n";
            cout << "Zone 4 (More than 23 stations): " << prices_yearly[3] << " LE\n";
        }
        new_zone = validation(1, 4);
    }
    else if (new_subscription_type == scholar) {
        calculate_birthday(index_of_user);
        if ((users[index_of_user].age <= 26) && (users[index_of_user].age >= 16)) {
            new_plan_type = three_months;
            Setcolor(white);
            cout << "\nYou have 4 zones:\n";
            cout << "Zone 1 (1 - 9 stations): " << prices_3months[0] << " LE\n";
            cout << "Zone 2 (10 - 16 stations): " << prices_3months[1] << " LE\n";
            cout << "Zone 3 (17 - 23 stations): " << prices_3months[2] << " LE\n";
            cout << "Zone 4 (More than 23 stations): " << prices_3months[3] << " LE\n";
            new_zone = validation(1, 4);
        }
        else {
            Setcolor(purple);
            cout << "You Are " << users[index_of_user].age << " years old.\n";
            cout << "Your age is not in tha range.. you can not have this type.\n";
            the_subscription[index_of_user].subscription_type = old_subscription;
            return;
        }
    }
    if (new_subscription_type == old_subscription &&
        new_plan_type == old_plan &&
        new_zone == old_zone) {
        Setcolor(purple);
        cout << "You have already had this subscription\n";
        cout << "Back to the user menu...\n";
        return;
    }
    else {
        if (new_subscription_type != smart_wallet) {
            bool payment_success = false;
            while (!payment_success) {
                the_subscription[index_of_user].subscription_type = new_subscription_type;
                the_subscription[index_of_user].plan_type = new_plan_type;
                the_subscription[index_of_user].zones = new_zone;
                payment_success = payment_subscription(index_of_user);
                if (!payment_success) {
                    Setcolor(purple);
                    cout << "Payment failed: Insufficient balance! Current balance: " << users[index_of_user].balance << " LE\n";
                    if (!try_again()) {
                        the_subscription[index_of_user].subscription_type = old_subscription;
                        the_subscription[index_of_user].plan_type = old_plan;
                        the_subscription[index_of_user].zones = old_zone;
                        break;
                    }
                    Setcolor(white);
                    cout << "You have 4 zones:\n";
                    if (new_plan_type == monthly) {
                        cout << "Zone 1 (1 - 9 stations): " << prices_monthly[0] << " LE\n";
                        cout << "Zone 2 (10 - 16 stations): " << prices_monthly[1] << " LE\n";
                        cout << "Zone 3 (17 - 23 stations): " << prices_monthly[2] << " LE\n";
                        cout << "Zone 4 (More than 23 stations): " << prices_monthly[3] << " LE\n";
                    }
                    else if (new_plan_type == yearly) {
                        cout << "Zone 1 (1 - 9 stations): " << prices_yearly[0] << " LE\n";
                        cout << "Zone 2 (10 - 16 stations): " << prices_yearly[1] << " LE\n";
                        cout << "Zone 3 (17 - 23 stations): " << prices_yearly[2] << " LE\n";
                        cout << "Zone 4 (More than 23 stations): " << prices_yearly[3] << " LE\n";
                    }
                    else if (new_plan_type == three_months) {
                        cout << "Zone 1 (1 - 9 stations): " << prices_3months[0] << " LE\n";
                        cout << "Zone 2 (10 - 16 stations): " << prices_3months[1] << " LE\n";
                        cout << "Zone 3 (17 - 23 stations): " << prices_3months[2] << " LE\n";
                        cout << "Zone 4 (More than 23 stations): " << prices_3months[3] << " LE\n";
                    }
                    new_zone = validation(1, 4);
                }
            }
            if (payment_success) {
                Setcolor(yellow);
                cout << "Subscription upgraded successfully!\n";
                cout << "Activation date: ";
                Setcolor(white);
                cout << the_subscription[index_of_user].activation_day.day << "/"
                    << the_subscription[index_of_user].activation_day.month << "/" << the_subscription[index_of_user].activation_day.year << "\n";
                Setcolor(yellow);
                cout << "Expiry date: ";
                Setcolor(white);
                cout << the_subscription[index_of_user].expiry_day.day << "/"
                    << the_subscription[index_of_user].expiry_day.month << "/" << the_subscription[index_of_user].expiry_day.year << "\n";
                if (old_subscription == smart_wallet) {
                    users[index_of_user].balance += the_subscription[index_of_user].wallet_balance;
                }
            }
            else {
                the_subscription[index_of_user].subscription_type = old_subscription;
                the_subscription[index_of_user].plan_type = old_plan;
                the_subscription[index_of_user].zones = old_zone;
            }
        }
        else {
            bool charging_success = false;
            while (!charging_success) {
                charging_success = wallet_charging(index_of_user);
                if (!charging_success)
                {
                    Setcolor(purple);
                    cout << "Charging failed. Please try again or cancel.\n";
                    if (!try_again()) {
                        Setcolor(purple);
                        cout << "Charging cancelled.\n";
                        Setcolor(yellow);
                        cout << "NOTE: Subscription set to Smart Wallet, but the wallet has no funds!\n";
                        break;
                    }
                }
            }
            if (charging_success) {
                the_subscription[index_of_user].plan_type = -1;
                the_subscription[index_of_user].zones = -1;
                the_subscription[index_of_user].remaining_trips = 0;
                the_subscription[index_of_user].activation_day = { 0, 0, 0 };
                the_subscription[index_of_user].expiry_day = { 0, 0, 0 };
                Setcolor(yellow);
                cout << "Subscription set successfully to Smart Wallet with ";
                Setcolor(white);
                cout << the_subscription[index_of_user].wallet_balance;
                Setcolor(yellow);
                cout << " LE!\n";
            }
        }
    }
}

bool try_again() { // Option to try again if the user enter invalid input.
    char choice;
    Setcolor(white);
    cout << "Would you like to try again? (y/n): ";
    cin >> choice;
    cin.ignore(1000,'\n');
    if (choice == 'y' || choice == 'Y') {
        return true;
    }
    if (choice == 'n' || choice == 'N') {
        return false;
    }
    Setcolor(purple);
    cout << "Invalid input. Please enter 'y' or 'n'.\n";
    return try_again(); // Recursive call for invalid input
}

int validation(int min, int max) {
    Setcolor(white);
    int input;
    if (!(cin >> input) || input < min || input > max) {
        Setcolor(purple);
        cout << "Invalid input. Please enter a number between " << min << " and " << max << ": ";
        cin.clear();
        cin.ignore(10000, '\n');
        return validation(min, max); // Recursive call for invalid input
    }
    cin.ignore();
    return input;
}

// Ride Management functions definition:-
// View specific line stations for user:-
void view_stations(int lineNum) {
    for (int counter = 0; counter < total_stations; counter++)
    {
        if (all_stations[counter].line == lineNum)
            cout << all_stations[counter].position << ". " << all_stations[counter].name << endl;
    }
}

// View zones fare for Smart Wallet commuters before booking a trip:-
void view_fare_price() {
    Setcolor(yellow);
    cout << "Zone 1 (1 - 9 stations): " << zones_fare[0] << " LE\n";
    cout << "Zone 2 (10 - 16 stations): " << zones_fare[1] << " LE\n";
    cout << "Zone 3 (17 - 23 stations): " << zones_fare[2] << " LE\n";
    cout << "Zone 4 (More than 23 stations): " << zones_fare[3] << " LE\n";
}

// Find station index in all_stations using its (line number & position in line):-
int find_station(int lineNum, int stationPosition) {
    for (int counter = 0; counter < total_stations; counter++)
    {
        if (all_stations[counter].line == lineNum && all_stations[counter].position == stationPosition)
            return counter;
    }
    return -1; // This line should technically never be reached, but it's added to avoid issues with non-void function rules.
}

// Find transition station index in all_stations using its entry & exit lines:-
int find_tStation(int entryLine = 0, int exitLine = 0) {
    for (int counter = 0; counter < total_stations; counter++)
    {
        if (all_stations[counter].is_transition)
        {
            if (all_stations[counter].connected_lines[0] == entryLine && all_stations[counter].connected_lines[1] == exitLine)
                return counter;
        }
    }

    return -1; // This line should technically never be reached, but it's added to avoid issues with non-void function rules.
}

// Calculate stations num in the same line:-
int calculate_stations(int line, int entryStation, int exitStation) {
    entryStation = find_station(line, entryStation);
    exitStation = find_station(line, exitStation);

    // Case of line 3:-
    if (line == 3)
    {
        // Both stations in the same branch.
        if (all_stations[entryStation].branch_ID == all_stations[exitStation].branch_ID)
            return abs(entryStation - exitStation) + 1;

        // Both stations in different parts.
        else
        {
            int kit_kat_index = find_tStation();
            int northern_branch_stations = 0;
            for (int counter = kit_kat_index + 1; all_stations[counter].branch_ID == 1; counter++)
                northern_branch_stations++;

            // Case 1: One in the main branch and the other one in northern or southern branch.
            if (all_stations[entryStation].branch_ID == 0 || all_stations[exitStation].branch_ID == 0)
            {
                if (all_stations[exitStation].branch_ID == 0)
                    swap(entryStation, exitStation);

                int kitkat_to_exit = all_stations[exitStation].branch_ID == 2 ? (exitStation - kit_kat_index - northern_branch_stations) : (exitStation - kit_kat_index);
                return (kit_kat_index - entryStation) + kitkat_to_exit + 1;
            }

            // Case 2: One in the northern and the other one in the southern.
            else
            {
                if (all_stations[exitStation].branch_ID == 1)
                    swap(entryStation, exitStation);

                return (entryStation - kit_kat_index) + (exitStation - kit_kat_index - northern_branch_stations) + 1;
            }
        }
    }

    else
        // Case line 1 or 2.
        return abs(entryStation - exitStation) + 1;
}

// Calculates the trip fare based on the pricing set by the admin:-
float calculate_fare(int stations_num) {
    if (stations_num >= 1 && stations_num <= 9)
        return zones_fare[0];
    else if (stations_num <= 16)
        return zones_fare[1];
    else if (stations_num <= 23)
        return zones_fare[2];
    else
        return zones_fare[3];
}

// Generating Ride ID (R + rides_counter):-
void generate_ride_ID(string& rideID) {
    int needed_digits = to_string(max_rides - 1).size();
    int counter_digits = to_string(rides_counter).size();

    if (counter_digits < needed_digits)
    {
        for (int counter = needed_digits - counter_digits; counter > 0; counter--)
            rideID.append("0");
    }

    rideID.append(to_string(rides_counter));
}

// Saving ride booking time:-
void save_date() {
    time_t timestamp = time(NULL);
    struct tm current_date;
    localtime_s(&current_date, &timestamp);
    all_rides[rides_counter].ride_date[0] = current_date.tm_mday;
    all_rides[rides_counter].ride_date[1] = 1 + current_date.tm_mon;
    all_rides[rides_counter].ride_date[2] = 1900 + current_date.tm_year;
}

// Simulating all booking process:-
void book_a_trip(int entryLine, int entryStation, int exitLine, int exitStation, string user_ID) {
    int stations_num = 0;
    // If entry and exit stations are in the same line.
    if (entryLine == exitLine)
        stations_num = calculate_stations(entryLine, entryStation, exitStation);

    // If entry and exit stations are in different lines.
    else
    {
        // Find the transition station between the two lines.
        int transition_station_entry = all_stations[find_tStation(entryLine, exitLine)].position;
        int transition_station_exit = all_stations[find_tStation(exitLine, entryLine)].position;

        // Calculate total stations: from entry to transition + from transition to exit.
        stations_num = calculate_stations(entryLine, entryStation, transition_station_entry);
        stations_num += calculate_stations(exitLine, transition_station_exit, exitStation) - 1;
    }

    if (stations_num == 1)
    {
        Setcolor(purple);
        cout << "Trip failed: Same station selected for entry and exit (transition station)." << endl;
        return;
    }

    float fare = calculate_fare(stations_num);
    int required_zone;
    if (stations_num <= 9)
        required_zone = 1;
    else if (stations_num <= 16)
        required_zone = 2;
    else if (stations_num <= 23)
        required_zone = 3;
    else
        required_zone = 4;


    // Check zone for subscription and balance for smart wallet:-
    if (the_subscription[index_of_user].subscription_type == smart_wallet)
    {
        if (the_subscription[index_of_user].wallet_balance < fare)
        {
            Setcolor(purple);
            cout << "Insufficient smart wallet balance to book the ticket." << endl;
            return;
        }
        else
            the_subscription[index_of_user].wallet_balance -= fare;
    }
    else if (the_subscription[index_of_user].subscription_type == general || the_subscription[index_of_user].subscription_type == scholar)
    {
        if (the_subscription[index_of_user].zones < required_zone)
        {
            Setcolor(purple);
            cout << "This trip doesn't match your subscription zone." << endl;
            return;
        }
        else
        {
            the_subscription[index_of_user].remaining_trips--;
        }
    }

    // Saveing successful ride details:-
    all_rides[rides_counter].user_ID = user_ID;
    generate_ride_ID(all_rides[rides_counter].ride_ID);
    all_rides[rides_counter].entry_station = all_stations[find_station(entryLine, entryStation)].name;
    all_rides[rides_counter].exit_station = all_stations[find_station(exitLine, exitStation)].name;
    save_date();
    all_rides[rides_counter].ride_fare = fare;

    // Display the recently booked ride details:-
    Setcolor(yellow);
    cout << "Your trip booked successfully! Here are the details:" << endl;
    Setcolor(white);
    cout << "----------------------------------------" << endl;
    Setcolor(yellow);
    cout << "Entry Station: ";
    Setcolor(white);
    cout << all_rides[rides_counter].entry_station << endl;
    Setcolor(yellow);
    cout << "Exit Station: ";
    Setcolor(white);
    cout << all_rides[rides_counter].exit_station << endl;
    Setcolor(yellow);
    cout << "Date: ";
    Setcolor(white);
    cout << all_rides[rides_counter].ride_date[0] << "/" << all_rides[rides_counter].ride_date[1] << "/" << all_rides[rides_counter].ride_date[2] << endl;
    if (the_subscription[index_of_user].subscription_type == smart_wallet) {
        Setcolor(yellow);
        cout << "Fare: ";
        Setcolor(white);
        cout << all_rides[rides_counter].ride_fare;
        Setcolor(yellow);
        cout << " LE" << endl;
    }
    Setcolor(white);
    cout << "----------------------------------------" << endl;

    rides_counter++;
}

// Displaying rides history of the currently signed-in user:-
void view_ride_history(string user_ID) {
    bool not_found = true;

    for (int counter = 0; counter < rides_counter; counter++)
    {
        if (all_rides[counter].user_ID == user_ID)
        {
            not_found = false;
            Setcolor(yellow);
            cout << "Entry Station: ";
            Setcolor(white);
            cout << all_rides[counter].entry_station << endl;
            Setcolor(yellow);
            cout << "Exit Station: ";
            Setcolor(white);
            cout << all_rides[counter].exit_station << endl;
            Setcolor(yellow);
            cout << "Date: ";
            Setcolor(white);
            cout << all_rides[counter].ride_date[0] << "/" << all_rides[counter].ride_date[1] << "/" << all_rides[counter].ride_date[2] << endl;
            if (the_subscription[index_of_user].subscription_type == smart_wallet) {
                Setcolor(yellow);
                cout << "Fare: ";
                Setcolor(white);
                cout << all_rides[counter].ride_fare;
                Setcolor(yellow);
                cout << " LE" << endl;
            }
            Setcolor(white);
            cout << "----------------------------------------" << endl;
        }
    }

    if (not_found) {
        Setcolor(purple);
        cout << "No rides found for this user." << endl;
    }
}

// File streaming functions definition:-
void reading_process() {
    // Reading global variables & arrays data:-
    ifstream globals("Globals_File.txt", ios::in);
    if (!globals.is_open()) {
        cout << "File was not opened successfully." << endl;
        return;
    }

    for (int counter = 0; counter < 4; counter++)
        globals >> prices_monthly[counter];

    for (int counter = 0; counter < 4; counter++)
        globals >> prices_yearly[counter];

    for (int counter = 0; counter < 4; counter++)
        globals >> prices_3months[counter];

    for (int counter = 0; counter < 3; counter++)
        globals >> stations_num[counter];

    globals >> total_stations;
    globals >> rides_counter;

    for (int counter = 0; counter < 4; counter++)
        globals >> zones_fare[counter];

    globals >> usersCount;
    globals >> idCount;
    globals.close();

    // Reading stations data from file : -
    ifstream stations("Stations_File.txt", ios::in);
    if (!stations.is_open()) {
        cout << "File was not opened successfully." << endl;
        return;
    }

    for (int counter = 0; counter < total_stations; counter++) {
        string line, position, is_transition, connected_line1, connected_line2, branch_ID;
        getline(stations, all_stations[counter].name, '|');
        getline(stations, line, '|');
        getline(stations, position, '|');
        getline(stations, is_transition, '|');
        getline(stations, connected_line1, '|');
        getline(stations, connected_line2, '|');
        getline(stations, branch_ID);

        all_stations[counter].line = stoi(line);
        all_stations[counter].position = stoi(position);
        all_stations[counter].is_transition = (is_transition == "1");
        all_stations[counter].connected_lines[0] = stoi(connected_line1);
        all_stations[counter].connected_lines[1] = stoi(connected_line2);
        all_stations[counter].branch_ID = stoi(branch_ID);
    }
    stations.close();

    // Reading users data from file : -
    ifstream users_data("User_File.txt", ios::in);
    if (!users_data.is_open()) {
        cout << "File was not opened successfully." << endl;
        return;
    }

    for (int counter = 1; counter < usersCount; counter++) {
        string balance, day, month, year, age, LoggedIn;
        getline(users_data, users[counter].user_ID, '|');
        users_data.getline(users[counter].username, 50, '|');
        users_data.getline(users[counter].password, 50, '|');
        getline(users_data, balance, '|');
        getline(users_data, day, '|');
        getline(users_data, month, '|');
        getline(users_data, year, '|');
        getline(users_data, age, '|');
        getline(users_data, users[counter].phone_num, '|');
        getline(users_data, LoggedIn);

        users[counter].balance = stof(balance);
        users[counter].user_date.day = stoi(day);
        users[counter].user_date.month = stoi(month);
        users[counter].user_date.year = stoi(year);
        users[counter].age = stoi(age);
        users[counter].LoggedIn = (LoggedIn == "1");
    }
    users_data.close();

    // Reading subscriptions data from file :-
    ifstream subscriptions("Subscription_File.txt", ios::in);
    if (!subscriptions.is_open()) {
        cout << "File was not opened successfully." << endl;
        return;
    }

    for (int counter = 1; counter < usersCount; counter++) {
        subscriptions >> the_subscription[counter].subscription_type;
        subscriptions >> the_subscription[counter].plan_type;
        subscriptions >> the_subscription[counter].remaining_trips;
        subscriptions >> the_subscription[counter].zones;
        subscriptions >> the_subscription[counter].wallet_balance;
        subscriptions >> the_subscription[counter].activation_day.day;
        subscriptions >> the_subscription[counter].activation_day.month;
        subscriptions >> the_subscription[counter].activation_day.year;
        subscriptions >> the_subscription[counter].expiry_day.day;
        subscriptions >> the_subscription[counter].expiry_day.month;
        subscriptions >> the_subscription[counter].expiry_day.year;
        getline(subscriptions, the_subscription[counter].user_id);
    }
    subscriptions.close();
    // Reading rides data:-

    ifstream rides("Ride_File.txt", ios::in);
    if (!rides.is_open()) {
        cout << "File was not opened successfully." << endl;
        return;
    }

    for (int counter = 0; counter < rides_counter; counter++) {
        string ride_day, ride_month, ride_year, ride_fare;
        getline(rides, all_rides[counter].user_ID, '|');
        getline(rides, all_rides[counter].ride_ID, '|');
        getline(rides, all_rides[counter].entry_station, '|');
        getline(rides, all_rides[counter].exit_station, '|');
        getline(rides, ride_day, '|');
        getline(rides, ride_month, '|');
        getline(rides, ride_year, '|');
        getline(rides, ride_fare);

        all_rides[counter].ride_date[0] = stoi(ride_day);
        all_rides[counter].ride_date[1] = stoi(ride_month);
        all_rides[counter].ride_date[2] = stoi(ride_year);
        all_rides[counter].ride_fare = stof(ride_fare);
    }
    rides.close();
}
void writing_process() {
    // Saving updates of global variables & arrays:-
    ofstream globals("Globals_File.txt", ios::out);
    if (!globals.is_open()) {
        cout << "File was not opened successfully." << endl;
        return;
    }

    for (int counter = 0; counter < 4; counter++)
        globals << prices_monthly[counter] << endl;

    for (int counter = 0; counter < 4; counter++)
        globals << prices_yearly[counter] << endl;

    for (int counter = 0; counter < 4; counter++)
        globals << prices_3months[counter] << endl;

    for (int counter = 0; counter < 3; counter++)
        globals << stations_num[counter] << endl;

    globals << total_stations << endl;
    globals << rides_counter << endl;

    for (int counter = 0; counter < 4; counter++)
        globals << zones_fare[counter] << endl;

    globals << usersCount << endl;
    globals << idCount << endl;
    globals.close();

    // Saving updates of stations data on file : -
    ofstream stations("Stations_File.txt", ios::out);
    if (!stations.is_open()) {
        cout << "File was not opened successfully." << endl;
        return;
    }

    for (int counter = 0; counter < total_stations; counter++) {
        stations << all_stations[counter].name << "|";
        stations << all_stations[counter].line << "|";
        stations << all_stations[counter].position << "|";
        stations << all_stations[counter].is_transition << "|";
        stations << all_stations[counter].connected_lines[0] << "|";
        stations << all_stations[counter].connected_lines[1] << "|";
        stations << all_stations[counter].branch_ID << endl;
    }
    stations.close();

    // Saving updates of users:-
    ofstream users_data("User_File.txt", ios::out);
    if (!users_data.is_open()) {
        cout << "File was not opened successfully." << endl;
        return;
    }

    for (int counter = 1; counter < usersCount; counter++) {
        users_data << users[counter].user_ID << "|";
        users_data << users[counter].username << "|";
        users_data << users[counter].password << "|";
        users_data << users[counter].balance << "|";
        users_data << users[counter].user_date.day << "|";
        users_data << users[counter].user_date.month << "|";
        users_data << users[counter].user_date.year << "|";
        users_data << users[counter].age << "|";
        users_data << users[counter].phone_num << "|";
        users_data << users[counter].LoggedIn << endl;
    }
    users_data.close();

    // Saving updates of users:-
    ofstream subscriptions("Subscription_File.txt", ios::out);
    if (!subscriptions.is_open()) {
        cout << "File was not opened successfully." << endl;
        return;
    }

    for (int counter = 1; counter < usersCount; counter++) {
        subscriptions << the_subscription[counter].subscription_type << " ";
        subscriptions << the_subscription[counter].plan_type << " ";
        subscriptions << the_subscription[counter].remaining_trips << " ";
        subscriptions << the_subscription[counter].zones << " ";
        subscriptions << the_subscription[counter].wallet_balance << " ";
        subscriptions << the_subscription[counter].activation_day.day << " ";
        subscriptions << the_subscription[counter].activation_day.month << " ";
        subscriptions << the_subscription[counter].activation_day.year << " ";
        subscriptions << the_subscription[counter].expiry_day.day << " ";
        subscriptions << the_subscription[counter].expiry_day.month << " ";
        subscriptions << the_subscription[counter].expiry_day.year << " ";
        subscriptions << the_subscription[counter].user_id << endl;
    }
    subscriptions.close();

    // Saving updates of rides data:-
    ofstream rides("Ride_File.txt", ios::out);
    if (!rides.is_open()) {
        cout << "File was not opened successfully." << endl;
        return;
    }

    for (int counter = 0; counter < rides_counter; counter++) {
        rides << all_rides[counter].user_ID << "|";
        rides << all_rides[counter].ride_ID << "|";
        rides << all_rides[counter].entry_station << "|";
        rides << all_rides[counter].exit_station << "|";
        for (int i = 0; i < 3; i++)
            rides << all_rides[counter].ride_date[i] << "|";
        rides << all_rides[counter].ride_fare << endl;
    }
    rides.close();
}
// Admin functions definition:-
// Modify and view user data by admin function:-
void Edit_Commuter() {
    int choice1;
    int commuterIndex = -1;
    string Userid;
    Setcolor(white);
    cout << "1. View commuter" << endl << "2. Modify commuter" << endl << "3. Remove commuter" << endl << "4. Back\n";
    cout << "What do you want to do ? ";
    choice1 = validation(1, 4);
    if (choice1 != 4)
    {
        cout << "Please Enter User_Id : ";
        cin >> Userid;
        for (int i = 0; i < usersCount; i++)
        {
            if (Userid == users[i].user_ID)
            {
                commuterIndex = i;
                break;
            }
        }
        if (commuterIndex == -1)
        {
            Setcolor(purple);
            cout << "Invalid User ID." << endl;
            return;
        }
        else if (commuterIndex == 0) {
            Setcolor(purple);
            cout << "Admin can not be modified!\n";
            return;
        }
    }
    if (choice1 == 1)
        Viewuser(commuterIndex);
    else if (choice1 == 2)
        modifyUser(commuterIndex);
    else if (choice1 == 3)
        remove_commuter(commuterIndex);
}

void Viewuser(int user_Index) {
    Setcolor(yellow);
    cout << "User ID: ";
    Setcolor(white);
    cout << users[user_Index].user_ID << endl;
    Setcolor(yellow);
    cout << "Username: ";
    Setcolor(white);
    cout << users[user_Index].username << endl;
    Setcolor(yellow);
    cout << "Balance: ";
    Setcolor(white);
    cout << users[user_Index].balance;
    Setcolor(yellow);
    cout << " LE" << endl;
    cout << "Phone number: ";
    Setcolor(white);
    cout << users[user_Index].phone_num << endl;
    //modify index of user
    index_of_user = user_Index;
    view_subscription(index_of_user);
    index_of_user = 0;
}

void modifyUser(int user_Index) {
    int choice1;
    char newUsername[MaxName];
    float newBalance;
    string new_phone;
    Setcolor(white);
    cout << "Modify commuter: " << endl << "1. Username" << endl << "2. Balance" << endl << "3. Phone number" << endl << "4. Back" << endl;
    choice1 = validation(1, 4);
    switch (choice1)
    {
    case 1:
        cout << "Enter Username: ";
        cin.getline(newUsername, MaxName);
        for (int i = 0; i < usersCount; i++)
        {
            if (strcmp(users[i].username, newUsername) == 0)   //strcmp compares the string content not the memory address 
            {
                Setcolor(purple);
                cout << "This username has already taken..\n";
                return;
            }
        }
        strcpy_s(users[user_Index].username, newUsername);
        Setcolor(yellow);
        cout << "Commuter Username modified successfully" << endl;
        break;
    case 2:
        Setcolor(white);
        cout << "Enter the amount you want to add to this account: ";
        cin >> newBalance;
        if (newBalance <= 0)
        {
            Setcolor(purple);
            cout << "Amount must be greater than zero." << endl;
            return;
        }
        users[user_Index].balance += newBalance;
        Setcolor(yellow);
        cout << "Charged successfully! Current balance = ";
        Setcolor(white);
        cout << users[user_Index].balance;
        Setcolor(yellow);
        cout << " LE.\n";
        break;
    case 3:
        Setcolor(white);
        cout << "Enter the new phone number:\n";
        cin >> new_phone;
        while (true) {
            // checking the length
            if (new_phone.length() != 11) {
                Setcolor(purple);
                cout << "Phone number must be 11 digits!" << endl;
            }
            // checking that the user does not enter letters
            else {
                bool isDigits = true;
                for (char c : new_phone) {
                    if ((c < '0') || (c > '9')) {
                        isDigits = false;
                        break;
                    }
                }
                if (!isDigits) {
                    Setcolor(purple);
                    cout << "Phone number must contain digits only!" << endl;
                }
                // checking the starts
                else {
                    string prefix = new_phone.substr(0, 3);
                    if (prefix != "015" && prefix != "012" && prefix != "011" && prefix != "010") {
                        Setcolor(purple);
                        cout << "Phone number must start with 015, 012, 011, or 010!" << endl;
                    }
                    else {
                        break; //the number is valid break the loop
                    }
                }
            }
            // enter again the number
            Setcolor(white);
            cout << "Please enter your phone number again:\n";
            cin >> new_phone;
        }
        users[user_Index].phone_num = new_phone;
        Setcolor(yellow);
        cout << "Number changed successfully\n";
        break;
    case 4:
        return;
    }
}

void remove_commuter(int user_Index) {
    for (int i = user_Index; i < usersCount - 1; ++i)
        users[i] = users[i + 1];
    //to remove the user
    strcpy_s(users[usersCount - 1].username, "");
    strcpy_s(users[usersCount - 1].password, "");
    users[usersCount - 1].user_ID = "U";
    users[usersCount - 1].balance = 0.0;
    users[usersCount - 1].LoggedIn = false;
    users[usersCount - 1].user_date = { 0 ,0 ,0 };
    users[usersCount - 1].phone_num = "";
    users[usersCount - 1].age = 0;
    //to remove the subscription of the user
    the_subscription[usersCount - 1].subscription_type = -1;
    the_subscription[usersCount - 1].plan_type = -1;
    the_subscription[usersCount - 1].zones = -1;
    the_subscription[usersCount - 1].remaining_trips = 0;
    the_subscription[usersCount - 1].expiry_day = { 0, 0,0 };
    the_subscription[usersCount - 1].activation_day = { 0, 0,0 };
    the_subscription[usersCount - 1].wallet_balance = 0;
   
    usersCount--;
    Setcolor(yellow);
    cout << "User removed successfully!\n";
}

void edit_subscription() {
    int choice1;
    string Userid;
    int indexOfCommuter;

    while (true) {
        Setcolor(white);
        cout << "\nSubscription Management Menu:\n";
        cout << "1. Add Subscription\n";
        cout << "2. Modify Subscription\n";
        cout << "3. Discontinue Subscription\n";
        cout << "4. Back to Admin Page\n";
        cout << "What do you want to do? ";
        choice1 = validation(1, 4);

        if (choice1 == 4) {
            return;
        }

        cout << "Please Enter User ID: ";
        cin >> Userid;

        bool found = false;
        for (int i = 0; i < usersCount; i++) {
            if (Userid == users[i].user_ID) {
                indexOfCommuter = i;
                found = true;
                break;
            }
        }
        if (!found) {
            Setcolor(purple);
            cout << "Invalid User ID. Please try again.\n";
            continue;
        }
        switch (choice1) {
        case 1: // Add Subscription
            addSubscription(indexOfCommuter);
            break;
        case 2: // Modify Subscription
            upgrade_subscription(indexOfCommuter);
            break;
        case 3: // Discontinue Subscription
            if (the_subscription[indexOfCommuter].subscription_type == -1) {
                Setcolor(purple);
                cout << "User " << Userid << " has no active subscription to discontinue.\n";
            }
            else {
                Discontinue_Subscription(indexOfCommuter);
            }
            break;
        }
        return;
    }
}

void addSubscription(int user_Index) {
    if (the_subscription[user_Index].subscription_type != -1) {
        char choice;
        Setcolor(purple);
        cout << "You already have an active subscription. Do you want to remove it and add a new one? (Y/N): ";
        Setcolor(white);
        cin >> choice;
        if (choice == 'Y' || choice == 'y') {
            Discontinue_Subscription(user_Index); // Remove current subscription
            Setcolor(yellow);
            cout << "Old subscription removed. Now you can add a new subscription.\n";
        }
        else {
            cout << "You chose not to remove the existing subscription. No changes made.\n";
            return;
        }
    }
    int newSubscriptionType;
    Setcolor(white);
    cout << "\n1. General Plan\n";
    cout << "2. Scholar Plan\n";
    cout << "3. Smart Wallet\n";
    cout << "4. Back\n\n";
    cout << "Enter new subscription type: ";
    newSubscriptionType = validation(1, 4);
    switch (newSubscriptionType) {
    case 1:
        the_subscription[user_Index].subscription_type = general;
        general_plan(user_Index);
        return;
    case 2:
        calculate_birthday(user_Index);
        if ((users[user_Index].age >= 16) && (users[user_Index].age <= 26)) {
            the_subscription[user_Index].subscription_type = scholar;
            scholar_plan(user_Index);
            return;
        }
        else {
            Setcolor(purple);
            cout << "You Are " << users[user_Index].age << " years old.\n";
            cout << "You are not in the range ..You can not have this type.\n";
            return;
        }
    case 3:
        the_subscription[user_Index].subscription_type = smart_wallet;
        wallet_charging(user_Index);
        Setcolor(white);
        cout << "_____\n";
        cout << "Returning to Admin Page.. \n";
        cout << "Note:\nThe wallet is created and the balance is " << the_subscription[user_Index].wallet_balance << endl;
        cout << "If you want to recharge it, Go to modify plan in edit subscription.\n";
        return;
    case 4:
        Setcolor(yellow);
        cout << "Exiting subscription process.\n";
        return;

    default:
        Setcolor(purple);
        cout << "INVALID CHOICE! No subscription added.\n";
        return;
    }
}

void Discontinue_Subscription(int user_Index) {
    // Check if subscription exists first
    if (the_subscription[user_Index].subscription_type == -1) {
        Setcolor(purple);
        cout << "This user doesn't have an active subscription to discontinue.\n";
        return;
    }
    // Clear subscription data
    the_subscription[user_Index].plan_type = -1;
    the_subscription[user_Index].subscription_type = -1;
    the_subscription[user_Index].remaining_trips = 0;
    the_subscription[user_Index].zones = -1;
    users[user_Index].balance += the_subscription[user_Index].wallet_balance;
    the_subscription[user_Index].activation_day = { 0, 0, 0 };
    the_subscription[user_Index].expiry_day = { 0, 0, 0 };
    time_t now = time(0);
    tm localTime;
    localtime_s(&localTime, &now);
    Setcolor(yellow);
    cout << "Subscription discontinued for user ";
    Setcolor(white);
    cout << users[user_Index].user_ID << "\n";
    Setcolor(yellow);
    cout << "Date: ";
    Setcolor(white);
    cout << localTime.tm_mday << "/" << (localTime.tm_mon + 1) << "/" << (localTime.tm_year + 1900) << "\n";
    cout << "--------------------------------------------------\n";
}

void viewRides() {
    Setcolor(white);
    cout << "\nALL RIDE HISTORIES" << endl;
    cout << "-------------------------" << endl;
    for (int i = 0; i < rides_counter; i++) {
        Setcolor(yellow);
        cout << "Ride #";
        Setcolor(white);
        cout << (i + 1) << endl;
        Setcolor(yellow);
        cout << "User: ";
        Setcolor(white);
        cout << all_rides[i].user_ID << endl;
        Setcolor(yellow);
        cout << "Route: ";
        Setcolor(white);
        cout << all_rides[i].entry_station << " -> " << all_rides[i].exit_station << endl;
        Setcolor(yellow);
        cout << "Date: ";
        Setcolor(white);
        cout << all_rides[i].ride_date[0] << "/" << all_rides[i].ride_date[1] << "/" << all_rides[i].ride_date[2] << endl;
        if (the_subscription[index_of_user].subscription_type == smart_wallet) {
            Setcolor(yellow);
            cout << "Fare: ";
            Setcolor(white);
            cout << all_rides[i].ride_fare;
            Setcolor(yellow);
            cout << " LE" << endl;
        }
    }

    if (rides_counter == 0) {
        Setcolor(purple);
        cout << "\nNo rides found in the system.\n";
    }
    else {
        Setcolor(white);
        cout << "\nTotal rides: " << rides_counter << endl;
    }
    Setcolor(white);
    cout << "=========================\n";
}

void StationManagement() {
    int choice1;
    Setcolor(white);
    cout << "\n1. Add Station " << endl << "2. Edit Station" << endl << "3. Remove Station" << endl << "4. Back" << endl;
    cout << "Enter your choice: ";
    choice1 = validation(1, 4);
    switch (choice1) {
    case 1:
        AddStation();
        break;
    case 2:
        EditStation();
        break;
    case 3:
        RemoveStation();
        break;
    case 4:
        return;
    }
}

// Renumering stations after Addition & Deletion:-
void renumering() {
    int position = 1;
    for (int counter = 0; counter < stations_num[0]; counter++)
        all_stations[counter].position = position++; // BE CAREFULL -> position++ NOT ++position.

    position = 1;
    for (int counter = stations_num[0]; counter < stations_num[0] + stations_num[1]; counter++)
        all_stations[counter].position = position++;

    position = 1;
    for (int counter = stations_num[0] + stations_num[1]; counter < total_stations; counter++)
        all_stations[counter].position = position++;
}

// Adding new station.. this function is zero-based:-
void AddStation() {
    if (total_stations >= max_stations) {
        Setcolor(purple);
        cout << "Maximum number of stations reached! Cannot add a new station." << endl;
        return;
    }
    Setcolor(purple);
    cout << "NOTE:\nThe numbering of stations is zero_based -> from 0 to " << total_stations - 1 << ".\n";
    cout << "\n";
    //checking if we exceed the limit
    string newname;
    int newline, insert_index, new_branch_ID = 0;
    Setcolor(white);
    cout << "Please enter name of station: ";
    getline(cin, newname);
    for (int counter = 0; counter < total_stations; counter++) {
        if (all_stations[counter].name == newname) {
            Setcolor(purple);
            cout << "This station name already exists." << endl;
            return;
        }//checking that the station name is taken or not
    }
    Setcolor(white);
    cout << "Please enter the line (1, 2, or 3): ";
    newline = validation(1, 3);
    //taking the target line
    int line_start, line_end;
    if (newline == 1) {
        line_start = 0;
        line_end = stations_num[0];
        cout << "Line 1: from " << line_start << " to " << line_end - 1 << endl;
        for (int counter = 0; counter < total_stations; counter++) {
            if (all_stations[counter].line == newline)
                cout << counter << ". " << all_stations[counter].name << endl;
        }//display the stations and its index in line 1
        cout << "Please enter the index of station (" << line_start << " to " << line_end << "): ";
        insert_index = validation(line_start, line_end);//taking the target index
        stations_num[0]++;
    }
    else if (newline == 2) {
        line_start = stations_num[0];
        line_end = stations_num[0] + stations_num[1];
        cout << "Line 2: from " << line_start << " to " << line_end - 1 << endl;
        for (int counter = line_start; counter < total_stations; counter++)
        {
            if (all_stations[counter].line == newline)
                cout << counter << ". " << all_stations[counter].name << endl;
        }//display the stations and its index in line 2
        cout << "Please enter the index of station (" << line_start << " to " << line_end << "): ";
        insert_index = validation(line_start, line_end);
        stations_num[1]++;
    }
    else {
        line_start = stations_num[0] + stations_num[1];
        line_end = total_stations;
        int main_branch_start = line_start;
        int main_branch_end = main_branch_start; //it will be adjusted
        int north_branch_start = -1, north_branch_end = -1;
        int south_branch_start = -1, south_branch_end = -1;
        //the end and start of each branch
        for (int i = line_start; i < total_stations; i++) {//this loop adjust the start and end of each branch
            //according to id branch
            if (all_stations[i].branch_ID == 0) {
                main_branch_end = i + 1;
            }
            else if (all_stations[i].branch_ID == 1) {
                if (north_branch_start == -1) north_branch_start = i;
                north_branch_end = i + 1;
            }
            else if (all_stations[i].branch_ID == 2) {
                if (south_branch_start == -1) south_branch_start = i;
                south_branch_end = i + 1;
            }
        }
        cout << "Line 3 branches:" << endl;
        cout << "Main branch (branch_ID = 0): from " << main_branch_start << " to " << main_branch_end - 1 << endl;
        cout << "Northern branch (branch_ID = 1): from " << north_branch_start << " to " << north_branch_end - 1 << endl;
        cout << "Southern branch (branch_ID = 2): from " << south_branch_start << " to " << south_branch_end - 1 << endl;
        cout << "___\n\n";
        cout << "Please enter the branch ID (0. main, 1. northern, 2. southern): ";
        new_branch_ID = validation(0, 2);
        if (new_branch_ID == 0) {
            line_start = main_branch_start;
            line_end = main_branch_end - 1; // -1 to prevent putting any station after kit kat and in main branch
            //if we want to put station after kit kat we do it in north,south branches
        }
        else if (new_branch_ID == 1) {
            line_start = north_branch_start;
            line_end = north_branch_end;
        }
        else {
            line_start = south_branch_start;
            line_end = south_branch_end;
        }
        for (int counter = line_start; counter < total_stations; counter++) {
            if (all_stations[counter].branch_ID == new_branch_ID)
                cout << counter << ". " << all_stations[counter].name << endl;
        }
        cout << "Please enter the index of station (" << line_start << " to " << line_end << "):\n";
        insert_index = validation(line_start, line_end);
        stations_num[2]++;
    }
    //shifting the stations
    for (int i = total_stations; i > insert_index; i--) {
        all_stations[i] = all_stations[i - 1];
    }
    //putting the right data in the station array
    all_stations[insert_index].name = newname;
    all_stations[insert_index].line = newline;
    all_stations[insert_index].position = 0; //adjusting it from calling renumering
    all_stations[insert_index].is_transition = false;
    all_stations[insert_index].connected_lines[0] = -1;
    all_stations[insert_index].connected_lines[1] = -1;
    all_stations[insert_index].branch_ID = new_branch_ID;

    total_stations++;
    renumering();
    Setcolor(yellow);
    cout << "The new station: " << newname << " is added successfully to the index " << insert_index << "!" << endl;
}

void EditStation() {
    int line, stationPosition, indexToEdit;
    string newName;
    Setcolor(white);
    cout << "Enter station's line (1-3):- ";
    line = validation(1, 3);
    cout << "Line " << line << " stations:" << endl;
    cout << "--------------------" << endl;
    view_stations(line);
    cout << "\nEnter station's position: ";
    stationPosition = validation(1, stations_num[line - 1]);
    indexToEdit = find_station(line, stationPosition);
    if (all_stations[indexToEdit].is_transition == true) {
        Setcolor(purple);
        cout << "You cannot edit a transition station." << endl;
        return;
    }
    Setcolor(white);
    cout << "Enter new name for the station:- ";
    getline(cin, newName);
    for (int counter = 0; counter < total_stations; counter++) {
        if (all_stations[counter].name == newName) {
            Setcolor(purple);
            cout << "This station name already exists." << endl;
            return;
        }
    }
    Setcolor(yellow);
    all_stations[indexToEdit].name = newName;
    cout << "Name changed successfully.\n";
}

void RemoveStation() {
    int line, stationPosition, indexToRemove;
    Setcolor(white);
    cout << "Enter station's line (1-3):- ";
    line = validation(1, 3);
    cout << "Line " << line << " stations:" << endl;
    cout << "--------------------" << endl;
    view_stations(line);
    cout << "\nEnter station's position: ";
    stationPosition = validation(1, stations_num[line - 1]);
    indexToRemove = find_station(line, stationPosition);
    if (all_stations[indexToRemove].is_transition == true) {
        Setcolor(purple);
        cout << "You cannot remove a transition station." << endl;
        return;
    }
    for (int i = indexToRemove; i < total_stations - 1; i++)
        all_stations[i] = all_stations[i + 1];
    all_stations[total_stations - 1].name = "";
    all_stations[total_stations - 1].line = 0;
    all_stations[total_stations - 1].position = 0;
    all_stations[total_stations - 1].is_transition = false;
    all_stations[total_stations - 1].connected_lines[0] = -1;
    all_stations[total_stations - 1].connected_lines[1] = -1;
    all_stations[total_stations - 1].branch_ID = 0;
    total_stations--;
    stations_num[line - 1]--;
    renumering();
    Setcolor(yellow);
    cout << "Station removed successfully!\n";
}

// Modifing zones fare:-
void EditFare() {
    int zoneNumber;
    float newPrice;
    Setcolor(white);
    cout << "\nEnter number of zone you want to edit (1-4):\nEnter 5 to back.\n";
    zoneNumber = validation(1, 5);
    if (zoneNumber == 5)
        return;
    cout << "Enter a new price of zone (newPrice > 0): ";
    cin >> newPrice;
    while (newPrice <= 0) {
        Setcolor(purple);
        cout << "Invalid price, please try again.\n";
        cout << "1. try again\n2. back to admin page\n";
        int trying;
        Setcolor(white);
        trying = validation(1, 2);
        if (trying == 1) {
            return EditFare();
        }
        else {
            return;
        }
    }
    Setcolor(yellow);
    // Saving successful modifing details:-
    zones_fare[zoneNumber - 1] = newPrice;
    cout << "Fare changed successfully!\n";
}

// Updating commuter data by the user:-
void EditSubscriptionPrice() {
    int type, plan, zone;
    int new_price;
    Setcolor(white);
    cout << "\nPlease enter the subscription type\n0. general\n1. scholar\n2. back\n";
    type = validation(0, 2);
    if (type == 2) return;
    if (type == general) {
        cout << "Please choose the plan\n0. monthly\n1. yearly\n";
        plan = validation(0, 1);
    }
    cout << "Please enter the zone (1 -> 4):\n";
    zone = validation(1, 4);
    cout << "Enter a new price of zone (new Price > 0): ";
    cin >> new_price;
    while (new_price <= 0) {
        Setcolor(purple);
        cout << "Invalid price, please try again.\n";
        cout << "1. try again\n2. back to admin page\n";
        int trying;
        Setcolor(white);
        trying = validation(1, 2);
        if (trying == 1) return EditSubscriptionPrice();
        else  return;
    }
    if (type == general) {
        if (plan == monthly) {
            prices_monthly[zone - 1] = new_price;
        }
        else if (plan == yearly) {
            prices_yearly[zone - 1] = new_price;
        }
    }
    else {
        prices_3months[zone - 1] = new_price;
    }
    Setcolor(yellow);
    cout << "Price changed successfully!\n";
}

// Counting num of each subscription type:-
void statistics() {
    int general_counter = 0, scholar_counter = 0, smartwallet_counter = 0;
    for (int i = 1;i <= usersCount;i++) {
        if (the_subscription[i].subscription_type == general) {
            general_counter++;
        }
        else if (the_subscription[i].subscription_type == scholar) {
            scholar_counter++;
        }
        else if (the_subscription[i].subscription_type == smart_wallet) {
            smartwallet_counter++;
        }
    }

    cout << "\nGeneral subscription: " << general_counter << endl;
    cout << "Scholar subscription: " << scholar_counter << endl;
    cout << "Smart wallet subscription: " << smartwallet_counter << endl;

    Setcolor(yellow);

    if (scholar_counter > smartwallet_counter && scholar_counter > general_counter)
        cout << "THE MOST USED : SCHOLAR SUBSCRIBTION" << endl;

    else  if (smartwallet_counter > scholar_counter && smartwallet_counter > general_counter)
        cout << "THE MOST USED : SMART WALLET SUBSCRIPTION" << endl;

    else if (general_counter > scholar_counter && general_counter > smartwallet_counter)
        cout << "THE MOST USED : GENERAL SUBSCRIBTION" << endl;

    else  if (general_counter == scholar_counter && general_counter == smartwallet_counter)
        cout << "THEY ARE EQUALLY USED " << endl;

    else if (general_counter == scholar_counter && general_counter > smartwallet_counter)
        cout << "THE MOST USED : GENERAL AND SCHOLAR SUBSCRIPTIONS" << endl;

    else if (scholar_counter == smartwallet_counter && scholar_counter > general_counter)
        cout << "THE MOST USED :SCHOLAR AND SMART WALLET SUBSCRIPTIONS" << endl;

    else if (general_counter == smartwallet_counter && general_counter > scholar_counter)
        cout << "THE MOST USED: GENERAL AND SMART WALLET SUBSCRIPTIONS" << endl;

    Setcolor(white);
}

int main()
{
    //writing_process(); //Uncomment it if you run the code for the first time then comment it again.
    reading_process();
    Setcolor(white);
    cout << "Welcome to CairoRide!" << endl;
    cout << "\n";
    bool exit_program = false;
    while (!exit_program) {
        Setcolor(white);
        cout << "\nMain Menu:" << endl;
        cout << "____________\n\n";
        cout << "1. Create Account\n";
        cout << "2. Sign In\n";
        cout << "3. Exit\n\n";
        cout << "Enter choice: ";
        int choice = validation(1, 3);
        if (choice == 1)
            createAcc();
        else if (choice == 2) {
            signIn();
            if (index_of_user != -1 && users[index_of_user].LoggedIn && index_of_user != 0) {
                while (true) {
                    Setcolor(white);
                    cout << "\nUser Menu:" << endl;
                    cout << "____________\n\n";
                    cout << "1. Buy Travel Plan\n";
                    cout << "2. Manage Plan\n";
                    cout << "3. Start/End Trip\n";
                    cout << "4. Ride History\n";
                    cout << "5. Update Profile\n";
                    cout << "6. Sign Out\n\n";
                    cout << "Enter choice: ";
                    int commuter_choice = validation(1, 6);
                    if (commuter_choice == 1 && the_subscription[index_of_user].subscription_type == -1) { // The second condition to prevent the user from buying a travel plan twice 
                        int subscription_type = choose_subscription(index_of_user);
                        if (subscription_type == general) {
                            general_plan(index_of_user);

                        }
                        else if (subscription_type == scholar) {
                            calculate_birthday(index_of_user);
                            if ((users[index_of_user].age <= 26) && (users[index_of_user].age >= 16)) {
                                scholar_plan(index_of_user);

                            }
                            else {
                                Setcolor(purple);
                                cout << "You Are " << users[index_of_user].age << " years old.\n";
                                cout << "Your age is not in tha range.. you can not have this type.\n";
                                the_subscription[index_of_user].subscription_type = -1;

                            }
                        }
                        else if (subscription_type == smart_wallet) {
                            bool charging_success = false;
                            while (!charging_success) {
                                charging_success = wallet_charging(index_of_user);
                                if (!charging_success) {
                                    Setcolor(purple);
                                    cout << "Charging failed. Please try again or cancel.\n";
                                    if (!try_again()) {
                                        Setcolor(purple);
                                        cout << "Charging cancelled.\n";
                                        Setcolor(yellow);
                                        cout << "NOTE: Subscription set to Smart Wallet, but the wallet has no funds!\n";
                                        break;
                                    }
                                }
                            }
                            if (charging_success) {
                                the_subscription[index_of_user].plan_type = -1;
                                the_subscription[index_of_user].zones = -1;
                                the_subscription[index_of_user].remaining_trips = 0;
                                the_subscription[index_of_user].activation_day = { 0, 0, 0 };
                                the_subscription[index_of_user].expiry_day = { 0, 0, 0 };
                                Setcolor(yellow);
                                cout << "Subscription set successfully to Smart Wallet with ";
                                Setcolor(white);
                                cout << the_subscription[index_of_user].wallet_balance;
                                Setcolor(yellow);
                                cout << " LE!\n";
                            }
                        }
                        else {
                            the_subscription[index_of_user].subscription_type = -1;
                            continue;
                        }
                    }
                    else if (commuter_choice == 2)
                    {
                        Setcolor(white);
                        cout << "\t\tManage Subscription\n";
                        cout << "\t\t__\n\n";
                        cout << "1. View Subscription\n2. Renew Subscription OR Charge your wallet\n3. Modify Subscription\n4. Back\n";
                        cout << "Enter choice: ";
                        int manage_choice = validation(1, 4);
                        if (manage_choice == 1) {
                            view_subscription(index_of_user);
                        }
                        else if (manage_choice == 2) {
                            renew_subscription(index_of_user);
                        }
                        else if (manage_choice == 3) {
                            Setcolor(yellow);
                            cout << "Current subscription details:\n\n";
                            view_subscription(index_of_user);
                            cout << "\n\n";
                            Setcolor(purple);
                            cout << "Note: the old subscription will be cancelled\n";
                            upgrade_subscription(index_of_user);
                        }
                        else {
                            continue;
                        }
                    }
                    // Ride Managment functions calling:-
                    else if (commuter_choice == 3)
                    {
                        if (rides_counter >= max_rides) {
                            Setcolor(purple);
                            cout << "Maximum ride limit reached!" << endl;
                            continue;
                        }
                        else if (!is_subscription_active) {
                            Setcolor(purple);
                            cout << "Your subscription has expired. Please renew it.\n";
                            continue;
                        }
                        else if (the_subscription[index_of_user].subscription_type == -1) {
                            Setcolor(purple);
                            cout << "No active subscription! Please buy a travel plan." << endl;
                            continue;
                        }
                        else if (the_subscription[index_of_user].subscription_type == general || the_subscription[index_of_user].subscription_type == scholar) {
                            if (the_subscription[index_of_user].remaining_trips <= 0) {
                                Setcolor(purple);
                                cout << "You don't have enough trips to book this trip." << endl;
                                continue;
                            }
                        }
                        // Viewing fare for each zone for smart wallet commuters:-
                        if (the_subscription[index_of_user].subscription_type == smart_wallet) {
                            Setcolor(white);
                            cout << "How Much Will You Pay?\n";
                            cout << "---------------\n";
                            view_fare_price();
                            Setcolor(white);
                            cout << "---------------\n";
                        }
                        // Input entry details:-
                        Setcolor(white);
                        cout << "Select entry line (1-3): ";
                        int entryLine = validation(1, 3);
                        cout << "Line " << entryLine << " stations:" << endl;
                        cout << "--------------------" << endl;
                        view_stations(entryLine);
                        cout << "Select entry station: ";
                        int entryStation = validation(1, stations_num[entryLine - 1]);
                        // Input exit details:-
                        cout << "Select exit line (1-3): ";
                        int exitLine = validation(1, 3);
                        cout << "Line " << exitLine << " stations:" << endl;
                        cout << "--------------------" << endl;
                        view_stations(exitLine);
                        cout << "Select exit station: ";
                        int exitStation = validation(1, stations_num[exitLine - 1]);
                        if (entryLine == exitLine && entryStation == exitStation) {
                            Setcolor(purple);
                            cout << "Entry and exit stations cannot be the same!" << endl;
                            continue;
                        }
                        book_a_trip(entryLine, entryStation, exitLine, exitStation, users[index_of_user].user_ID);
                    }
                    else if (commuter_choice == 4) {
                        Setcolor(white);
                        cout << "Ride History for User ID -> (" << users[index_of_user].user_ID << "):" << endl;
                        cout << "----------------------------------------" << endl;
                        view_ride_history(users[index_of_user].user_ID);
                    }
                    else if (commuter_choice == 5) {
                        UpdateProfile();
                    }
                    else if (commuter_choice == 6) {
                        signOut();
                        break;
                    }
                    else {
                        Setcolor(purple);
                        cout << "You can not buy a travel plan twice but you can upgrade it from manage plan\n";
                        cout << "\n";
                        continue;
                    }
                }
            }
            else if (index_of_user != -1 && users[index_of_user].LoggedIn && index_of_user == 0)
            {
                int choice;
                while (true) {
                    Setcolor(white);
                    cout << endl << "\t\tWelcome to the Admin Page:" << endl;
                    cout << "\t\t---\n\n";
                    cout << "1. Edit Commuter." << endl << "2. Edit Subscription." << endl << "3. View rides of all users." << endl
                        << "4. Station Management." << endl << "5. Edit metro fare." << endl << "6. Edit subscription prices."
                        << endl << "7. Subscribtions statistics." << endl << "8. Sign Out.\n";
                    cout << "Enter choice: ";
                    choice = validation(1, 8);
                    if (choice == 1)
                        Edit_Commuter();
                    else if (choice == 2)
                        edit_subscription();
                    else if (choice == 3)
                        viewRides();
                    else if (choice == 4)
                        StationManagement();
                    else if (choice == 5)
                        EditFare();
                    else if (choice == 6)
                        EditSubscriptionPrice();
                    else if (choice == 7)
                        statistics();
                    else if (choice == 8) {
                        signOut();
                        break;
                    }
                }
            }
        }
        else {

            writing_process();
            exit_program = true;
        }
    }
    cout << "____\n\n";
    cout << "Thanks for using CairoRide lines.\n\n";
    return 0;
}