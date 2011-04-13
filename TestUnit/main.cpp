#include <iostream>
#include "../OUE/String.h"
#include "../OUE/List.h"
#include "../OUE/Table.h"
#include "../Base/Logger.h"
#include "../Base/LoggerNull.h"
#include <sstream>
#include "../Base/Internal/DXException.h"
#include <d3d9.h>

using namespace std;

int main()
{
	OneU::Allocator_build(OneU::Allocator_create);
	OneU::Logger_build(OneU::LoggerDisk_Factory);
	OneU::GetLogger().stream() << OneU::TimeInfo << 1 << 5.0f << '\n';
	{
		OneU::Table<int> table;
		while(!cin.eof()){
			string str;
			int cmd;

			cout << "operation: 1 insert 2 remove 3 search 4 list" << endl;
			cin >> cmd;
			getline(cin, str);
			if(cmd == 1){
				cout << "input key:" << endl;
				getline(cin, str);
				cout << "input value:" << endl;
				int n;
				cin >> n;
				int* p = table.insert(OneU::ANSI2Wide(str.c_str()));
				getline(cin, str);
				if(p == NULL)
					cout << "failed to insert" << endl;
				else
					*p = n;
			}
			if(cmd == 2){
				cout << "input key:" << endl;
				getline(cin, str);
				table.remove(OneU::ANSI2Wide(str.c_str()));
			}
			if(cmd == 3){
				cout << "input key:" << endl;
				getline(cin, str);
				int* p = table.find(OneU::ANSI2Wide(str.c_str()));
				if(p != NULL)
					cout << "key: " << *p << endl;
				else
					cout << "can't find the key.";
			}
			if(cmd == 4){
				cout << "list the table:" << endl;
				for(OneU::Table<int>::iterator it = table.begin(); it != table.end(); ++it){
					cout << "key:" << OneU::Wide2ANSI(it._getKey().c_str());
					cout << "     value:" << it.getValue() << endl;
				}
			}
		}
	}
	system("pause");
	OneU::Logger_destroy();
	OneU::Allocator_destroy();
	return 0;
}