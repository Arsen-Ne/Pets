#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <Windows.h>
#include <regex>

using namespace std;

map <string, set<string>> ownerPets; // 1
multimap <string, pair<string, string>> pets; // 2?
map <string, pair<set<string>, set<string>>> typeOwnersNicks; // 2
unordered_map <string, unordered_set<string>> nicks; // 3
map <string, set<int>> ages; // 4

struct record {
    string owner;
    string type;
    string nick;
    int age;
};

void addToOwnerPets(record pet) {    
    auto it = ownerPets.find(pet.owner);
    if (it == ownerPets.end()) {
        set<string> tmp;
        tmp.insert(pet.type);
        ownerPets.insert(make_pair(pet.owner, tmp));
    }
    else {
        it->second.insert(pet.type);
    }    
};

void printOwnerPets() {
    cout << "<Владелец>: <количество разных видов животных>" << endl;
    for (auto it = ownerPets.begin(); it != ownerPets.end(); it++) {
        cout << it->first << ": " << it->second.size() << endl;
    }
}

void addToPets(record pet) {   
    pets.insert(make_pair(pet.type, make_pair(pet.owner, pet.nick)));    
}

void printPets(string type) {    
    cout << "<Тип>: <владелец> <кличка>" << endl;
    auto it = pets.equal_range(type);
    for (auto i = it.first; i != it.second; i++)
        cout << i->first << ": " << i->second.first << " " << i->second.second << endl;
}

void addToTypeOwnersNicks(record pet) {
    auto it = typeOwnersNicks.find(pet.type);
    if (it == typeOwnersNicks.end()) {
        set<string> tmp1;
        tmp1.insert(pet.owner);
        set<string> tmp2;
        tmp2.insert(pet.nick);
        typeOwnersNicks.insert(make_pair(pet.type, make_pair(tmp1, tmp2)));
    }
    else {
        it->second.first.insert(pet.owner);
        it->second.second.insert(pet.nick);
    }
}

void printTypeOwnersAndNicks(string type) {    
    auto it = typeOwnersNicks.find(type);
    if (it != typeOwnersNicks.end()) {
        cout << "Владельцы:";        
        for (string const& owner : it->second.first)
        {
            std::cout << " " << owner;
        }
        cout << endl << "Клички:";
        for (string const& nick : it->second.second)
        {
            std::cout << " " << nick;
        }
        cout << endl;
    }
    else {
        cout << "Такой вид не найден" << endl;
    }    
}

void addToNicks(record pet) {
    auto it = nicks.find(pet.nick);
    if (it == nicks.end()) {
        unordered_set<string> tmp;
        tmp.insert(pet.type);
        nicks.insert(make_pair(pet.nick, tmp));
    }
    else {
        it->second.insert(pet.type);
    }
}

void printNicks(string nick) {
    auto it = nicks.find(nick);
    if (it != nicks.end()) {        
        cout << it->second.size() << " вида животных носит такую кличку" << endl;
    }
    else {
        cout << "Такая кличка не найдена" << endl;
    }
}

void addToAges(record pet) {  
    auto it = ages.find(pet.type);
    if (it == ages.end()) {
        set<int> tmp;
        if (pet.age != NULL) {
            tmp.insert(pet.age);
        }
        ages.insert(make_pair(pet.type, tmp));
    }
    else {
        if (pet.age != NULL) {
            it->second.insert(pet.age);
        }
    }
}

void printTypeAges() {
    cout << "<Тип>: <наименьший возраст> - <наибольший возраст>" << endl;
    for (auto &const pair : ages) {
        // todo if unknown
        cout << pair.first << ": ";
        if (pair.second.size() > 0) {
          cout << *pair.second.begin() << " - " << *pair.second.rbegin() << endl;
        }
        else {
            cout << "неизвестен" << endl;
        }
    }
}

string trim(string &str) {
  return regex_replace(str, regex("(^[ ]+)|([ ]+$)"), "");
}


void printComand() {
    cout << "нажмите" << endl;
    cout << "0 - для выхода" << endl;
    cout << "1 - сколько разных видов животных" << endl;
    cout << "2 - вывести всех владельцев и клички введённого вида животного" << endl;
    cout << "3 - вывести сколько видов животных носит введённую кличку" << endl;
    cout << "4 - вывести самых старых и самых молодых животных всех видов" << endl;
    cout << "5 - вывести команды" << endl;
}
int main() 
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    ifstream in("input.txt");
    string line, word;
    record pet;
    
    while (!in.eof())
    {
        getline(in, line);
        stringstream ss(line);
        getline(ss, word, ',');
        word = trim(word);
        pet.owner = (!word.empty()) ? word : "бездомный";
        getline(ss, word, ',');
        word = trim(word);
        pet.type = (!word.empty()) ? word : "неизвестен";
        getline(ss, word, ',');
        word = trim(word);
        pet.nick = (!word.empty()) ? word : "безымянный";
        getline(ss, word, ',');
        word = trim(word);
        if (!word.empty()) {
            try {
                pet.age = stoi(word);
            }
            catch (const std::invalid_argument&) {
                pet.age = NULL;
            }
        }
        else pet.age = NULL;
        

        addToOwnerPets(pet);
        addToTypeOwnersNicks(pet);
        addToPets(pet);
        addToNicks(pet);
        addToAges(pet);
    }
    in.close();

    printComand();

    while (true) {
        cout << endl << "введите ключ операции: ";
        int s;
        cin >> s;
        string animal;
        string name;
        switch (s) {
        case 0:
            return 0;
        case 1:
            printOwnerPets();
            break;
        case 2:
            cout << "введите вид животного: ";
            cin >> animal;            
            printTypeOwnersAndNicks(animal);
            break;
        case 3:
            cout << "введите кличку: ";
            cin >> name;
            printNicks(name);
            break;
        case 4:
            printTypeAges();
            break;
        case 5:
            printComand();
            break;
        default:
            cout << "Неправильный ключ, введите нужный ключ: ";
            break;
        }
    }
}


