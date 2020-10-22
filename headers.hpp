#include <iostream>
#include <string.h>

using namespace std;

string getRandomName();
long long int getRandomNumber();

class Record;

class Phonebook {
   private:
    static long long int p_numberOfRecords;

   public:
    string p_fileName;
    Phonebook();
    ~Phonebook();
    static void incrementNumberOfRecords();
    static void decrementNumberOfRecords();
    void createRandomRecord();
    void createCustomRecord();
    void createCustomRecord(long long int, string);
    int writeRecordToFile(Record);
    void readAllRecordsFromFile();
    long long int searchForRecord(long long int);
    long long int getCountOfRecords();
    int deleteRecord(long long int);
    void deleteFile();
};

class Record {
   private:
    long long int r_number;
    string r_name;

   public:
    Record();
    Record(long long int, string);
    string getName();
    long long int getPhoneNumber();
    void setPhoneNumber(long long int);
    void printRecord();
    void friend Phonebook::incrementNumberOfRecords();
};
