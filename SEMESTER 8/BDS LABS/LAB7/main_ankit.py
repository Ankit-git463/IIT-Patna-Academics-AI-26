import mysql.connector
import datetime

# ==============================
# ACCESS CONTROL MATRIX
# ==============================

PERMISSIONS = {
    'admin': {
        'stud_info': ['SELECT', 'INSERT', 'UPDATE', 'DELETE'],
        'users': ['SELECT', 'INSERT', 'UPDATE', 'DELETE'],
        'audit_log': ['SELECT']
    },

    'editor': {
        'stud_info': ['SELECT', 'INSERT', 'UPDATE'],
        'users': ['SELECT'],
        'audit_log': []
    },

    'viewer': {
        'stud_info': ['SELECT'],
        'users': [],
        'audit_log': []
    }
}

# ==============================
# QUERY LOG STORAGE
# ==============================

query_log = []

def log_query(user, table, query, status):

    timestamp = datetime.datetime.now().strftime("%H:%M:%S")

    query_log.append({
        "time": timestamp,
        "user": user,
        "table": table,
        "query": query,
        "status": status
    })


# ==============================
# SQL INJECTION DETECTION
# ==============================

def detect_sql_injection(value):

    patterns = ["'", "--", ";", " OR ", " AND ", "="]

    value = str(value).upper()

    for p in patterns:
        if p in value:
            return True

    return False


# ==============================
# DATABASE CONNECTION
# ==============================

def connect_db():

    print("\n=== DATABASE CONNECTION ===")

    db = mysql.connector.connect(
        host="localhost",
        user="root",
        password="iitpatna",
        database="stud_db"
    )

    print("Connected to MySQL database: stud_db")

    return db


# ==============================
# SECURE QUERY EXECUTION
# ==============================

def execute_query(user, role, table, operation, filters=None):

    print("\n=== ATTEMPTING QUERY ===")

    # -------- Permission Check --------

    if operation not in PERMISSIONS.get(role, {}).get(table, []):
        print(f"Permission check: ✗ {role} cannot {operation} from {table}")
        log_query(user, table, operation, "DENIED")
        return

    print(f"Permission check: ✓ {role} can {operation} from {table}")

    # -------- Input Validation --------

    if filters:
        for key, value in filters.items():

            if detect_sql_injection(value):

                print("Detection: ✗ Potential SQL injection detected")
                print("Action: Query blocked")

                log_query(user, table, value, "BLOCKED")
                return

    print("Input validation: ✓ Input safe")

    # -------- Database Connection --------

    db = connect_db()
    cursor = db.cursor()

    # -------- Prepared Statement --------

    if operation == "SELECT":

        query = f"SELECT * FROM {table} WHERE branch=%s"
        values = (filters["branch"],)

        print("Using prepared statement...")

        cursor.execute(query, values)

        results = cursor.fetchall()

        print("Query executed successfully")
        print("Results:", len(results), "records returned")

        log_query(user, table, query, "ALLOWED")

    db.close()


# ==============================
# SHOW QUERY LOG
# ==============================

def show_log():

    print("\n=== QUERY LOG ===")

    print("+----------+---------+----------+----------------------+----------+")

    for log in query_log:
        print(f"{log['time']} | {log['user']} | {log['table']} | {log['query']} | {log['status']}")

    print("+---------------------------------------------------------------+")


# ==============================
# MAIN PROGRAM
# ==============================

def main():

    print("=== DATABASE ACCESS CONTROL SYSTEM ===")

    user = input("Enter username: ")
    role = input("Enter role (admin/editor/viewer): ")

    while True:

        print("\n1. SELECT students by branch")
        print("2. Attempt DELETE (test permission)")
        print("3. Test SQL Injection")
        print("4. Show Query Log")
        print("5. Exit")

        choice = input("Enter choice: ")

        if choice == "1":

            branch = input("Enter branch: ")

            execute_query(
                user=user,
                role=role,
                table="stud_info",
                operation="SELECT",
                filters={"branch": branch}
            )

        elif choice == "2":

            execute_query(
                user=user,
                role=role,
                table="stud_info",
                operation="DELETE",
                filters={"roll": "CS101"}
            )

        elif choice == "3":

            branch = input("Enter branch (try SQL injection): ")

            execute_query(
                user=user,
                role=role,
                table="stud_info",
                operation="SELECT",
                filters={"branch": branch}
            )

        elif choice == "4":

            show_log()

        elif choice == "5":

            print("Exiting...")
            break

        else:
            print("Invalid choice")


# ==============================

main()