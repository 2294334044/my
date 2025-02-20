#include <iostream>
#include <string>
#define  MAXSIZE 100
using namespace std;

// ��¼�ṹ
struct Record {
    string username;
    string phone;
    string address;
};

// ɢ�к���1: ���ַ����
int hashFunction1(string& key) {
    int hash = 0;
    for (char c : key) {
        hash += c;  // ���ַ�����ÿ���ַ��� ASCII ֵ�ۼ���Ϊ��ϣֵ
    }
    return hash;
}

// ɢ�к���2: ����ʽ����ɢ��
int hashFunction2(string& key) {
    int hash = 0;
    int p = 31;  // ���� p�����ڼ������ʽ������ϣ
    int power = 1;
    for (char c : key) {
        hash = (hash + (c - 'a' + 1) * power) % 1000000007;  // ����ʽ������ϣ�㷨
        power = (power * p) % 1000000007;
    }
    return hash;
}

// ��ϣ����
class HashTable {
private:
    Record* table[MAXSIZE];  // ��ϣ��ʹ������洢��¼ָ��
    int (*hashFunc)(string&);  // ����ָ�룬ָ��ʹ�õ�ɢ�к���

public:
    // ���캯������ʼ����ϣ���ɢ�к���
    HashTable(int (*func)(string&)) : hashFunc(func) {
        for (int i = 0; i < MAXSIZE; ++i) {
            table[i] = NULL;  // ��ʼ����ϣ�����飬����λ���ÿ�
        }
    }
    // �����¼
    void insert(Record& record) {
        int hash = hashFunc(record.username) % MAXSIZE;  // �����¼�Ĺ�ϣֵ
        while (table[hash] != NULL) {
            hash = (hash + 1) % MAXSIZE; // ����̽������ϣ��ͻ���ҵ�����λ��
        }
        table[hash] = new Record(record);  // ���ҵ��Ŀ���λ�ô洢�¼�¼�ĸ���
    }

    // ������¼
    Record* search(string& username) {
        int hash = hashFunc(username) % MAXSIZE;  // ����Ҫ�������û����Ĺ�ϣֵ
        int startHash = hash; // ��¼��ʼλ�ã������жϱ��Ƿ�����
        while (table[hash] != NULL) {
            if (table[hash]->username == username) {
                return table[hash];  // �ҵ�ƥ����û��������ؼ�¼ָ��
            }
            cout << "��ͻ��" << table[hash]->username << "��" << table[hash + 1]->username;
            hash = (hash + 1) % MAXSIZE;  // ����̽����һ��λ��
            if (hash == startHash) break;  // ����ص���ʼλ�ã����������˳�����
        }
        return NULL;  // δ�ҵ���¼�����ؿ�ָ��
    }

    // �����ͻ����
 int countCollisions() {
        int collisions = 0;
        for (int i = 0; i < MAXSIZE; ++i) {
            if (table[i] != NULL) {
                int hash = hashFunc(table[i]->username) % MAXSIZE;  // �����¼�Ĺ�ϣֵ
                if (hash != i) {
                    collisions++;  // �����ϣֵ��λ�ò�ƥ�䣬������ͻ
                }
            }
        }
        return collisions;  // ���س�ͻ����
    }
};

// ��Ӽ�¼�ĺ���
void addRecords(HashTable& phonebook, float n) {

    for (int i = 0; i < n; ++i) {
        Record record;
        cout << "�����û���: ";
        cin >> record.username;
        cout << "����绰: ";
        cin >> record.phone;
        cout << "�����ַ: ";
        cin >> record.address;
        phonebook.insert(record);  // ���ù�ϣ��Ĳ��뷽����Ӽ�¼
    }
}
// ���Ҽ�¼�ĺ���
void searchRecord(HashTable& phonebook) {
    string username;
    cout << "����Ҫ���ҵ��û���: ";
    cin >> username;
    Record* record = phonebook.search(username);  // ���ù�ϣ��������������Ҽ�¼
    if (record) {
        cout << "�û���: " << record->username << endl;
        cout << "�绰: " << record->phone << endl;
        cout << "��ַ: " << record->address << endl;
    }
    else {
        cout << "δ�ҵ���¼." << endl;
    }
}
int main() {
    // ��������ʹ�ò�ͬɢ�к����Ĺ�ϣ��
    HashTable phonebook1(hashFunction1);
    HashTable phonebook2(hashFunction2);
    float n1, n2;
    // ʹ��ɢ�к���1��Ӽ�¼
    cout << "ʹ��ɢ�к���1��Ӽ�¼..." << endl;
    cout << "�����¼����: ";
    cin >> n1;
    if (n1 == 0) {
        cout << "ɢ�б�1��ͻ�ʣ�0%" << endl;
    }
    else {
        addRecords(phonebook1, n1);
        cout << "ɢ�к���1��ͻ�ʣ� " << (phonebook1.countCollisions() / n1) * 100 << "%" << endl;
    }
    // ʹ��ɢ�к���2��Ӽ�¼
    cout << "ʹ��ɢ�к���2��Ӽ�¼..." << endl;
    cout << "�����¼����: ";
    cin >> n2;
    if (n2 == 0) {
        cout << "ɢ�б�2��ͻ�ʣ�0%" << endl;
    }
    else {
        addRecords(phonebook2, n2);
        cout << "ɢ�к���2��ͻ�ʣ� " << (phonebook2.countCollisions() / n2) * 100 << "%" << endl;
    }
    int choice;
    // �û�ѡ��Ҫʹ�õ�ɢ�к��������Ҽ�¼
    cout << "ѡ��Ҫʹ�õ�ɢ�к������Ҽ�¼ (1 �� 2): ";
    cin >> choice;
    if (choice == 1) {
        searchRecord(phonebook1);  // ʹ��ɢ�к���1���м�¼����
    }
    else if (choice == 2) {
        searchRecord(phonebook2);  // ʹ��ɢ�к���2���м�¼����
    }
    else {
        cout << "��Чѡ��." << endl;  // ������Чѡ��
    }
    return 0;
}
