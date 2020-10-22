/*
 * UE18CS208B - Programming With C++
 *
 * Project: Phonebook With Efficient Searching
 *
 * Team Members:
 *      Satyam Priyam - PES2201800595
 *      Ruchira R Vadiraj - PES2201800602
 *      Roshan Daivajna - PES2201800372
 *      Naveen K Murthy - PES2201800051
 */

// NOTE: Compile using:  g++ -std=c++11 main.cpp definitions.cpp

#include <iostream>
#include <sys/stat.h>
#include <string.h>
#include <time.h>

#include "headers.hpp"

using namespace std;

int main() {
    srand(time(NULL));

    Phonebook p;
    // p.deleteFile();

    int option = -1;

    while (option != 8) {
        cout << "\nEnter option:\n"
             << "1. Fill phonebook with random entries\n"
             << "2. Add a particular number\n"
             << "3. Search for an entry\n"
             << "4. View complete phonebook\n"
             << "5. Get count of records\n"
             << "6. Delete a record\n"
             << "7. Delete phonebook\n"
             << "8. Exit\n";
        cin >> option;
        cout << "\n";
        switch (option) {
            case 1: {
                long long int entries;
                cout << "How many random entries do you want to add?\n";
                cin >> entries;
                for (long long int i = 0; i < entries; i++)
                    p.createRandomRecord();
                cout << "Successfully added " << entries << " entries!\n";
                break;
            }

            case 2: {
                p.createCustomRecord();
                break;
            }

            case 3: {
                struct stat buffer;
                if (stat(p.p_fileName.c_str(), &buffer) == 0) {
                    long long int target;
                    cout << "Enter target:";
                    cin >> target;
                    cout << "\n";

                    int found = p.searchForRecord(target);
                    if (found != -1) {
                        cout << "Found the above record at position " << found + 1 << "\n";
                    } else {
                        cout << "Number not in phonebook\n";
                    }
                } else {
                    // If file doesn't exist
                    cout << "ERROR: File doesn't exist yet.\n";
                }
                break;
            }

            case 4: {
                p.readAllRecordsFromFile();
                break;
            }

            case 5: {
                int count = p.getCountOfRecords();
                if (count != -1) {
                    cout << count << " records exist in the phonebook.\n";
                }
                break;
            }

            case 6: {
                struct stat buffer;
                if (stat(p.p_fileName.c_str(), &buffer) == 0) {
                    long long int target;
                    cout << "Enter value to delete:";
                    cin >> target;
                    cout << "\n";

                    int found = p.deleteRecord(target);
                    if (found == 1) {
                        cout << "Successfully deleted the above record!\n";
                    } else if (found == 0) {
                        cout << "ERROR: Record not found.\n";
                    } else if (found == -1) {
                        cout << "No records exist in the phonebook\n";
                    }
                } else {
                    // If file doesn't exist
                    cout << "ERROR: File doesn't exist yet.\n";
                }
                break;
            }

            case 7: {
                p.deleteFile();
                break;
            }

            case 8: {
                exit(0);
            }

            default: {
                cout << "ERROR: Invalid option! Try again.\n";
            }
        }
    }
}