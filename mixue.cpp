#include <iostream>
#include <string>
#include <fstream>
#include <limits>    // To help check user inputs are valid
#include <cctype>    // For character functions (like tolower)
#include <iomanip>  // For formatting output (like setw)


using namespace std;

const int TABLE_SIZE = 50;  // Size of hash table, determines how many slots the hash table has 

struct Drink{
	string name;
	string type;
	double price;     
    int stock;        
    Drink* next;   // Pointer to next drink in the chain (linked list), used to handle collisions
	
	Drink(const string& n, const string& t, double p, int s) {
        name = n;
        type = t;
        price = p;
        stock = s;
        next = NULL;
    }
}; 

string toLower(const string& s) {
    string result = s;
    for (int i = 0; i < result.length(); i++) {
        result[i] = tolower(result[i]);
    }
    return result;
}

void printCentered(const string& text, int width = 80) {
    int pad = (width - (int)text.length()) / 2;  // Calculate left padding
    if (pad < 0) pad = 0;
    cout << string(pad, ' ') << text << endl;
}

class HashTable {
private:
    Drink* table[TABLE_SIZE];  // Array of pointers to linked lists
    
	// Simple hash function: sum ASCII values of lowercase letters and mod table size
    int hashFunction(const string& key) {
    	string lowerKey = toLower(key);
        int hash = 0;
        for (size_t i = 0; i < lowerKey.length(); i++) {
            hash += (int)lowerKey[i];  // Add ASCII value of each character
        }
        return hash % TABLE_SIZE;  // Get index within table size
    }
    
    
public:
    HashTable() {   //When a HashTable is created, this sets all entries in the table to NULL (empty)
        for (int i = 0; i < TABLE_SIZE; i++) {
            table[i] = NULL;  
        }
    }

    ~HashTable() {    //When the program ends, this deletes all drinks in every table slot to free memory.
        for (int i = 0; i < TABLE_SIZE; i++) {
            Drink* current = table[i];        // Start with the first drink in the list
            while (current != NULL) {        // While there's a drink in the list
                Drink* temp = current;      // While there's a drink in the list
                current = current->next;   // Move to the next drink
                delete temp;		 	 // Delete current drink to free memory
            }
        }
    }
    
    // Insert a new drink or update if it already exists in the hash table
    void insert(const string& name, const string& type, double price, int stock) {
        int index = hashFunction(name);      // Compute hash index based on drink name
    	Drink* current = table[index];          
		string lowerName = toLower(name);
		
		
    	// Check if drink already exists to update
        while (current != NULL) {         
            if (toLower(current->name) == lowerName) {
                current->type = type;
                current->price = price;
                current->stock = stock;
                return;
            }
            current = current->next;     // Move to next drink in list
        }
        
        // If not found, insert new drink at head of list
        Drink* newDrink = new Drink(name, type, price, stock);   
        newDrink->next = table[index];
        table[index] = newDrink;
    }
    
    Drink* search(const string& name) {
        int index = hashFunction(name);   //Calculate hash index using the drink name
        Drink* current = table[index];   //Start at the linked list head at that index
        string lowerName = toLower(name);
        
        while (current != NULL) {       //Loop through the linked list
            if (toLower(current->name) == lowerName) {    //If drink found, return pointer to it
                return current;
            }
            current = current->next;
        }
        return NULL;
    }
    	
    bool remove(const string& name) {
        int index = hashFunction(name);
        Drink* current = table[index];
        Drink* prev = NULL;   //Keep track of previous node (for remove)
        string lowerName = toLower(name);

        while (current != NULL) {   //Traverse linked list
            if (toLower(current->name) == lowerName) {
                if (prev == NULL) {     //If it's the first node in the list,
                    table[index] = current->next;   // remove it by changing head pointer
                } else {
                    prev->next = current->next;    // bypass current node
                }
                delete current;   // Free memory
                return true;
            }
            prev = current;
            current = current->next;
        }
        return false;
    }
    
    void update(const string& name,const string& newType, double newPrice, int newStock) {
        Drink* d = search(name);
        string lowerName = toLower(name);
        if (d != NULL) {
        	d->type = newType;
            d->price = newPrice;  //Update the price
            d->stock = newStock;
            cout << "Drink details updated.\n";
    	} else {
        	cout << "Drink not found.\n";
        }
    }
    
    void displayAll() {
    cout << "\n                        Drink List                         \n";
    cout << "-------------------------------------------------------------\n";
    cout << "| Name               | Type         | Price (RM) | Stock    |\n";
    cout << "-------------------------------------------------------------\n";

    for (int i = 0; i < TABLE_SIZE; i++) {
        Drink* current = table[i];
        while (current) {
            cout << "| " << setw(18) << left << current->name
                 << "| " << setw(13) << left << current->type
                 << "| " << setw(10) << fixed << setprecision(2) << current->price
                 << "| " << setw(10) << current->stock
                 << "|\n";
            current = current->next;
        }
    }
	cout << "-------------------------------------------------------------\n";   
}
    
    void displayByType(const string& queryType) {
    	string lowerType = toLower(queryType);
    	bool found = false;
    	
    		cout << "\n                  Drinks of type \"" << queryType << "\"   \n";
			cout << "-------------------------------------------------------------\n";
    		cout << "| Name               | Type         | Price (RM) | Stock    |\n";
    		cout << "-------------------------------------------------------------\n";
    		
    		for (int i = 0; i < TABLE_SIZE; ++i) {
        		Drink* current = table[i];
        		while (current) {
            		if (toLower(current->type) == lowerType) {
                		cout << "| " << setw(19) << left << current->name
                     		 << "| " << setw(13) << left << current->type
                     		 << "| " << setw(10) << fixed << setprecision(2) << current->price
                     		 << "| " << setw(10) << current->stock
                     		 << "|\n";
                		found = true;
            		}
            		current = current->next;
        		}
        	}
			cout << "-------------------------------------------------------------\n";
    	if (!found) {
    		cout << "No drinks found for the type.\n";
    	}
	}
    
    void loadFromFile(const string& filename) {  
        cout << "Loading drink data from file...\n";      // Inform user that program is searching for the file

        ifstream fin(filename.c_str());   // Open file for reading
        if (!fin) {
            cout << "Unable to find file: " << filename << endl;
            return;
        }
        string name, type;
        double price;
        int stock;

        while (fin >> name >> type >> price >> stock) {
            cout << "Loaded: " << name << ", " << type << ", " << price << ", " << stock << endl;
            insert(name, type, price, stock);    // Add each drink to the hash table
        }

        fin.close();
        cout << "Data loaded from " << filename << endl;  // Inform user loading is done
    }
    
    void saveToFile(const string& filename) {
        ofstream fout(filename.c_str());  // Open file for writing
        if (!fout) {             // Check if file opened successfully
            cout << "Cannot open file to save: " << filename << endl;
            return;
        }
        for (int i = 0; i < TABLE_SIZE; i++) {
            Drink* current = table[i];
            while (current != NULL) {
                fout << current->name << " " << current->type << " " << current->price << " " << current->stock << "\n";
                current = current->next;
            }
        }
        fout.close();
        cout << "Changes saved to " << filename << endl;
    }
};

void pause() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');  //Clears any leftover input from the user 
    cin.get();
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Ask user for an integer input with validation
int getValidatedInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            return value;
        } else {
            cout << "Invalid input. Please enter a valid integer.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

// Ask user for a number input with validation
double getValidatedDouble(const string& prompt) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        } else {
            cout << "Invalid input. Please enter a valid number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

void manageItemsMenu(HashTable& shop);

int main() {
    HashTable shop;
    shop.loadFromFile("mixue.txt");

    int choice;
    do {
        clearScreen();
        printCentered("=== MIXUE DRINK MANAGEMENT ===");
        
    
        cout << endl;
        printCentered("1. Manage Items");
        printCentered("0. Exit");
        cout << "\nEnter choice: ";
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = -1; // Invalid choice
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        if (choice == 1) {
            manageItemsMenu(shop);
        } else if (choice != 0) {
            cout << "Invalid choice, try again.\n";
            pause();
        }
    } while (choice != 0);

    cout << "Exiting program.\n";
    return 0;
}

void manageItemsMenu(HashTable& shop) {
    int choice;
    do {
        clearScreen();
        printCentered("=== Manage Items Menu ===");
        cout << endl;
        printCentered("1. Add New Drink\n");
        printCentered("2. Search for a Drink\n");
        printCentered("3. Display All Drinks\n");
        printCentered("4. Display Drinks by Type\n");
        printCentered("5. Remove a Drink\n");
        printCentered("6. Update Drink Details\n");
        printCentered("7. Save Changes to File\n");
        printCentered("0. Back to Main Menu\n");
        cout << "Please choose an option: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = -1; // Invalid choice
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        if (choice == 1) {  //Add item
            clearScreen();
            string name, type;
            cout << "Add New Drink\n";
            cout << "Enter name: ";
            getline(cin, name);
            cout << "Enter type: ";
            getline(cin, type);
            double price = getValidatedDouble("Enter price: ");
            int stock = getValidatedInt("Enter stock: ");
            shop.insert(name, type, price, stock);
            cout << "Drink added successfully! \n";
            pause();

        } else if (choice == 2) {  //Search Drink
            clearScreen();
            string name;
            cout << "Search Drink\n";
            cout << "Enter name: ";
            getline(cin, name);
            Drink* d = shop.search(name);
            if (d != NULL) {
                cout << "Drink found: " << d->name
                     << ", Type: " << d->type
                     << ", Price: RM" << d->price
                     << ", Stock: " << d->stock << endl;
            } else {
                cout << "No matching drink found.\n";
            }
            pause();

        } else if (choice == 3) {  //Display All Drinks
            clearScreen();
            shop.displayAll();
            pause();

		} else if (choice == 4) {  //Display Drinks by Type
            clearScreen();
            string type;
            cout << "Enter drink type to display: ";
            getline(cin, type);
			shop.displayByType(type);
            pause();


        } else if (choice == 5) {  //Remove Drink
            clearScreen();
            string name;
            cout << "Remove Drink\n";
            cout << "Enter name: ";
            getline(cin, name);
            bool removed = shop.remove(name);
            if (removed) {
                cout << "Drink successfully removed from the list.\n";
            } else {
                cout << "Drink not found.\n";
            }
            pause();

        } else if (choice == 6) {
    		clearScreen();
    		string name;
    		cout << "Update Drink\n ";
    		cout << "Enter name: ";
    		getline(cin, name);
    		Drink* d = shop.search(name);
    		if (d != NULL) {
        		cout << "Current type: " << d->type << endl;
        		cout << "Enter new type: ";
        		string newType;
        		getline(cin, newType);

        		cout << "Current price: RM" << d->price << endl;
        		double newPrice = getValidatedDouble("Enter new price: ");

        		cout << "Current stock: " << d->stock << endl;
        		int newStock = getValidatedInt("Enter new stock: ");

        		// Update all fields
        		d->type = newType;
        		d->price = newPrice;
        		d->stock = newStock;

        		cout << "Drink updated.\n";
    		} else {
        		cout << "Drink not found.\n";
    		}
   		 	pause();
   		 	
		} else if (choice == 7) {  //Save data to File
            clearScreen();
            shop.saveToFile("mixue.txt");
            pause();

        } else if (choice == 0) {  //Back to Main Menu
            break;

        } else {
            cout << "Invalid choice, try again.\n";
            pause();
        }

    } while (choice != 0);
}