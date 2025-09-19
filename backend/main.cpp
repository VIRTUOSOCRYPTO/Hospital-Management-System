#include <bits/stdc++.h>
#include <mysql/mysql.h>
using namespace std;

// ---------- Utility Functions ----------
void viewPatients(MYSQL* conn) {
    if (mysql_query(conn, "SELECT id, name, age, disease FROM patients")) {
        cerr << "Query failed: " << mysql_error(conn) << endl;
        return;
    }
    MYSQL_RES* res = mysql_store_result(conn);
    MYSQL_ROW row;

    cout << "\n--- Patient List ---\n";
    while ((row = mysql_fetch_row(res))) {
        cout << "ID: " << row[0] << " | Name: " << row[1]
             << " | Age: " << row[2] << " | Disease: " << row[3] << endl;
    }
    mysql_free_result(res);
}

void addPatient(MYSQL* conn, int doctorId) {
    string name, disease;
    int age;

    cin.ignore();
    cout << "Enter patient name: ";
    getline(cin, name);
    cout << "Enter age: ";
    cin >> age;
    cin.ignore();
    cout << "Enter disease: ";
    getline(cin, disease);

    string query = "INSERT INTO patients (name, age, disease, doctor_id) VALUES ('" +
                   name + "', " + to_string(age) + ", '" + disease + "', " + to_string(doctorId) + ")";

    if (mysql_query(conn, query.c_str())) {
        cerr << "Insert failed: " << mysql_error(conn) << endl;
    } else {
        cout << "Patient added successfully!\n";
    }
}

void updatePatient(MYSQL* conn) {
    int id, age;
    string disease;

    cout << "Enter patient ID to update: ";
    cin >> id;
    cout << "Enter new age: ";
    cin >> age;
    cin.ignore();
    cout << "Enter new disease: ";
    getline(cin, disease);

    string query = "UPDATE patients SET age=" + to_string(age) +
                   ", disease='" + disease + "' WHERE id=" + to_string(id);

    if (mysql_query(conn, query.c_str())) {
        cerr << "Update failed: " << mysql_error(conn) << endl;
    } else {
        cout << "Patient updated successfully!\n";
    }
}

void deletePatient(MYSQL* conn) {
    int id;
    cout << "Enter patient ID to delete: ";
    cin >> id;

    string query = "DELETE FROM patients WHERE id=" + to_string(id);

    if (mysql_query(conn, query.c_str())) {
        cerr << "Delete failed: " << mysql_error(conn) << endl;
    } else {
        cout << "Patient deleted successfully!\n";
    }
}

void viewOwnRecord(MYSQL* conn, const string& username) {
    string query = "SELECT name, age, disease FROM patients WHERE name='" + username + "'";
    if (mysql_query(conn, query.c_str())) {
        cerr << "Query failed: " << mysql_error(conn) << endl;
        return;
    }
    MYSQL_RES* res = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(res);

    if (row) {
        cout << "\nYour Record -> Name: " << row[0]
             << " | Age: " << row[1]
             << " | Disease: " << row[2] << endl;
    } else {
        cout << "No record found.\n";
    }
    mysql_free_result(res);
}

// ---------- Main ----------
int main() {
    MYSQL* conn;
    conn = mysql_init(0);
    conn = mysql_real_connect(conn, "localhost", "hms", "hms123", "hospital", 3306, NULL, 0);

    if (!conn) {
        cout << "Database connection failed: " << mysql_error(conn) << endl;
        return 1;
    }
    cout << "Connected to DB\n";

    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    string query = "SELECT id, role FROM users WHERE username='" + username +
                   "' AND password='" + password + "'";
    if (mysql_query(conn, query.c_str())) {
        cerr << "Login query failed: " << mysql_error(conn) << endl;
        return 1;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    if (mysql_num_rows(res) > 0) {
        MYSQL_ROW row = mysql_fetch_row(res);
        int userId = stoi(row[0]);
        string role = row[1];
        cout << "Login successful! Role: " << role << "\n";

        // Audit log
        string log = "INSERT INTO audit_logs(username, action) VALUES('" +
                     username + "', 'login')";
        mysql_query(conn, log.c_str());

        if (role == "doctor") {
            int choice;
            do {
                cout << "\n--- Doctor Dashboard ---\n";
                cout << "1. View Patients\n";
                cout << "2. Add Patient\n";
                cout << "3. Update Patient\n";
                cout << "4. Delete Patient\n";
                cout << "5. Logout\n";
                cout << "Enter choice: ";
                cin >> choice;

                switch (choice) {
                    case 1: viewPatients(conn); break;
                    case 2: addPatient(conn, userId); break;
                    case 3: updatePatient(conn); break;
                    case 4: deletePatient(conn); break;
                    case 5: cout << "Logging out...\n"; break;
                    default: cout << "Invalid choice\n";
                }
            } while (choice != 5);
        } else {
            cout << "\n--- Patient Dashboard ---\n";
            viewOwnRecord(conn, username);
        }
    } else {
        cout << "Invalid credentials\n";
    }

    mysql_free_result(res);
    mysql_close(conn);
    return 0;
}
