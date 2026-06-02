#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<iomanip>
using namespace std;

class Book {
private:
	string id;
	string name;
	string author;
	string category;
	bool borrowed;

public:
	Book() :borrowed(false) {}

	Book(string id, string name, string author, string category, bool borrowed = false) {
		this->id = id;
		this->name = name;
		this->author = author;
		this->category = category;
		this->borrowed = borrowed;
	}
	string getId()const { return id; }
	string getName()const { return name; }
	string getAuthor() const { return author; }
	string getCategory()const { return category; }
	bool isBorrowed()const { return borrowed; }

	void setName(string name) { this->name = name; }
	void setAuthor(string author) { this->author = author; }
	void setCategory(string category) { this->category = category; }
	void setBorrowed(bool borrowed) { this->borrowed = borrowed; }

	void display()const {
		cout << left << setw(12) << id << setw(20) << name << setw(16) << author << setw(14) << category << setw(10) << (borrowed ? "已借出" : "可借阅") << endl;
	}
};

class LibrarySystem{
private:
	vector<Book>books;
	const string filename = "books.txt";

public:
	LibrarySystem() {
		loadFromFile();
	}

	~LibrarySystem()
	{
		saveToFile();
	}

	void mainMenu() {
		int choice;
		do {
			cout << "\n==========图书管理系统==========" << endl;
			cout << "1.添加图书" << endl;
			cout << "2.显示所有图书" << endl;
			cout << "3.查询图书" << endl;
			cout << "4.删除图书" << endl;
			cout << "5.修改图书信息" << endl;
			cout << "6.借书" << endl;
			cout << "7.还书" << endl;
			cout << "0,退出系统" << endl;
			cout << "请选择功能：";
			cin>>choice;

			switch (choice) {
			case 1:
				addBook();
				break;
			case 2:
				showAllBooks();
				break;
			case 3:
				searchBook();
				break;
			case 4:
				deleteBook();
				break;
			case 5:
				modifyBook();
				break;
			case 6:
				borrowBook();
				break;
			case 7:
				returnBook();
				break;
			case 0:
				cout << "正在保存数据，感谢使用！" << endl;
				saveToFile();
				break;
			default:
				cout << "输入错误，请重新选择。" << endl;
			}
		} while (choice != 0);
	}

	void addBook() {
		string id, name, author, category;

		cout << "请输入图书编号：";
		cin >> id;

		if (findBookIndexById(id) != -1) {
			cout << "该编号已存在，添加失败。" << endl;
			return;
		}
		cout << "请输入图书名称：";
		cin >> name;
		cout << "请输入作者：";
		cin >> author;
		cout << "请输入类别:";
		cin >> category;

		books.push_back(Book(id, name, author, category));
		cout << "图书添加成功。" << endl;
		return;
	}

	void showAllBooks() const {
		if (books.empty()) {
			cout << "暂无图书信息。" << endl;
			return;
		}

	printHeader();
	for(const Book&book:books){
		book.display();
	}
}

void searchBook()const {
	string keyword;
	bool found = false;

	cout << "请输入要查询的图书编号、书名或作者:";
	cin >> keyword;

	printHeader();
	for (const Book& book : books) {
		if (book.getId() == keyword || book.getName() == keyword || book.getAuthor() == keyword) {
			book.display();
			found = true;
		}
	}
	if (!found) {
		cout << "没有找到相关图书。" << endl;
	}
}
void deleteBook() {
	string id;
	cout << "请输入要删除的图书编号：";
	cin >> id;

	int index = findBookIndexById(id);
	if (index == -1) {
		cout << "未找到该图书，删除失败。" << endl;
		return;
	}

	books.erase(books.begin() + index);
	cout << "图书删除成功。" << endl;
}

void modifyBook() {
	string id;
	cout << "请输入要修改的图书编号:";
	cin >> id;

		int index = findBookIndexById(id);
	if (index == -1) {
		cout << "未找到该图书，修改失败。" << endl;
		return;
	}

	string newName, newAuthor, newCategory;
	cout << "请输入新的图书名称:";
	cin >> newName;
	cout << "请输入新的作者:";
	cin >> newAuthor;
	cout << "请输入新的类别:";
	cin >> newCategory;

	books[index].setName(newName);
	books[index].setAuthor(newAuthor);
	books[index].setCategory(newCategory);

	cout << "图书信息修改成功。" << endl;
}

void borrowBook() {
	string id;
	cout << "请输入要借阅的图书编号:";
	cin >> id;

	int index = findBookIndexById(id);
	if (index == -1) {
		cout << "未找到该图书。" << endl;
		return;
	}

	if (books[index].isBorrowed()) {
		cout << "该图书已经被借出，无法借阅。" << endl;
		return;
	}

	books[index].setBorrowed(true);
	cout << "借书成功。" << endl;
}

void returnBook() {
	string id;
	cout << "请输入要归还的图书编号：";
	cin >> id;

	int index = findBookIndexById(id);
	if (index == -1) {
		cout << "未找到该图书。" << endl;
		return;
	}

	if (!books[index].isBorrowed()) {
		cout << "该图书目前未被借出，无需归还。" << endl;
		return;
	}

	books[index].setBorrowed(false);
	cout << "还书成功。" << endl;
}

void saveToFile() const {
	ofstream fout(filename);
	if (!fout) {
		cout << "文件保存失败。" << endl;
		return;
	}

	for (const Book& book : books) {
		fout << book.getId() << " " << book.getName() << " " << book.getAuthor() << " " << book.getCategory() << " " << book.isBorrowed() << endl;
	}

	fout.close();
}

void loadFromFile() {
	ifstream fin(filename);
	if (!fin) {
		return;
	}

	string id, name, author, category;
	bool borrowed;

	while (fin >> id >> name >> author >> category >> borrowed) {
		books.push_back(Book(id, name, author, category, borrowed));
	}

	fin.close();
}

private:
	int findBookIndexById(const string & id)const {
		for (int i = 0; i < static_cast<int>(books.size()); i++) {
			if (books[i].getId() == id) {
				return i;
			}
		}
		return -1;
	}

	void printHeader()const {
		cout << left << setw(12) << "编号" << setw(20) << "书名" << setw(16) <<"作者" << setw(14) << "类别" << setw(10) << "状态" << endl;
			cout << string(72, '-') << endl;
	}
};

int main() {
	LibrarySystem system;
	system.mainMenu();
	return 0;
}

