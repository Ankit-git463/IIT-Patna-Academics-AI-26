from flask import Flask, render_template, request, redirect, session, url_for, abort, flash
from flask_bcrypt import Bcrypt
from flask_wtf.csrf import CSRFProtect
from flaskext.mysql import MySQL
import pymysql.cursors
from functools import wraps

app = Flask(__name__)
app.secret_key = "super_secret_key"  
# Database Configuration
app.config['MYSQL_DATABASE_HOST'] = 'localhost'
app.config['MYSQL_DATABASE_USER'] = 'rbac_user'
app.config['MYSQL_DATABASE_PASSWORD'] = 'password'
app.config['MYSQL_DATABASE_DB'] = 'rbac_db'

mysql = MySQL(app)
bcrypt = Bcrypt(app)
csrf = CSRFProtect(app)

# ------------------ HELPERS ------------------

def get_db_connection():
    return mysql.get_db()

def require_role(*roles):
    def decorator(f):
        @wraps(f)
        def wrapped(*args, **kwargs):
            if 'role' not in session:
                return redirect(url_for('login'))
            if session['role'] not in roles:
                abort(403)
            return f(*args, **kwargs)
        return wrapped
    return decorator

def log_action(action, table_name, record_id):
    try:
        conn = get_db_connection()
        cur = conn.cursor()
        cur.execute("""
            INSERT INTO audit_log (user_id, action, table_name, record_id, ip_address)
            VALUES (%s, %s, %s, %s, %s)
        """, (
            session.get('user_id'),
            action,
            table_name,
            str(record_id),
            request.remote_addr
        ))
        conn.commit()
        cur.close()
    except Exception as e:
        print(f"Error logging action: {e}")

# ------------------ ROUTES ------------------

@app.route('/')
def index():
    if 'role' in session:
        return redirect(url_for('dashboard'))
    return redirect(url_for('login'))

@app.route('/login', methods=['GET', 'POST'])
def login():
    if request.method == 'POST':
        username = request.form['username']
        password = request.form['password']
        
        conn = get_db_connection()
        cur = conn.cursor(pymysql.cursors.DictCursor)
        cur.execute("SELECT * FROM users WHERE username=%s", (username,))
        user = cur.fetchone()
        cur.close()

        if user and bcrypt.check_password_hash(user['password_hash'], password):
            session['user_id'] = user['user_id']
            session['username'] = user['username']
            session['role'] = user['role']
            flash('Logged in successfully.', 'success')
            return redirect(url_for('dashboard'))
        else:
            flash('Invalid username or password.', 'danger')
            
    return render_template('login.html')

@app.route('/logout')
def logout():
    session.clear()
    flash('You have been logged out.', 'info')
    return redirect(url_for('login'))

@app.route('/dashboard')
def dashboard():
    if 'role' not in session:
        return redirect(url_for('login'))
    
    role = session['role']
    if role == 'admin':
        return render_template('admin_dashboard.html')
    elif role == 'editor':
        return render_template('editor_dashboard.html')
    elif role == 'viewer':
        return render_template('viewer_dashboard.html')
    else:
        return "Unknown Role", 403

# --- STUDENT MANAGEMENT ---

@app.route('/students')
@require_role('admin', 'editor', 'viewer')
def view_students():
    conn = get_db_connection()
    cur = conn.cursor(pymysql.cursors.DictCursor)
    cur.execute("SELECT * FROM stud_info")
    students = cur.fetchall()
    cur.close()
    
    log_action("SELECT", "stud_info", "ALL")
    return render_template('students.html', students=students)

@app.route('/students/add', methods=['POST'])
@require_role('admin', 'editor')
def add_student():
    roll = request.form['roll']
    name = request.form['name']
    age = request.form['age']
    branch = request.form['branch']
    hometown = request.form['hometown']
    created_by = session['user_id']

    conn = get_db_connection()
    cur = conn.cursor()
    try:
        cur.execute("""
            INSERT INTO stud_info (roll, name, age, branch, hometown, created_by)
            VALUES (%s, %s, %s, %s, %s, %s)
        """, (roll, name, age, branch, hometown, created_by))
        conn.commit()
        flash('Student added successfully!', 'success')
        log_action("CREATE", "stud_info", roll)
    except pymysql.MySQLError as e:
        flash(f'Error adding student: {e}', 'danger')
    finally:
        cur.close()
        
    return redirect(url_for('view_students'))

@app.route('/students/edit/<roll>', methods=['GET', 'POST'])
@require_role('admin', 'editor')
def edit_student(roll):
    conn = get_db_connection()
    cur = conn.cursor(pymysql.cursors.DictCursor)
    
    if request.method == 'GET':
        cur.execute("SELECT * FROM stud_info WHERE roll=%s", (roll,))
        student = cur.fetchone()
        cur.close()
        if not student:
            flash('Student not found!', 'danger')
            return redirect(url_for('view_students'))
        return render_template('edit_student.html', student=student)
    
    # POST handling
    name = request.form['name']
    age = request.form['age']
    branch = request.form['branch']
    hometown = request.form['hometown']

    try:
        cur.execute("""
            UPDATE stud_info
            SET name=%s, age=%s, branch=%s, hometown=%s
            WHERE roll=%s
        """, (name, age, branch, hometown, roll))
        conn.commit()
        flash('Student updated successfully!', 'success')
        log_action("UPDATE", "stud_info", roll)
    except pymysql.MySQLError as e:
        flash(f'Error updating student: {e}', 'danger')
    finally:
        cur.close()

    return redirect(url_for('view_students'))

@app.route('/students/delete/<roll>', methods=['POST'])
@require_role('admin')
def delete_student(roll):
    conn = get_db_connection()
    cur = conn.cursor()
    try:
        cur.execute("DELETE FROM stud_info WHERE roll=%s", (roll,))
        conn.commit()
        flash('Student deleted successfully!', 'success')
        log_action("DELETE", "stud_info", roll)
    except pymysql.MySQLError as e:
        flash(f'Error deleting student: {e}', 'danger')
    finally:
        cur.close()

    return redirect(url_for('view_students'))

# --- USER MANAGEMENT (Admin Only) ---

@app.route('/users')
@require_role('admin')
def manage_users():
    conn = get_db_connection()
    cur = conn.cursor(pymysql.cursors.DictCursor)
    cur.execute("SELECT user_id, username, role, created_at FROM users")
    users = cur.fetchall()
    cur.close()
    return render_template('users.html', users=users)

@app.route('/users/add', methods=['POST'])
@require_role('admin')
def add_user():
    username = request.form['username']
    password = request.form['password']
    role = request.form['role']
    password_hash = bcrypt.generate_password_hash(password).decode('utf-8')

    conn = get_db_connection()
    cur = conn.cursor()
    try:
        cur.execute("INSERT INTO users (username, password_hash, role) VALUES (%s, %s, %s)", 
                    (username, password_hash, role))
        conn.commit()
        flash('User added successfully!', 'success')
        log_action("CREATE", "users", username)
    except pymysql.MySQLError as e:
        flash(f'Error adding user: {e}', 'danger')
    finally:
        cur.close()
    return redirect(url_for('manage_users'))

@app.route('/users/delete/<int:user_id>', methods=['POST'])
@require_role('admin')
def delete_user(user_id):
    if user_id == session['user_id']:
        flash('Cannot delete yourself!', 'danger')
        return redirect(url_for('manage_users'))

    conn = get_db_connection()
    cur = conn.cursor()
    try:
        cur.execute("DELETE FROM users WHERE user_id=%s", (user_id,))
        conn.commit()
        flash('User deleted successfully!', 'success')
        log_action("DELETE", "users", user_id)
    except pymysql.MySQLError as e:
        flash(f'Error deleting user: {e}', 'danger')
    finally:
        cur.close()
    return redirect(url_for('manage_users'))

# --- AUDIT LOGS (Admin Only) ---

@app.route('/audit')
@require_role('admin')
def audit_logs():
    conn = get_db_connection()
    cur = conn.cursor(pymysql.cursors.DictCursor)
    cur.execute("""
        SELECT a.log_id, u.username, a.action, a.table_name, a.record_id, a.timestamp, a.ip_address
        FROM audit_log a
        LEFT JOIN users u ON a.user_id = u.user_id
        ORDER BY a.timestamp DESC
    """)
    logs = cur.fetchall()
    cur.close()
    return render_template('audit_logs.html', logs=logs)

if __name__ == '__main__':
    app.run(debug=True)
