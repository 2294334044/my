#include <iostream>
#include <string>
#define  MAXSIZE 100
using namespace std;

// 记录结构
struct Record {
    string username;
    string phone;
    string address;
};

// 散列函数1: 简单字符求和
int hashFunction1(string& key) {
    int hash = 0;
    for (char c : key) {
        hash += c;  // 将字符串中每个字符的 ASCII 值累加作为哈希值
    }
    return hash;
}

// 散列函数2: 多项式滚动散列
int hashFunction2(string& key) {
    int hash = 0;
    int p = 31;  // 质数 p，用于计算多项式滚动哈希
    int power = 1;
    for (char c : key) {
        hash = (hash + (c - 'a' + 1) * power) % 1000000007;  // 多项式滚动哈希算法
        power = (power * p) % 1000000007;
    }
    return hash;
}

// 哈希表类
class HashTable {
private:
    Record* table[MAXSIZE];  // 哈希表使用数组存储记录指针
    int (*hashFunc)(string&);  // 函数指针，指向使用的散列函数

public:
    // 构造函数，初始化哈希表和散列函数
    HashTable(int (*func)(string&)) : hashFunc(func) {
        for (int i = 0; i < MAXSIZE; ++i) {
            table[i] = NULL;  // 初始化哈希表数组，所有位置置空
        }
    }
    // 插入记录
    void insert(Record& record) {
        int hash = hashFunc(record.username) % MAXSIZE;  // 计算记录的哈希值
        while (table[hash] != NULL) {
            hash = (hash + 1) % MAXSIZE; // 线性探测解决哈希冲突，找到空闲位置
        }
        table[hash] = new Record(record);  // 在找到的空闲位置存储新记录的副本
    }

    // 搜索记录
    Record* search(string& username) {
        int hash = hashFunc(username) % MAXSIZE;  // 计算要搜索的用户名的哈希值
        int startHash = hash; // 记录起始位置，用于判断表是否已满
        while (table[hash] != NULL) {
            if (table[hash]->username == username) {
                return table[hash];  // 找到匹配的用户名，返回记录指针
            }
            cout << "冲突在" << table[hash]->username << "和" << table[hash + 1]->username;
            hash = (hash + 1) % MAXSIZE;  // 线性探测下一个位置
            if (hash == startHash) break;  // 如果回到起始位置，表已满，退出查找
        }
        return NULL;  // 未找到记录，返回空指针
    }

    // 计算冲突数量
 int countCollisions() {
        int collisions = 0;
        for (int i = 0; i < MAXSIZE; ++i) {
            if (table[i] != NULL) {
                int hash = hashFunc(table[i]->username) % MAXSIZE;  // 计算记录的哈希值
                if (hash != i) {
                    collisions++;  // 如果哈希值与位置不匹配，发生冲突
                }
            }
        }
        return collisions;  // 返回冲突数量
    }
};

// 添加记录的函数
void addRecords(HashTable& phonebook, float n) {

    for (int i = 0; i < n; ++i) {
        Record record;
        cout << "输入用户名: ";
        cin >> record.username;
        cout << "输入电话: ";
        cin >> record.phone;
        cout << "输入地址: ";
        cin >> record.address;
        phonebook.insert(record);  // 调用哈希表的插入方法添加记录
    }
}
// 查找记录的函数
void searchRecord(HashTable& phonebook) {
    string username;
    cout << "输入要查找的用户名: ";
    cin >> username;
    Record* record = phonebook.search(username);  // 调用哈希表的搜索方法查找记录
    if (record) {
        cout << "用户名: " << record->username << endl;
        cout << "电话: " << record->phone << endl;
        cout << "地址: " << record->address << endl;
    }
    else {
        cout << "未找到记录." << endl;
    }
}
int main() {
    // 创建两个使用不同散列函数的哈希表
    HashTable phonebook1(hashFunction1);
    HashTable phonebook2(hashFunction2);
    float n1, n2;
    // 使用散列函数1添加记录
    cout << "使用散列函数1添加记录..." << endl;
    cout << "输入记录数量: ";
    cin >> n1;
    if (n1 == 0) {
        cout << "散列表1冲突率：0%" << endl;
    }
    else {
        addRecords(phonebook1, n1);
        cout << "散列函数1冲突率： " << (phonebook1.countCollisions() / n1) * 100 << "%" << endl;
    }
    // 使用散列函数2添加记录
    cout << "使用散列函数2添加记录..." << endl;
    cout << "输入记录数量: ";
    cin >> n2;
    if (n2 == 0) {
        cout << "散列表2冲突率：0%" << endl;
    }
    else {
        addRecords(phonebook2, n2);
        cout << "散列函数2冲突率： " << (phonebook2.countCollisions() / n2) * 100 << "%" << endl;
    }
    int choice;
    // 用户选择要使用的散列函数来查找记录
    cout << "选择要使用的散列函数查找记录 (1 或 2): ";
    cin >> choice;
    if (choice == 1) {
        searchRecord(phonebook1);  // 使用散列函数1进行记录查找
    }
    else if (choice == 2) {
        searchRecord(phonebook2);  // 使用散列函数2进行记录查找
    }
    else {
        cout << "无效选择." << endl;  // 输入无效选项
    }
    return 0;
}
