
#include "recordcpp.h"

int menu() {
	for (int i = 0; i < 20; ++i)
		std::cout << '*';
	std::cout << std::endl;
	std::cout << "1. Open database" << std::endl;
	std::cout << "2. Print database" << std::endl;
	std::cout << "3. Save database" << std::endl;
	std::cout << "4. Export database" << std::endl;
	std::cout << "5. Add a record" << std::endl;
	std::cout << "6. Sorting" << std::endl;
	std::cout << "7. Delete a record" << std::endl;
	std::cout << "8. Search" << std::endl;
	std::cout << "0. Exit" << std::endl;
	int tmp;
	std::cin >> tmp;
	std::cin.ignore();
	return tmp;
}

int main()
{
	RecordBook DB;
	DB.count = 0;
	DB.data = nullptr;
	int action;
	do {
		action = menu();
		switch (action) {
		case 1: openDB(DB); break;
		case 2: printDB(DB); break;
		case 3: saveDB(DB); break;
		case 4: exportDB(DB); break;
		case 5: addRecord(DB); break;
		case 6: sortDB(DB); break;
		case 7: deleteRecord(DB); break;
		case 8: search(DB); break;
		}
	} while (action != 0);

	// free memory
	if (DB.data)
		delete[] DB.data;

	return 0;
}