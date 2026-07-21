import pymysql
import sys
import subprocess
import time
from getpass import getpass
from flask import Flask
from flask_bcrypt import Bcrypt

# Initialize Flask app for Bcrypt context
app = Flask(__name__)
bcrypt = Bcrypt(app)

# Database Configuration for data init
DB_HOST = 'localhost'
DB_USER = 'rbac_user'
DB_PASSWORD = 'password'
DB_NAME = 'rbac_db'

def init_data():
    """Seeds the database with initial users."""
    try:
        conn = pymysql.connect(
            host=DB_HOST,
            user=DB_USER,
            password=DB_PASSWORD,
            database=DB_NAME,
            cursorclass=pymysql.cursors.DictCursor,
            autocommit=True
        )
        with conn.cursor() as cur:
            # Check if admin exists
            cur.execute("SELECT * FROM users WHERE username = 'admin_ankit'")
            existing_admin = cur.fetchone()

            if not existing_admin:
                print("Creating Admin user...")
                password_hash = bcrypt.generate_password_hash('password').decode('utf-8')
                cur.execute("INSERT INTO users (username, password_hash, role) VALUES (%s, %s, %s)", 
                            ('admin_ankit', password_hash, 'admin'))
                print("Admin user created (username: admin_ankit, password: password)")
            else:
                print("Admin user already exists.")

            # Create Editor content
            cur.execute("SELECT * FROM users WHERE username = 'editor_ankit'")
            if not cur.fetchone():
                print("Creating Editor user...")
                hash_pw = bcrypt.generate_password_hash('password').decode('utf-8')
                cur.execute("INSERT INTO users (username, password_hash, role) VALUES (%s, %s, %s)", 
                            ('editor_ankit', hash_pw, 'editor'))

            # Create Viewer content
            cur.execute("SELECT * FROM users WHERE username = 'viewer_ankit'")
            if not cur.fetchone():
                print("Creating Viewer user...")
                hash_pw = bcrypt.generate_password_hash('password').decode('utf-8')
                cur.execute("INSERT INTO users (username, password_hash, role) VALUES (%s, %s, %s)", 
                            ('viewer_ankit', hash_pw, 'viewer'))
            
        conn.close()
        print("Data initialization complete.")

    except pymysql.MySQLError as e:
        print(f"Error connecting to MySQL during data init: {e}")
        print("Ensure the database 'rbac_db' exists and user 'rbac_user' has access.")

def run_sql_commands(commands, user, password, db_name=None):
    """Executes a list of SQL commands."""
    try:
        conn = pymysql.connect(
            host='localhost', 
            user=user, 
            password=password, 
            database=db_name,
            autocommit=True
        )
        with conn.cursor() as cur:
            for cmd in commands:
                if cmd.strip():
                    try:
                        cur.execute(cmd)
                    except pymysql.MySQLError as e:
                        # Ignore specific errors like "User already exists" if handled effectively by SQL,
                        # but print warnings for visibility.
                        print(f"  Note: {e}")
        conn.close()
        return True
    except pymysql.MySQLError as e:
        print(f"Connection Error: {e}")
        return False

# SQL Scripts
SETUP_SQL = """
CREATE DATABASE IF NOT EXISTS rbac_db;
CREATE USER IF NOT EXISTS 'rbac_user'@'localhost' IDENTIFIED BY 'password';
GRANT ALL PRIVILEGES ON rbac_db.* TO 'rbac_user'@'localhost';
FLUSH PRIVILEGES;
"""

SCHEMA_SQL = """
-- Users table
CREATE TABLE IF NOT EXISTS users (
    user_id INT PRIMARY KEY AUTO_INCREMENT,
    username VARCHAR(50) UNIQUE NOT NULL,
    password_hash VARCHAR(255) NOT NULL,
    role ENUM('admin', 'editor', 'viewer') NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Student information
CREATE TABLE IF NOT EXISTS stud_info (
    roll VARCHAR(20) PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    age INT,
    branch VARCHAR(50),
    hometown VARCHAR(100),
    created_by INT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (created_by) REFERENCES users(user_id)
);

-- Audit log
CREATE TABLE IF NOT EXISTS audit_log (
    log_id INT PRIMARY KEY AUTO_INCREMENT,
    user_id INT,
    action VARCHAR(50),
    table_name VARCHAR(50),
    record_id VARCHAR(20),
    timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    ip_address VARCHAR(45)
);
"""

def main():
    print("==========================================")
    print("   RBAC Portal - One-Click Setup & Run")
    print("==========================================")

    # 1. Database Setup
    print("\n[Step 1] Database Configuration")
    print("To set up the database, we need root access to MySQL.")
    print("Please enter your MySQL 'root' password (press Enter if empty):")
    
    # Use standard input for password to avoid getpass issues in some IDE terminals if needed, 
    # but getpass is cleaner.
    try:
        root_pass = getpass("> ")
    except Exception:
        root_pass = input("Password (visible): ")

    print("\nCreating Database and User...")
    setup_cmds = SETUP_SQL.split(';')
    if not run_sql_commands(setup_cmds, 'root', root_pass):
        print("❌ Failed to create database/user. Check your root password.")
        retry = input("Try again? (y/n): ")
        if retry.lower() == 'y':
            main()
        return

    print("✓ Database 'rbac_db' and user 'rbac_user' ready.")

    print("\nCreating Tables...")
    try:
        schema_cmds = SCHEMA_SQL.split(';')
        # Use the newly created rbac_user
        if not run_sql_commands(schema_cmds, 'rbac_user', 'password', 'rbac_db'):
             print("❌ Failed to create tables.")
             return
        print("✓ Tables created successfully.")
    except Exception as e:
        print(f"❌ Table creation failed: {e}")
        return

    # 2. Data Initialization
    print("\n[Step 2] Seeding Data")
    try:
        init_data()
        print("✓ Admin, Editor, and Viewer users ready.")
    except Exception as e:
        print(f"❌ Data seeding failed: {e}")

    # 3. Run App
    print("\n[Step 3] Launching Application")
    print("Starting Flask server...")
    print("-> Access the app at: http://127.0.0.1:5000/login")
    print("-> Press Ctrl+C to stop.")
    print("==========================================\n")

    try:
        subprocess.check_call([sys.executable, 'app.py'])
    except KeyboardInterrupt:
        print("\n\nStopped by user.")
    except subprocess.CalledProcessError as e:
        print(f"\nApplication crashed with error code {e.returncode}")

if __name__ == "__main__":
    main()
