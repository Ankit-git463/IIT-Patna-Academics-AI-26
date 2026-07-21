# RBAC User Management Portal

This project implements a Role-Based Access Control (RBAC) system using Flask and MySQL.

## Project Structure (Simplified)
- **app.py**: Main application file containing all backend logic (Routes, Auth, DB, Audit).
- **init_db.py**: Script to initialize the database and create default users.
- **static/style.css**: Minimalist styling.
- **templates/**: HTML templates.

## Prerequisites
- Python 3.x
- MySQL Server running locally.

## Setup Instructions

1.  **Install Dependencies**:
    ```bash
    pip install -r requirement.txt
    ```

2.  **Database Configuration**:
    - The app expects a MySQL user `rbac_user` with password `password` and a database named `rbac_db`.
    - If you haven't set this up, run the legacy `setup_db_user.sql` in your MySQL client, or update `app.py` with your credentials.

3.  **Initialize Database**:
    - Run the initialization script to create tables and default users:
    ```bash
    python init_db.py
    ```
    - This will create the following users (password for all is their username + '123' e.g. `admin123`):
        - `admin` (Administrator)
        - `editor` (Editor)
        - `viewer` (Viewer)

4.  **Run the Application**:
    ```bash
    python app.py
    ```

5.  **Access**:
    - Open `http://localhost:5000/login` in your browser.

