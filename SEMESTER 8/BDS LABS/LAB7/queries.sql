-- Create database
CREATE DATABASE IF NOT EXISTS stud_db;

-- Use the database
USE stud_db;

-- Create student table
CREATE TABLE IF NOT EXISTS stud_info (
    roll VARCHAR(10) PRIMARY KEY,
    name VARCHAR(50),
    branch VARCHAR(10)
);

-- Insert demo records
INSERT INTO stud_info (roll, name, branch) VALUES
('CS106','Ayush','CSE'),
('CS105','Priyank','CSE'),
('EC102','Amit','ECE'),
('ME103','RaviShankar','ME')
ON DUPLICATE KEY UPDATE
name = VALUES(name),
branch = VALUES(branch);

-- Check the data
SELECT * FROM stud_info;