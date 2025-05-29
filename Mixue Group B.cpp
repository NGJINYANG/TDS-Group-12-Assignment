#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <iomanip>
using namespace std;

const int MAX_ENTRIES=50; 
const int MAX_CATEGORIES=10; 

struct Drink {
    string name;
    string category;
    int price;
    int stock;
};

struct Stack {
    Drink items[MAX_ENTRIES];
    int top;
    
    Stack() :top(-1) {}
    
    bool isEmpty() {
        return top==-1;
    }
    
    bool isFull() {
        return top==MAX_ENTRIES - 1;
    }
    
    void push(Drink drink) {
        if (!isFull()) {
            items[++top] = drink;
        }
    }
    
    Drink pop() {
        if (!isEmpty()) {
            return items[top--];
        }
        return Drink(); 
    }
    
    int size() {
        return top + 1;
    }
};

Drink drinks[MAX_ENTRIES];
string categories[MAX_CATEGORIES];
int totalEntries = 0;
int totalCategories = 0;

void clearScreen() {
    system("cls || clear"); 
}

void displayHeader(const string& title) {
    clearScreen();
    cout << "============================================\n";
    cout << "   Mixue Drink System - " << title << "\n";
    cout << "============================================\n\n";
}

void waitForEnter() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void initializeCategories() { 
    categories[totalCategories++] = "Beverage";
    categories[totalCategories++] = "Juice";
    categories[totalCategories++] = "Tea";
}

void readDataFromFile() {
    ifstream file("mixue.txt");
    if (!file.is_open()) {
        cout << "mixue.txt not found.\n";
        return;
    }

    totalEntries = 0;
    while (file >> drinks[totalEntries].name 
                >> drinks[totalEntries].category 
                >> drinks[totalEntries].price 
                >> drinks[totalEntries].stock) {
        totalEntries++;
        if (totalEntries >= MAX_ENTRIES) break;
    }
    file.close();
}

void saveDataToFile() {
    ofstream file("mixue.txt");
    if (!file.is_open()) {
        cout << "Error saving to mixue.txt\n";
        return;
    }

    for (int i = 0; i < totalEntries; i++) {
        file <<drinks[i].name<< " " 
             <<drinks[i].category<< " " 
             <<drinks[i].price<< " " 
             <<drinks[i].stock<< "\n";
    }
    file.close();
}

void saveSortedDataToFile() {
    ofstream file("sorted_information.txt");
    if (!file.is_open()) {
        cout << "Cannot save to sorted_information.txt\n";
        return;
    }
    Drink sorted[MAX_ENTRIES];

    int categoryCount[MAX_CATEGORIES] = {0};
    for (int i=0; i<totalEntries; i++) {
        for (int j=0; j<totalCategories; j++) {
            if (drinks[i].category == categories[j]) {
                categoryCount[j]++;
                break;
            }
        }
    }

    int categoryIndex[MAX_CATEGORIES] = {0};
    for (int i=1; i<totalCategories; i++) {
        categoryIndex[i] = categoryIndex[i-1] + categoryCount[i-1];
    }

    Stack categoryStacks[MAX_CATEGORIES];
    for (int i=0; i<totalEntries; i++) {
        for (int j = 0; j<totalCategories; j++) {
            if (drinks[i].category == categories[j]) {
                categoryStacks[j].push(drinks[i]);
                break;
            }
        }
    }

    Drink tempSorted[MAX_ENTRIES];
    int currentIndex = 0;
    for (int cat = 0; cat < totalCategories; cat++) {

        Drink tempCategory[MAX_ENTRIES];
        int tempCount = 0;
        while (!categoryStacks[cat].isEmpty()) {
            tempCategory[tempCount++] = categoryStacks[cat].pop();
        }

        for (int i=1; i<tempCount; i++) {
            Drink key = tempCategory[i];
            int j=i-1;
            while (j>=0 && tempCategory[j].name > key.name) {
                tempCategory[j+1] = tempCategory[j];
                j--;
            }
            tempCategory[j+1] = key;
        }

        for (int i=0; i<tempCount; i++) {
            tempSorted[currentIndex++] = tempCategory[i];
        }
    }

    for (int i = 0; i < totalEntries; i++) {
        sorted[i] = tempSorted[i];
    }

    for (int i = 0; i < totalEntries; i++) {
        file << sorted[i].name << " " 
             << sorted[i].category << " " 
             << sorted[i].price << " " 
             << sorted[i].stock << "\n";
    }
    file.close();
}

void displayDrinks(bool showSorted = false) {
    displayHeader(showSorted ? "Sorted Drinks" : "All Drinks");
    
    cout << left << setw(20) << "Name" << setw(15) << "Category" 
         << setw(10) << "Price" << setw(10) << "Stock" << "\n";
    cout << string(55, '-') << "\n";

    if (showSorted) {
        ifstream file("sorted_information.txt");
        if (!file.is_open()) {
            cout << "No data available.\n";
            return;
        }

        Drink temp;
        while (file >> temp.name >> temp.category >> temp.price >> temp.stock) {
            cout << left << setw(20) << temp.name 
                 << setw(15) << temp.category 
                 << setw(10) << temp.price 
                 << setw(10) << temp.stock << "\n";
        }
        file.close();
    } else {
        for (int i = 0; i < totalEntries; i++) {
            cout << left << setw(20) << drinks[i].name 
                 << setw(15) << drinks[i].category 
                 << setw(10) << drinks[i].price 
                 << setw(10) << drinks[i].stock << "\n";
        }
    }
}

void addNewDrinks() {
    int numToAdd = 0;
    while (true) {
        displayHeader("Add New Drinks");
        cout << "How many drinks to add? (0 to cancel, min 5): ";
        cin >> numToAdd;
        
        if (cin.fail()) { 
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
            waitForEnter();
            continue;
        }
        
        if (numToAdd == 0) return;
        if (numToAdd >= 5) break;
        
        cout << "Minimum 5 drinks required.\n";
        waitForEnter();
    }
    cin.ignore();

    for (int i = 0; i < numToAdd; i++) {
        displayHeader("Add Drink #" + to_string(i+1));
        
        cout << "Name: ";
        getline(cin, drinks[totalEntries].name);
        
        cout << "Available Categories:\n";
        for (int j = 0; j < totalCategories; j++) {
            cout << j+1 << ". " << categories[j] << "\n";
        }
        
        int catChoice;
        while (true) {
            cout << "Select Category (1-" << totalCategories << "): ";
            cin >> catChoice;
            if (cin.fail() || catChoice < 1 || catChoice > totalCategories) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid choice. Try again.\n";
            } else {
                break;
            }
        }
        cin.ignore();
        drinks[totalEntries].category = categories[catChoice-1];
        
        while (true) {
            cout << "Price: ";
            cin >> drinks[totalEntries].price;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid price. Try again.\n";
            } else {
                break;
            }
        }
        
        while (true) {
            cout << "Stock: ";
            cin >> drinks[totalEntries].stock;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid stock. Try again.\n";
            } else {
                break;
            }
        }
        cin.ignore();

        totalEntries++;
        saveDataToFile();
        saveSortedDataToFile();
    }
    
    cout << numToAdd << " drinks added successfully!\n";
    waitForEnter();
}

void editDrink() {
    displayHeader("Edit Drink");
    
    if (totalEntries == 0) {
        cout << "No drinks available to edit.\n";
        waitForEnter();
        return;
    }

    cout << left << setw(5) << "No." << setw(20) << "Name" << setw(15) << "Category" 
         << setw(10) << "Price" << setw(10) << "Stock" << "\n";
    cout << string(55, '-') << "\n";
    for (int i = 0; i < totalEntries; i++) {
        cout << left << setw(5) << i+1 
             << setw(20) << drinks[i].name 
             << setw(15) << drinks[i].category 
             << setw(10) << drinks[i].price 
             << setw(10) << drinks[i].stock << "\n";
    }
    
    int choice;
    while (true) {
        cout << "\nEnter drink number to edit (0 to cancel): ";
        cin >> choice;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        
        if (choice == 0) {
            cin.ignore();
            return;
        }
        
        if (choice < 1 || choice > totalEntries) {
            cout << "Invalid choice. Please select 1-" << totalEntries << ".\n";
        } else {
            break;
        }
    }
    cin.ignore();
    
    Drink& drink = drinks[choice-1];
    cout << "\nEditing: " << drink.name << "\n";
    
    cout << "New name (" << drink.name << "): ";
    string newName;
    getline(cin, newName);
    if (!newName.empty()) drink.name = newName;
    
    cout << "Available Categories:\n";
    for (int j = 0; j < totalCategories; j++) {
        cout << j+1 << ". " << categories[j] << "\n";
    }
    cout << "New category (" << drink.category << "): ";
    string newCat;
    getline(cin, newCat);
    if (!newCat.empty()) drink.category = newCat;
    
    cout << "New price (" << drink.price << "): ";
    string newPrice;
    getline(cin, newPrice);
    if (!newPrice.empty()) drink.price = stoi(newPrice);
    
    cout << "New stock (" << drink.stock << "): ";
    string newStock;
    getline(cin, newStock);
    if (!newStock.empty()) drink.stock = stoi(newStock);
    
    saveDataToFile();
    saveSortedDataToFile();
    cout << "Drink updated successfully!\n";
    waitForEnter();
}

Drink* loadSortedDrinks(int& size) {
    ifstream file("sorted_information.txt");
    if (!file.is_open()) {
        cout << "Cannot open sorted_information.txt\n";
        size = 0;
        return nullptr;
    }
    
    string line;
    size = 0;
    while (getline(file, line)) {
        size++;
    }
    file.close();
    
    Drink* sortedDrinks = new Drink[size];
    file.open("sorted_information.txt");
    for (int i = 0; i < size; i++) {
        file >> sortedDrinks[i].name >> sortedDrinks[i].category 
             >> sortedDrinks[i].price >> sortedDrinks[i].stock;
    }
    file.close();
    return sortedDrinks;
}

int ternarySearch(Drink arr[], int l, int r, const string& x) {
    if (r >= l) {
        int mid1 = l + (r - l) / 3;
        int mid2 = r - (r - l) / 3;
        if (arr[mid1].name == x) return mid1;
        if (arr[mid2].name == x) return mid2;
        if (x < arr[mid1].name) {
            return ternarySearch(arr, l, mid1 - 1, x);
        } else if (x > arr[mid2].name) {
            return ternarySearch(arr, mid2 + 1, r, x);
        } else {
            return ternarySearch(arr, mid1 + 1, mid2 - 1, x);
        }
    }
    return -1;
}

void searchDrink() {
    displayHeader("Search Drink");
    
    if (totalEntries == 0) {
        cout << "No drinks available to search.\n";
        waitForEnter();
        return;
    }
    
    string category;
    cout << "Enter drink category (0 to return): ";
    getline(cin, category);
    
    if (category == "0") {
        return;
    }
    
    int size;
    Drink* sortedDrinks = loadSortedDrinks(size);
    if (size == 0) {
        cout << "No data available.\n";
        waitForEnter();
        return;
    }
    
    int left = 0;
    int right = size - 1;
    int start = -1;
    while (left<=right) {
        int mid=left + (right-left) / 2;
        if (sortedDrinks[mid].category == category) {
            start=mid;
            right= mid-1; 
        } else if (sortedDrinks[mid].category<category) {
            left= mid+1;
        } else {
            right= mid-1;
        }
    }
    
    if (start == -1) {
        cout << "Category not found.\n";
        delete[] sortedDrinks;
        waitForEnter();
        return;
    }
    
    left=start;
    right=size - 1;
    int end=start;
    while (left<=right) {
        int mid = left + (right - left) / 2;
        if (sortedDrinks[mid].category == category) {
            end=mid;
            left=mid + 1; 
        } else if (sortedDrinks[mid].category < category) {
            left=mid + 1;
        } else {
            right=mid - 1;
        }
    }
    
    cout << "\nDrinks in category " <<category<< ":\n";
    cout << setw(20) << "Name" << setw(15) << "Category" 
         << setw(10) << "Price" << setw(10) << "Stock" << "\n";
    cout << string(55, '-') << "\n";
    for (int i = start; i <= end; i++) {
        cout << setw(20) << sortedDrinks[i].name 
             << setw(15) << sortedDrinks[i].category 
             << setw(10) << sortedDrinks[i].price 
             << setw(10) << sortedDrinks[i].stock << "\n";
    }
    
    string name;
    cout << "\nEnter drink name to search (0 to return): ";
    getline(cin, name);
    
    if (name == "0") {
        delete[] sortedDrinks;
        return; 
    }
    
    int result = ternarySearch(sortedDrinks, start, end, name);
    if (result != -1) {
        cout << "\nDrink Found:\n";
        cout << "Name: " <<sortedDrinks[result].name << "\n";
        cout << "Category: " <<sortedDrinks[result].category << "\n";
        cout << "Price: " <<sortedDrinks[result].price << "\n";
        cout << "Stock: " <<sortedDrinks[result].stock << "\n";
    } else {
        cout << "Drink not found in category " <<category<< ".\n";
    }
    
    delete[] sortedDrinks;
    waitForEnter();
}

void removeDrink() {
    displayHeader("Remove Drink");
    
    if (totalEntries == 0) {
        cout<< "No drinks available to remove.\n";
        waitForEnter();
        return;
    }
    
    cout << left << setw(5) << "No." << setw(20) << "Name" << setw(15) << "Category" 
         << setw(10) << "Price" << setw(10) << "Stock" << "\n";
    cout << string(55, '-') << "\n";
    for (int i = 0; i < totalEntries; i++) {
        cout << left << setw(5) << i+1 
             << setw(20) << drinks[i].name 
             << setw(15) << drinks[i].category 
             << setw(10) << drinks[i].price 
             << setw(10) << drinks[i].stock << "\n";
    }
    
    int choice;
    while (true) {
        cout<< "\nEnter drink number to remove (0 to cancel): ";
        cin>>choice;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout<< "Invalid input. Please enter a number.\n";
            continue;
        }
        
        if (choice == 0) {
            cin.ignore();
            return;
        }
        
        if (choice<1 || choice>totalEntries) {
            cout<< "Invalid choice. Please select a number between 1 and " <<totalEntries<< ".\n";
        } else {
            break;
        }
    }
    cin.ignore();
    
    cout << "\nAre you sure you want to remove " << drinks[choice-1].name << "? (Y/N): ";
    char confirm;
    cin >> confirm;
    cin.ignore();
    
    if (tolower(confirm) != 'y') {
        cout << "Removal cancelled.\n";
        waitForEnter();
        return;
    }
    
    for (int i=choice-1; i<totalEntries-1; i++) {
        drinks[i] =drinks[i+1];
    }
    
    totalEntries--;
    saveDataToFile();
    saveSortedDataToFile();
    
    cout << "Drink removed successfully!\n";
    waitForEnter();
}

void sortAndSaveDrinks() {
    displayHeader("Sort and Save Drinks");
    
    Drink sorted[MAX_ENTRIES];
    
    int categoryCount[MAX_CATEGORIES] = {0};
    for (int i=0; i<totalEntries; i++) {
        for (int j=0; j<totalCategories; j++) {
            if (drinks[i].category == categories[j]) {
                categoryCount[j]++;
                break;
            }
        }
    }
    
    int categoryIndex[MAX_CATEGORIES] = {0};
    for (int i = 1; i < totalCategories; i++) {
        categoryIndex[i] = categoryIndex[i-1] + categoryCount[i-1];
    }
    
    Stack categoryStacks[MAX_CATEGORIES];
    for (int i=0;i<totalEntries;i++) {
        for (int j=0; j<totalCategories; j++) {
            if (drinks[i].category == categories[j]) {
                categoryStacks[j].push(drinks[i]);
                break;
            }
        }
    }
    
    Drink tempSorted[MAX_ENTRIES];
    int currentIndex = 0;
    for (int cat = 0; cat < totalCategories; cat++) {
        Drink tempCategory[MAX_ENTRIES];
        int tempCount = 0;
        while (!categoryStacks[cat].isEmpty()) {
            tempCategory[tempCount++] = categoryStacks[cat].pop();
        }
        
        for (int i=1;i<tempCount;i++) {
            Drink key = tempCategory[i];
            int j=i - 1;
            while (j>=0 && tempCategory[j].name > key.name) {
                tempCategory[j+1] = tempCategory[j];
                j--;
            }
            tempCategory[j+1] = key;
        }
        
        for (int i=0;i<tempCount;i++) {
            tempSorted[currentIndex++] = tempCategory[i];
        }
    }
    
    for (int i=0;i<totalEntries;i++) {
        sorted[i] = tempSorted[i];
    }

    ofstream file("sorted_information.txt");
    if (!file.is_open()) {
        cout<< "Cannot save to sorted_information.txt\n";
        waitForEnter();
        return;
    }

    for (int i = 0; i < totalEntries; i++) {
        file <<sorted[i].name<< " " 
             <<sorted[i].category<< " " 
             <<sorted[i].price<< " " 
             <<sorted[i].stock<< "\n";
    }
    file.close();

    cout << "Drinks have been sorted and saved to sorted_information.txt\n";
    cout << "Sorted order:\n";
    cout <<left<<setw(20)<< "Name" <<setw(15) << "Category" 
         <<setw(10) << "Price" <<setw(10) << "Stock" << "\n";
    cout <<string(55, '-') << "\n";
    
    for (int i = 0; i < totalEntries; i++) {
        cout <<left <<setw(20) <<sorted[i].name 
             <<setw(15) <<sorted[i].category 
             <<setw(10) <<sorted[i].price 
             <<setw(10) <<sorted[i].stock << "\n";
    }
    
    waitForEnter();
}

void mainMenu() {
    while (true) {
        displayHeader("Main Menu");
        cout << "1. View All Drinks\n";
        cout << "2. Display Sorted Drinks by Categories and Name\n";
        cout << "3. Add New Drinks\n";
        cout << "4. Edit Drink\n";  
        cout << "5. Search Drink\n";
        cout << "6. Sort and Save Drinks\n";  
        cout << "7. Remove Drink\n";       
        cout << "8. Exit\n";                
        cout << "Choose option: ";
        
        int choice;
        cin >> choice;
        cin.ignore();
        
        switch (choice) {
            case 1:
                displayDrinks(false);
                waitForEnter();
                break;
            case 2:
                displayDrinks(true);
                waitForEnter();
                break;
            case 3:
                addNewDrinks();
                break;
            case 4:  
                editDrink();
                break;
            case 5:
                searchDrink();
                break;
            case 6:  
                sortAndSaveDrinks();
                break;
            case 7: 
                removeDrink();
                break;
            case 8: 
                return;
            default:
                cout << "Invalid choice. Try again.\n";
                waitForEnter();
        }
    }
}

int main() {
    initializeCategories();
    readDataFromFile();
    mainMenu();
    return 0;
}
