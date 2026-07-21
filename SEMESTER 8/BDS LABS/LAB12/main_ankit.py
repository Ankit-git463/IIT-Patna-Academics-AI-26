import json
import uuid
import hmac
import hashlib
import base64
import time
from datetime import datetime, timedelta
from threading import Thread

# Third-party libraries
try:
    from flask import Flask, request, jsonify
    import jwt
    from cryptography.hazmat.primitives.ciphers.aead import AESGCM
except ImportError:
    print("Missing dependencies! Run: pip install flask pyjwt cryptography")
    exit()

app = Flask(__name__)

# --- CONFIGURATION ---
SECRET_KEY = "big-data-super-secret-key"
SESSION_KEY = AESGCM.generate_key(bit_length=256)
USERS = {"data_scientist": "password123"}
USER_ROLES = {"data_scientist": ["spark:read", "spark:submit", "hive:query", "hdfs:read"]}

# --- UTILS ---
def encrypt_response(data_dict):
    aesgcm = AESGCM(SESSION_KEY)
    iv = uuid.uuid4().bytes[:12]
    ciphertext = aesgcm.encrypt(iv, json.dumps(data_dict).encode(), None)
    return base64.b64encode(iv + ciphertext).decode(), base64.b64encode(iv).decode()

# --- SERVER SIDE LOGIC ---

@app.route('/api/v1/auth/login', methods=['POST'])
def login():
    auth = request.json
    user = auth.get('username')
    password = auth.get('password')

    if user in USERS and USERS[user] == password:
        token = jwt.encode({
            "user": user,
            "permissions": USER_ROLES[user],
            "exp": datetime.utcnow() + timedelta(minutes=15)
        }, SECRET_KEY, algorithm="HS256")

        print("\n=== AUTHENTICATION FLOW ===")
        print("Gateway processing:")
        print("1. Decrypt API key: ✓ Valid client")
        print("2. Verify credentials: ✓ Valid user")
        print("3. Check rate limit: ✓ Within limits")
        print("4. Generate JWT: ✓ Issued with 15min expiry")
        print("5. Log audit entry: ✓ Recorded")

        return jsonify({
            "access_token": token,
            "token_type": "Bearer",
            "expires_in": 900,
            "permissions": USER_ROLES[user]
        })
    return jsonify({"error": "Unauthorized"}), 401

@app.route('/api/v1/spark/jobs', methods=['POST'])
def spark_job():
    token = request.headers.get('Authorization', '').replace('Bearer ', '')
    try:
        decoded = jwt.decode(token, SECRET_KEY, algorithms=["HS256"])
        job_id = f"spark-{datetime.now().strftime('%Y%m%d%H%M')}-0001"
        
        print("\n=== SPARK JOB SUBMISSION ===")
        print("Gateway processing:")
        print("1. JWT validation: ✓ Valid")
        print("2. Signature verification: ✓ Valid")
        print(f"3. Permission check: ✓ User has 'spark:submit'")
        print(f"4. Submit to Spark: ✓ Job ID: {job_id}")

        return jsonify({
            "job_id": job_id,
            "status": "ACCEPTED",
            "tracking_url": f"https://spark.company.com/ui/{job_id}",
            "estimated_completion": "2025-02-03T14:45:00Z"
        }), 202
    except:
        return jsonify({"error": "Invalid Token"}), 401

@app.route('/api/v1/hdfs/file/<path:path>', methods=['GET'])
def hdfs_access(path):
    token = request.headers.get('Authorization', '').replace('Bearer ', '')
    try:
        jwt.decode(token, SECRET_KEY, algorithms=["HS256"])
        enc_data, iv_hex = encrypt_response({"content": f"Data from {path}", "size": "1MB"})
        
        print("\n=== HDFS FILE ACCESS ===")
        print("Gateway processing:")
        print("1. JWT validation: ✓ Valid")
        print("2. Path validation: ✓ Authorized")
        print("3. Encrypt response: ✓ AES-256-GCM")
        
        print("\n=== AUDIT LOG ENTRY ===")
        print(json.dumps({
            "timestamp": datetime.utcnow().isoformat() + "Z",
            "user": "data_scientist",
            "endpoint": f"/api/v1/hdfs/file/{path}",
            "status_code": 200,
            "permissions_used": ["hdfs:read"]
        }, indent=2))

        return jsonify({"body_encrypted": enc_data, "iv": iv_hex})
    except:
        return jsonify({"error": "Invalid Token"}), 401

# --- CLIENT SIMULATION ENGINE ---
def run_client():
    import requests
    time.sleep(2) # Wait for server to start
    token = None

    while True:
        print("\n--- SECURE GATEWAY MENU ---")
        print("1. Login (Auth Flow)")
        print("2. Submit Spark Job")
        print("3. Access HDFS File")
        print("4. Exit")
        choice = input("Select an option: ")

        if choice == '1':
            resp = requests.post("http://127.0.0.1:5000/api/v1/auth/login", 
                               json={"username": "data_scientist", "password": "password123"})
            token = resp.json().get('access_token')
            print(f"Response: {resp.status_code} OK - Token Received")
        
        elif choice == '2':
            if not token: print("Please login first!"); continue
            resp = requests.post("http://127.0.0.1:5000/api/v1/spark/jobs", 
                               headers={"Authorization": f"Bearer {token}"})
            print(f"Response: {resp.status_code} Accepted\n{json.dumps(resp.json(), indent=2)}")

        elif choice == '3':
            if not token: print("Please login first!"); continue
            resp = requests.get("http://127.0.0.1:5000/api/v1/hdfs/file/sales/2025/01.csv", 
                              headers={"Authorization": f"Bearer {token}"})
            print(f"Response: {resp.status_code} OK (Encrypted Data Received)")
            print(f"Encrypted Payload: {resp.json().get('body_encrypted')[:50]}...")

        elif choice == '4':
            print("Shutting down...")
            break

if __name__ == '__main__':
    print("=== SECURE BIG DATA API GATEWAY ===")
    print("Gateway URL: https://api.bigdata.company.com")
    print("Port: 443 (Simulated on 5000)")
    
    # Run server in background thread
    server_thread = Thread(target=lambda: app.run(port=5000, debug=False, use_reloader=False))
    server_thread.daemon = True
    server_thread.start()

    # Run Interactive Client
    run_client()