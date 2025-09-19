CREATE DATABASE hospital;
USE hospital;

-- Users table
CREATE TABLE users (
  id INT AUTO_INCREMENT PRIMARY KEY,
  username VARCHAR(50) UNIQUE,
  password VARCHAR(100),
  role ENUM('doctor','patient') NOT NULL
);

-- Patients table
CREATE TABLE patients (
  id INT AUTO_INCREMENT PRIMARY KEY,
  name VARCHAR(100),
  age INT,
  disease VARCHAR(100),
  doctor_id INT,
  FOREIGN KEY (doctor_id) REFERENCES users(id)
);

-- Audit logs
CREATE TABLE audit_logs (
  id INT AUTO_INCREMENT PRIMARY KEY,
  username VARCHAR(50),
  action VARCHAR(100),
  time TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Sample data
INSERT INTO users(username,password,role) VALUES
('dr_smith','1234','doctor'),
('john_doe','abcd','patient');

INSERT INTO patients(name,age,disease,doctor_id) VALUES
('John Doe',30,'Flu',1),
('Jane Roe',25,'Asthma',1);
