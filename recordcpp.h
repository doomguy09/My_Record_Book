
#ifndef __MYDATABASE_2020__
#define __MYDATABASE_2020__
#include <iostream>

struct record {
	int number;
	char surname[32];
	char name[16];
	char telnumber[32];
	char email[48];
	int day, month, year;
};

struct RecordBook {
	int count;
	record* data;
};

std::ostream& operator << (std::ostream& out, const record& st);
std::istream& operator >> (std::istream& in, record& st);

bool operator < (const record& left, const record& right);
bool compByNumber(const record& left, const record& right);
bool compBySurname(const record& left, const record& right);
bool compByDate(const record& left, const record& right);
bool compByBirthD(const record& left, const record& right);
//bool compByMarks(const record& left, const record& right);

//double marks_average(const unsigned char* marks, int n);

int addRecord(RecordBook& DB);
int findRecord_telnum(const RecordBook& DB);
int findRecord_surname(const RecordBook& DB);
int findRecord_num(const RecordBook& DB);
void search(const RecordBook& DB);
int searchmenu();
void deleteRecord(RecordBook& DB);
int deletemenu();
//void setMarks(const RecordBook& DB);

int openDB(RecordBook& DB);
void printDB(const RecordBook& DB);
void saveDB(const RecordBook& DB);
void exportDB(const RecordBook& DB);
void sortDB(RecordBook& DB);
int sortmenu();


template <typename T>
void MySort(T* M, int n, bool(*comparator)(const T&, const T&)) {
	for (int j = 0; j < n - 1; ++j) {
		int k = j;
		for (int i = k + 1; i < n; ++i) {
			if (comparator(M[i], M[k]))  // M[i] < M[k]
				k = i;
		}
		if (j < k) {
			std::swap(M[k], M[j]);
		}
	}
}
#endif
