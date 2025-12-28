#include<iostream>
#include<fstream>
#include<cstdlib>
#include<ctime>
#include<random>
#include<string>
#include<vector>
using namespace std;

class account {
protected:
	struct user {
		long int acc_num;
		string name, phone, password;
		long long int balance = 0 ;

	} usr;
	vector<user> users;
	user tem;
public:
	void create_acc() {
		cin.ignore();
		cout << "Enter your Name: " << endl;
		getline(cin, usr.name);
		cout << "Enter your Mobile Number: " << endl;
		getline(cin, usr.phone);

		cout << "Create a new password: " << endl;
		getline(cin, usr.password);

		srand(time(0));
		usr.acc_num = (rand() + rand() + rand() + rand() + 10000000);
		cout << "Your Account Number is " << usr.acc_num << endl;
		ofstream file("Bank.txt", ios::app);
		if (file.is_open()) {
			file << usr.name << endl << usr.phone << endl << usr.password
					<< endl << usr.acc_num << endl << usr.balance << endl;
			cout << "Account Created Successfully!" << endl;
			file.close();
		} else {
			cout << "Error Occurs!" << endl;
		}

	}

	void depo_sit() {
		int deposit;
		bool updated = false;
		cout << "Enter the amount to deposit: $";
		cin >> deposit;

		ifstream file("Bank.txt");
		ofstream temp("temp.txt");
		user temp_user;

		if (file.is_open() && temp.is_open()) {
			while (getline(file, temp_user.name)) {
				getline(file, temp_user.phone);
				getline(file, temp_user.password);
				file >> temp_user.acc_num >> temp_user.balance;
				file.ignore();

				if (temp_user.phone == usr.phone
						&& temp_user.password == usr.password) {
					temp_user.balance += deposit;
					usr.balance = temp_user.balance;
					updated = true;
					cout << "Deposit successful. New balance: $" << usr.balance
							<< endl;
				}
				temp << temp_user.name << endl << temp_user.phone << endl
						<< temp_user.password << endl << temp_user.acc_num
						<< endl << temp_user.balance << endl;
			}
			file.close();
			temp.close();

			remove("Bank.txt");
			rename("temp.txt", "Bank.txt");
		} else {
			cout << "Error Occurs!" << endl;
		}

		if (!updated) {
			cout << "Deposit failed: user not found!" << endl;
		}
	}

	void with_drawal() {
		int withdrawal;
		bool update = false;
		cout << "Enter the amount to Withdraw: $";
		cin >> withdrawal;
		ifstream file("Bank.txt");
		ofstream temp("temp.txt");
		user temp_user;
		if (usr.balance > withdrawal) {

			if (file.is_open() && temp.is_open()) {
				while (getline(file, temp_user.name)) {
					getline(file, temp_user.phone);
					getline(file, temp_user.password);
					file >> temp_user.acc_num >> temp_user.balance;
					file.ignore();

					if (temp_user.phone == usr.phone
							&& temp_user.password == usr.password) {
						temp_user.balance -= withdrawal;
						usr.balance = temp_user.balance;
						update = true;
						cout << "Withdrawal successful, Your new balance is $"
								<< usr.balance << endl;
					}
					temp << temp_user.name << endl << temp_user.phone << endl
							<< temp_user.password << endl << temp_user.acc_num
							<< endl << temp_user.balance << endl;
				}
				file.close();
				temp.close();

				remove("Bank.txt");
				rename("temp.txt", "Bank.txt");
			} else {
				cout << "Error Occurs!" << endl;
			}
		} else {
			cout << "Insufficient Balance!" << endl;
		}

		if (!update) {
			cout << "Withdrawal failed, user not found!" << endl;
		}
	}

};

class login: public account {
private:
	string phone, password;

public:

	void log_in() {
		int choice1;
		bool found = false;

		cout << "Enter your Mobile number: ";
		cin >> phone;
		cout << "Enter your Password: ";
		cin >> password;

		ifstream file("Bank.txt");
		if (file.is_open()) {
			while (getline(file, usr.name)) {
				getline(file, usr.phone);
				getline(file, usr.password);
				file >> usr.acc_num >> usr.balance;
				file.ignore();

				if (usr.phone == phone && usr.password == password) {
					found = true;
					cout << "Login Successfully!" << endl;
					break;
				}
			}
			file.close();
		}

		if (found) {
			do {
				cout << "\nWelcome " << usr.name << "!" << endl
						<< "1. View Balance\n2.Deposit\n3.Withdrawal\n4.Transfer\n5.Exit\n \nEnter your choice: ";
				cin >> choice1;
				switch (choice1) {
				case 1:
					cout << "Your current Balance is $" << usr.balance << endl;
					break;
				case 2:
					depo_sit();
					break;
				case 3:
					with_drawal();
					break;
				case 4:
					transfer();
					break;
				case 5:
					cout << "Exiting..." << endl;
					break;
				default:
					cout << "Invalid choice!" << endl;
				}
			} while (choice1 != 5);
		} else {
			cout << "Account not found or incorrect credentials!" << endl;
		}
	}

	void transfer() {
		string number;
		int amount;
		bool send = false;
		bool receive = false;
		cin.ignore();
		cout << "Enter the Receiver number: " << endl;
		cin >> number;
		cout << "Enter the Amount to Transfer: " << endl;
		cin >> amount;
		ifstream file("Bank.txt", ios::in);
		ofstream temp("temp.txt", ios::out);
		user temp_user, send_user, receive_user;

		if (file.is_open() && temp.is_open()) {
			while (getline(file, temp_user.name)) {
				getline(file, temp_user.phone);
				getline(file, temp_user.password);
				file >> temp_user.acc_num >> temp_user.balance;
				file.ignore();
				if (temp_user.phone == usr.phone) {
					send_user = temp_user;
					send = true;
				}
				if (temp_user.phone == number) {
					receive_user = temp_user;
					receive = true;
				}
			}
			file.close();
			temp.close();
		}
		if (!send || !receive) {
			cout << "Error Occurs, Account not found!" << endl;
			return;
		}
		if (send_user.balance < amount) {
			cout << "Insufficient Balance!" << endl;
			return;
		}

		send_user.balance -= amount;
		receive_user.balance += amount;

		file.open("Bank.txt");
		temp.open("temp.txt");

		while (getline(file, temp_user.name)) {
			getline(file, temp_user.phone);
			getline(file, temp_user.password);
			file >> temp_user.acc_num >> temp_user.balance;
			file.ignore();

			if (temp_user.phone == usr.phone
					&& temp_user.password == usr.password) {
				temp_user = send_user;
				send = true;

			} else if (temp_user.phone == number) {
				temp_user = receive_user;
				receive = true;
			}

			temp << temp_user.name << endl << temp_user.phone << endl
					<< temp_user.password << endl << temp_user.acc_num << endl
					<< temp_user.balance << endl;
		}
		file.close();
		temp.close();

		remove("Bank.txt");
		rename("temp.txt", "Bank.txt");
		cout << "Transfer Successfull!" << endl;
	}

};

class admin_user: public account {

public:
	string input_id, input_pass, admin1, pass1, user_id, user_pass;

	int choice2;

public:

	void admin() {
		bool found = false;
		string first_line;
		//bool login = false;
		ifstream admin("Admin.txt", ios::in);
		if (!getline(admin, first_line) || first_line.empty()) {
			ofstream admin("Admin.txt", ios::out);
			cout << "Create a new User ID: " << endl;
			cin >> user_id;
			cout << "Create a new password: " << endl;
			cin >> user_pass;
			if (admin) {
				admin << user_id << endl << user_pass << endl;
				found = true;
			}
			if (found) {
				cout << "User ID created Successfully!" << endl;
			}

			admin.close();
		} else {

			cout << "Enter the Admin user ID: " << endl;
			cin.ignore();
			getline(cin, input_id);
			cout << "Enter the password: " << endl;
			cin >> input_pass;
			ifstream admin("Admin.txt", ios::in);
			getline(admin, user_id);
			getline(admin, user_pass);
			admin.close();
			cout << "ID: " << user_id << endl;
			cout << "Password: " << user_pass << endl;
			cout << "ID: " << input_id << endl;
			cout << "password: " << input_pass << endl;

			if (input_id == user_id && input_pass == user_pass) {
				cout << "Access Granted!" << endl;
				//login = true;
				do {
					cout
							<< "1.View all Users\n2.change user ID and password\n3.Close\n4.Exit\n"
							<< endl;
					cout << "Enter the choice: " << endl;
					cin >> choice2;

					switch (choice2) {

					case 1:
						view_user();
						break;
					case 2:
						change_admin();
						break;
					case 3:
						close();
						break;
					case 4:
						cout << "Exiting.." << endl;
						break;
					default:
						cout << "Invalid Choice!" << endl;
						break;
					}
				} while (choice2 != 4);
			} else {
				cout << "Invalid credentials!" << endl;
			}
		}

	}

	void change_admin() {
		string user, user1, pass, pass1;
		bool pas = false;
		cout << "Enter your old User ID: " << endl;
		cin >> user1;
		cout << "Enter your old password: " << endl;
		cin >> pass1;
		ofstream admin("Admin.txt", ios::out);
		//admin >> input_id >> input_pass;
		if (user1 == input_id && pass1 == input_pass) {
			pas = true;
			cout << "Enter the New ID: " << endl;
			cin >> user;
			cout << "Enter the new password: " << endl;
			cin >> pass;
			admin << user << endl << pass << endl;
			admin.close();
			cout << "user ID, password changed successfully!" << endl;
		}
		if (!pas) {
			cout << "Invalid Password!" << endl;
		}

	}
	void view_user() {
		ifstream file("Bank.txt", ios::in);

		if (file.is_open()) {
			while (getline(file, usr.name) && getline(file, usr.phone)
					&& getline(file, usr.password) && file >> usr.acc_num
					&& file >> usr.balance) {
				file.ignore();
				cout << "Name: " << usr.name << endl;
				cout << "Phone: " << usr.phone << endl;
				cout << "Account Number: " << usr.acc_num << endl;
				cout << "Balance: " << usr.balance << endl;
			}
			file.close();

		}
	}
	void close() {
		ofstream file("Bank.txt", ios::out);
		file.close();

	}

};

int main() {

	int choice;
	account obj1;
	login obj2;
	admin_user obj3;
	do {
		cout << "Welcome to BS Bank!" << endl;
		cout << "1.Create a New Account" << endl
				<< "2.Login in to an Existing Account" << endl << "3.Admin"
				<< endl << "4.Exit" << endl;
		cout << "Enter your Choice: " << endl;
		cin >> choice;

		switch (choice) {

		case 1:
			obj1.create_acc();
			break;
		case 2:
			obj2.log_in();
			break;
		case 3:
			obj3.admin();
			break;
		case 4:
			cout << "Thank you for using our bank!" << endl;
			break;
		default:
			cout << "Invalid choice!" << endl;
			break;
		}
	} while (choice != 4);

	return 0;
}
