#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdio>   // for remove(), rename()
using namespace std;

// ---------- Student record structure ----------
struct Student {
    int rollNo;
    char name[50];
    int age;
    float marks;
};

const char* FILENAME = "students.dat";

// ---------- Add a new student ----------
void addStudent() {
    Student s;
    ofstream fout(FILENAME, ios::binary | ios::app);
    if (!fout) {
        cout << "Error opening file!\n";
        return;
    }

    cout << "Enter Roll No: ";
    cin >> s.rollNo;
    cout << "Enter Name: ";
    cin.ignore();
    cin.getline(s.name, 50);
    cout << "Enter Age: ";
    cin >> s.age;
    cout << "Enter Marks: ";
    cin >> s.marks;

    fout.write((char*)&s, sizeof(Student));
    fout.close();
    cout << "Student added successfully!\n";
}

// ---------- Display all students ----------
void displayAll() {
    Student s;
    ifstream fin(FILENAME, ios::binary);
    if (!fin) {
        cout << "No records found.\n";
        return;
    }

    bool any = false;
    cout << left << setw(10) << "Roll" << setw(20) << "Name"
         << setw(6) << "Age" << "Marks\n";
    cout << "---------------------------------------------\n";

    while (fin.read((char*)&s, sizeof(Student))) {
        cout << left << setw(10) << s.rollNo << setw(20) << s.name
             << setw(6) << s.age << s.marks << "\n";
        any = true;
    }
    fin.close();

    if (!any) cout << "No records found.\n";
}

// ---------- Update an existing student ----------
void updateStudent() {
    int roll;
    bool found = false;
    cout << "Enter Roll No to update: ";
    cin >> roll;

    fstream file(FILENAME, ios::binary | ios::in | ios::out);
    if (!file) {
        cout << "No records found.\n";
        return;
    }

    Student s;
    while (file.read((char*)&s, sizeof(Student))) {
        if (s.rollNo == roll) {
            cout << "Enter new Name: ";
            cin.ignore();
            cin.getline(s.name, 50);
            cout << "Enter new Age: ";
            cin >> s.age;
            cout << "Enter new Marks: ";
            cin >> s.marks;

            // Move write pointer back to the start of this record
            file.seekp(-(int)sizeof(Student), ios::cur);
            file.write((char*)&s, sizeof(Student));
            found = true;
            break;
        }
    }
    file.close();
    cout << (found ? "Student updated successfully!\n" : "Roll No not found.\n");
}

// ---------- Delete a student ----------
void deleteStudent() {
    int roll;
    bool found = false;
    cout << "Enter Roll No to delete: ";
    cin >> roll;

    ifstream fin(FILENAME, ios::binary);
    if (!fin) {
        cout << "No records found.\n";
        return;
    }
    ofstream fout("temp.dat", ios::binary);

    Student s;
    while (fin.read((char*)&s, sizeof(Student))) {
        if (s.rollNo != roll)
            fout.write((char*)&s, sizeof(Student));
        else
            found = true;
    }
    fin.close();
    fout.close();

    remove(FILENAME);
    rename("temp.dat", FILENAME);

    cout << (found ? "Student deleted successfully!\n" : "Roll No not found.\n");
}

// ---------- Menu-driven main ----------
int main() {
    int choice;
    do {
        cout << "\n===== Student Management System =====\n";
        cout << "1. Add Student\n";
        cout << "2. Display All Students\n";
        cout << "3. Update Student\n";
        cout << "4. Delete Student\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addStudent();    break;
            case 2: displayAll();    break;
            case 3: updateStudent(); break;
            case 4: deleteStudent(); break;
            case 5: cout << "Exiting... Goodbye!\n"; break;
            default: cout << "Invalid choice! Try again.\n";
        }
    } while (choice != 5);

    return 0;
}
