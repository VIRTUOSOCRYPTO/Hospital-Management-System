# 🏥 Hospital Management System (C++ + SQL )

## 📌 Features
- Patient/Doctor login system (SQL-based authentication).
- Role-based dashboards (Doctor sees all patients, Patient sees own record).
- Audit logs (tracks every login).
- Frontend login page (HTML/CSS).
- Backend in **C++** with MySQL integration.

## 📂 Tech Stack
- **Backend**: C++
- **Database**: MySQL

## 🚀 How to Run
1. Import `database/schema.sql` into MySQL.
2. Update MySQL password in `backend/main.cpp`.
3. Compile:
   ```bash
   g++ main.cpp -o hospital -lmysqlclient
   ./hospital
