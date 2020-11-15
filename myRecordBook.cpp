#define _CRT_SECURE_NO_WARNINGS
#include "recordcpp.h"
#include <fstream>
#include <ctime>

std::ostream& operator << (std::ostream& out, const record& st) {
	out << st.number << ". " << st.surname << " " << st.name << ". ";
	out << "Tel: " << st.telnumber << "; E'mail: " << st.email;
	out << "\nBirth date: ";
	if (st.day < 10) std::cout << 0 << st.day;
	else std::cout << st.day;
	if (st.month < 10)
		std::cout << "." << 0 << st.month << "." << st.year;
	else std::cout << "." << st.month << "." << st.year;
	return out;
}

std::istream& operator >> (std::istream& in, record& st) {
	std::cout << "record number: ";
	in >> st.number;
	in.ignore();
	std::cout << "Surname: "; in.getline(st.surname, 32);
	std::cout << "Name: ";    in.getline(st.name, 16);
	std::cout << "Telephon number: "; in.getline(st.telnumber, 32);
	std::cout << "E'mail: "; in.getline(st.email, 48);
	do {
		std::cout << "Burth date - day: "; in >> st.day;
	} while (st.day < 1 || st.day > 31);
	do {
		std::cout << "Burth date - month: "; in >> st.month;
	} while (st.month < 1 || st.month > 12);
	do {
		std::cout << "Burth date - year: "; in >> st.year;
	} while (st.year < 1960);
	return in;
}

bool operator < (const record& left, const record& right) {
	return strcmp(left.surname, right.surname) < 0;
}

bool compByNumber(const record& left, const record& right) {
	return left.number < right.number;
}

bool compBySurname(const record& left, const record& right) {
	int result = strcmp(left.surname, right.surname);
	if (result < 0)
		return true;
	if (result == 0) 
		return strcmp(left.name, right.name) < 0;
	return false;
}

bool compByDate(const record& left, const record& right) {
	if (left.year != right.year) return left.year < right.year;
	else if (left.month != right.month) return left.month < right.month;
	else return left.day < right.day;
}

bool compByBirthD(const record& left, const record& right) {
	time_t now = time(0);
	tm* ltm = localtime(&now);
	int months[12] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	
	int tmp1 = left.day, tmp2 = right.day;
	
	for (int i = 0; i < left.month - 1; ++i) {
		tmp1 += months[i];
	}
	if (ltm->tm_yday < tmp1) tmp1 = ltm->tm_yday - tmp1 + 366;
	else tmp1 = ltm->tm_yday - tmp1;
	
	for (int i = 0; i < right.month - 1; ++i) {
		tmp2 += months[i];
	}
	if (ltm->tm_yday < tmp2) tmp2 = ltm->tm_yday - tmp2 + 366;
	else tmp2 = ltm->tm_yday - tmp2;
	return tmp1 > tmp2;
}

int addRecord(RecordBook& DB) {
	record* tmp = new record[DB.count + 1];
	for (int i = 0; i < DB.count; ++i) {
		tmp[i] = DB.data[i];
	}
	if (DB.data)
		delete[] DB.data;
	DB.data = tmp;

	std::cin >> DB.data[DB.count];
	++DB.count;
	return DB.count;
}

void printDB(const RecordBook& DB) {
	for (int i = 0; i < DB.count; ++i) {
		std::cout << DB.data[i] << std::endl;
		std::cout << '\n';
	}
}

int findRecord_telnum(const RecordBook& DB) {
	std::cout << "Telephone number: ";
	char tmp[32];
	std::cin.getline(tmp, 32);
	int index;
	for (index = 0; index < DB.count; ++index) {
		if (strcmp(DB.data[index].telnumber, tmp) == 0)
			return index;
	}
	std::cout << "There are no any record with this surname in Data Base" << std::endl;
	return -1;
}

int findRecord_surname(const RecordBook& DB) {
	std::cout << "Surname: ";
	char tmp[32];
	std::cin.getline(tmp, 32);
	int index;
	for (index = 0; index < DB.count; ++index) {
		if (strcmp(DB.data[index].surname, tmp) == 0)
			return index;
	}
	std::cout << "There are no any record with this surname in Data Base" << std::endl;
	return -1;
}

int findRecord_num(const RecordBook& DB) {
	std::cout << "Record number: ";
	int tmp;
	std::cin >> tmp; std::cin.ignore();
	int index;
	for (index = 0; index < DB.count; ++index) {
		if (DB.data[index].number == tmp)
			return index;
	}
	std::cout << "There are no any record with number " << tmp << " in DB" << std::endl;
	return -1;
}

void search(const RecordBook& DB) {
	int action;
	do {
		action = searchmenu();
	} while (action < 1 || action > 2);
	int index;
	switch (action) {
	case 1: index = findRecord_telnum(DB);
		std::cout << DB.data[index] << std::endl; break;
	case 2: index = findRecord_surname(DB);
		std::cout << DB.data[index] << std::endl; break;
	}
}

void saveDB(const RecordBook& DB) {
	char fname[256];
	std::cout << "Database file name [myrecords.db]: ";
	std::cin.getline(fname, 256);
	if (strlen(fname) < 1)
		strcpy_s(fname, "myrecords.db");

	std::ofstream outFile;
	outFile.open(fname, std::ios_base::binary);
	if (!outFile) {
		std::cout << "Can not open file " << fname << std::endl;
		return;
	}
	else std::cout << "Your records is saved in " << fname << std::endl;
	outFile.write((const char*)&DB.count, sizeof(int));
	outFile.write((const char*)DB.data, sizeof(record) * DB.count);
	outFile.close();
}

int openDB(RecordBook& DB) {
	char fname[256];
	std::cout << "Database file name [myrecords.db]: ";
	std::cin.getline(fname, 256);
	if (strlen(fname) < 1)
		strcpy_s(fname, "myrecords.db");
	std::ifstream inFile;
	inFile.open(fname, std::ios_base::binary);
	if (!inFile) {
		std::cout << "Can not open file " << fname << std::endl;
		return 0;
	}
	int count;
	inFile.read((char*)&count, sizeof(int));
	//count = (count > 100) ? 100 : count;
	if (DB.data)
		delete[] DB.data;
	DB.data = new record[count];
	DB.count = count;
	inFile.read((char*)DB.data, DB.count * sizeof(record));
	inFile.close();
	std::cout << "Database " << fname << " is opened. Total " << count << " records." << std::endl;
	return count;
}

void exportDB(const RecordBook& DB) {
	char fname[256];
	std::cout << "Database file name [recordsTEXT.db]: ";
	std::cin.getline(fname, 256);
	if (strlen(fname) < 1)
		strcpy_s(fname, "recordsTEXT.db");

	std::ofstream outFile;
	outFile.open(fname);
	if (!outFile) {
		std::cout << "Can not open file " << fname << std::endl;
		return;
	}
	for (int i = 0; i < DB.count; ++i) {
		outFile << DB.data[i] << std::endl;
	}
	outFile.close();
	std::cout << "Database is exported in :" << fname << '\n';
}

void sortDB(RecordBook& DB) {
	int action;
	do {
		action = sortmenu();
	} while (action < 1 || action > 4);

	switch (action) {
	case 1: MySort(DB.data, DB.count, compByNumber); break;
	case 2: MySort(DB.data, DB.count, compBySurname); break;
	case 3: MySort(DB.data, DB.count, compByDate); break;
	case 4: MySort(DB.data, DB.count, compByBirthD); break;
	}
	std::cout << "Database sorted" << std::endl;
}

void deleteRecord(RecordBook& DB) {
	int action;
	do {
		action = deletemenu();
	} while (action < 1 || action > 3);
	int index;
	char ch[128];
	switch (action) {
	case 1: index = findRecord_num(DB);
		if (index < 0) {
			return;
		}
		std::cout << "Are you sure? (Y/N) ";
		std::cin.getline(ch, 128);
		if (ch[0] == 'Y' || ch[0] == 'y') {
			for (int i = index + 1; i < DB.count; ++i) {
				DB.data[i - 1] = DB.data[i];
			}
			--DB.count;
			std::cout << "Record " << index + 1 << " is deleted\n";
		} break;
	case 2: index = findRecord_surname(DB);
		if (index < 0) {
			return;
		}
		std::cout << "Are you sure? (Y/N) ";
		std::cin.getline(ch, 128);
		if (ch[0] == 'Y' || ch[0] == 'y') {
			for (int i = index + 1; i < DB.count; ++i) {
				DB.data[i - 1] = DB.data[i];
			}
			--DB.count;
			std::cout << "Record " << index + 1 << " is deleted\n";
		} break;
	}
}

int sortmenu() {
	std::cout << "------ Sorting method ------------" << std::endl;
	std::cout << "1. By number" << std::endl;
	std::cout << "2. By surname, name" << std::endl;
	std::cout << "3. By birth date" << std::endl;
	std::cout << "4. By closest birth date" << std::endl;
	int tmp;
	std::cin >> tmp;
	std::cin.ignore();
	return tmp;
}

int deletemenu() {
	std::cout << "--------Deleting by--------" << std::endl;
	std::cout << "1. By number" << std::endl;
	std::cout << "2. By surname" << std::endl;
	int tmp;
	std::cin >> tmp;
	std::cin.ignore();
	return tmp;
}

int searchmenu() {
	std::cout << "--------Searching by--------" << std::endl;
	std::cout << "1. By telephone number" << std::endl;
	std::cout << "2. By surname" << std::endl;
	int tmp;
	do {
		std::cin >> tmp;
		std::cin.ignore();
	} while (tmp < 1 || tmp > 2);
	return tmp;
}