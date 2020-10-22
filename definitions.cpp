#include <sys/stat.h>
#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>

#include "headers.hpp"
#include "TextTable.h"

using namespace std;

/***************************** Utility Function Definitions *****************************/

string getRandomName() {
    char consonents[] = {'b', 'c', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm', 'n', 'p', 'q', 'r', 's', 't', 'v', 'w', 'x', 'z'};
    char vowels[] = {'a', 'e', 'i', 'o', 'u', 'y'};

    string name = "";
    int length = rand() % 10 + 1;

    int count = 0;

    for (int i = 0; i < length; i++) {
        if ((rand() % 2) < 2 && count < 2) {
            name += consonents[rand() % sizeof(consonents)];
            count++;
        } else {
            name += vowels[rand() % sizeof(vowels)];
            count = 0;
        }
    }

    return name;
}

long long int getRandomNumber() {
    string number = "";

    if ((rand() % 10) < 5) {
        number += "8";
    } else {
        number += "9";
    }

    for (int i = 0; i < 9; i++) {
        number += to_string(rand() % 10);
    }

    return stoll(number, nullptr, 10);
}

/***************************** Record Method Definitions *****************************/

Record::Record() {
    r_number = getRandomNumber();
    r_name = getRandomName();
}

Record::Record(long long int number, string name) {
    r_number = number;
    r_name = name;
}

string Record::getName() {
    return r_name;
}

long long int Record::getPhoneNumber() {
    return r_number;
}

void Record::setPhoneNumber(long long int n) {
    r_number = n;
}

void Record::printRecord() {
    cout << getName() << " " << getPhoneNumber() << endl;
}

/***************************** Phonebook Method Definitions *****************************/

long long int Phonebook::p_numberOfRecords = 0;

Phonebook::Phonebook() {
    p_fileName = "phonebook.dat";
    cout << "Created phonebook!" << endl;
}

Phonebook::~Phonebook() {
    cout << "Destroyed phonebook!" << endl;
}

void Phonebook::incrementNumberOfRecords() {
    p_numberOfRecords++;
}

void Phonebook::decrementNumberOfRecords() {
    p_numberOfRecords--;
}

void Phonebook::createRandomRecord() {
    Record r;
    writeRecordToFile(r);
}

void Phonebook::createCustomRecord() {
    long long int number;
    cout << "Enter a 10-digit phone number: ";
    cin >> number;

    string name;
    cout << "Enter name: ";
    cin >> name;

    Record r(number, name);
    if (writeRecordToFile(r) == 1) {
        cout << "Successfully wrote data to the file!" << endl
             << endl;
    }
}

void Phonebook::createCustomRecord(long long int number, string name) {
    Record r(number, name);
    if (writeRecordToFile(r) == 1) {
        cout << "Successfully wrote data to the file!" << endl
             << endl;
    }
}

/*
return 0: error in writing
       1: success
       2: duplicate value (hence not written into file)
*/
int Phonebook::writeRecordToFile(Record r) {
    struct stat buffer;
    if (stat(p_fileName.c_str(), &buffer) == 0) {
        // If file already exists
        ifstream fi(p_fileName, ios::in | ios::binary);
        ofstream fo("temp.dat", ios::out | ios::binary);

        char last = 'y';
        Record rCompare;

        while (true) {
            fi.read((char *)&rCompare, sizeof(Record));
            if (fi.eof())
                break;

            if (r.getPhoneNumber() <= rCompare.getPhoneNumber()) {
                if (r.getPhoneNumber() == rCompare.getPhoneNumber()) {
                    cout << "ERROR: Duplicate value entered." << endl;
                    r.setPhoneNumber(INT32_MAX);
                    last = 'd';  // d stands for duplicate
                    fo.write((char *)&rCompare, sizeof(Record));
                    break;
                }
                fo.write((char *)&r, sizeof(Record));
                fo.write((char *)&rCompare, sizeof(Record));
                last = 'n';
                break;
            } else {
                fo.write((char *)&rCompare, sizeof(Record));
            }
        }

        if (last == 'y') {
            fo.write((char *)&r, sizeof(Record));
        } else if (!fi.eof()) {
            while (true) {
                fi.read((char *)&rCompare, sizeof(Record));
                if (fi.eof())
                    break;
                fo.write((char *)&rCompare, sizeof(Record));
            }
        }

        fi.close();
        fo.close();

        remove(p_fileName.c_str());
        rename("temp.dat", p_fileName.c_str());

        if (last == 'd') {
            incrementNumberOfRecords();
            return 2;
        }

    } else {
        // Otherwise, create a new file and write this record
        ofstream fo(p_fileName, ios::out | ios::binary);
        fo.write((char *)&r, sizeof(Record));
        fo.close();
    }

    incrementNumberOfRecords();
    return 1;
}

void Phonebook::readAllRecordsFromFile() {
    struct stat buffer;
    if (stat(p_fileName.c_str(), &buffer) == 0) {
        ifstream fi(p_fileName, ios::in);
        Record r;

        TextTable t('-', '|', '+');
        t.add("Name");
        t.add("Phone Number");
        t.endOfRow();

        int found = 0;
        while (!fi.eof()) {
            fi.read((char *)&r, sizeof(Record));
            if (fi.eof())
                break;
            found = 1;
            t.add(r.getName());
            t.add(to_string(r.getPhoneNumber()));
            t.endOfRow();
        }
        if (found)
            cout << t;
        else {
            cout << "No records exist.\n";
        }
        fi.close();
    } else {
        // If file doesn't exist
        cout << "ERROR: File doesn't exist yet.\n";
        return;
    }
}

long long int Phonebook::searchForRecord(long long int target) {
    ifstream fin(p_fileName, ios::in | ios::binary);
    fin.seekg(0, ios::end);
    Record r;
    long long int size = fin.tellg();

    long long int low = 0;
    long long int high = size;
    long long int mid;

    while (low <= (high - sizeof(Record))) {
        if ((low + high) % 2 == 0) {
            mid = (low + high) / 2;
            if (mid % sizeof(Record) != 0) {
                mid = low;
            }
        } else {
            mid = (low + high - sizeof(Record)) / 2;
            if (mid % sizeof(Record) != 0)
                mid = high;
        }

        fin.seekg(mid);
        fin.read((char *)&r, sizeof(Record));
        if (target == r.getPhoneNumber()) {
            r.printRecord();
            fin.close();
            return (mid / sizeof(Record));
        } else if (target > r.getPhoneNumber()) {
            low = mid + sizeof(Record);
        } else {
            high = mid;
        }

        if (mid == low && mid == high) {
            fin.close();
            return -1;
        }
    }

    fin.close();
    return -1;
}

int Phonebook::deleteRecord(long long int target) {
    struct stat buffer;
    if (stat(p_fileName.c_str(), &buffer) == 0) {
        // If file already exists
        ifstream fi(p_fileName, ios::in | ios::binary);
        ofstream fo("temp.dat", ios::out | ios::binary);

        char found = 'f';
        int exists = 0;
        Record rCompare;

        while (true) {
            fi.read((char *)&rCompare, sizeof(Record));
            if (fi.eof())
                break;
            exists = 1;
            if (target == rCompare.getPhoneNumber()) {
                rCompare.printRecord();
                found = 't';
            } else {
                fo.write((char *)&rCompare, sizeof(Record));
            }
        }

        fi.close();
        fo.close();

        remove(p_fileName.c_str());
        rename("temp.dat", p_fileName.c_str());

        if (!exists) {
            return -1;
        } else if (found == 'f') {
            return 0;
        } else {
            decrementNumberOfRecords();
            return 1;
        }

    } else {
        cout << "ERROR: File doesn't exist yet.\n";
        return 0;
    }
}

long long int Phonebook::getCountOfRecords() {
    struct stat buffer;
    if (stat(p_fileName.c_str(), &buffer) == 0) {
        ifstream fin(p_fileName, ios::in | ios::binary);
        fin.seekg(0, ios::end);
        long long int count = (fin.tellg() / sizeof(Record));
        fin.close();
        return count;
    } else {
        cout << "ERROR: File doesn't exist yet.\n";
        return -1;
    }
}

void Phonebook::deleteFile() {
    struct stat buffer;
    if (stat(p_fileName.c_str(), &buffer) == 0) {
        if (!remove(p_fileName.c_str())) {
            cout << "Deleted existing file!" << endl;
        } else {
            // If file exists and we couldn't delete it
            cout << "ERROR: Unable to delete file." << endl;
        }
    } else {
        cout << "ERROR: No existing file to delete." << endl;
    }
    p_numberOfRecords = 0;
}