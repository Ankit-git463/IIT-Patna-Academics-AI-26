"""
Symmetric Searchable Encryption — Interactive Terminal

"""

import os, sys, re, hmac, hashlib, sqlite3, time, glob
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.backends import default_backend

DB_PATH  = "sse_medical.db"
KEY_FILE = "sse.key"

# TERMINAL HELPERS

def hr(char="─", width=60): print(char * width)
def header(title):
    print()
    hr("═")
    print(f"  {title}")
    hr("═")

def ask(prompt, default=None):
    suffix = f" [{default}]" if default else ""
    try:
        val = input(f"\n  {'▶'} {prompt}{suffix}: ").strip()
        return val if val else default
    except (EOFError, KeyboardInterrupt):
        print()
        sys.exit(0)

def confirm(prompt):
    val = ask(f"{prompt} (y/n)", "n")
    return val.lower() in ("y", "yes")

def pause():
    try:
        input("\n  Press Enter to continue...")
    except (EOFError, KeyboardInterrupt):
        pass

# Cryptography functions to generate the key for storing the encrypted data

def load_or_create_key() -> bytes:
    if os.path.exists(KEY_FILE):
        with open(KEY_FILE, "rb") as f:
            key = f.read()
    else:
        key = os.urandom(32)
        with open(KEY_FILE, "wb") as f:
            f.write(key)
        print(f"  [KEY] New 256-bit key generated → {KEY_FILE}")
    return key

# Function to encrypt the data 

def aes_encrypt(key: bytes, plaintext: bytes) -> bytes:
    nonce = os.urandom(16)
    cipher = Cipher(algorithms.AES(key), modes.CTR(nonce), backend=default_backend())
    e = cipher.encryptor()
    return nonce + e.update(plaintext) + e.finalize()

# Function to decrpyt the data 

def aes_decrypt(key: bytes, blob: bytes) -> bytes:
    nonce, ct = blob[:16], blob[16:]
    cipher = Cipher(algorithms.AES(key), modes.CTR(nonce), backend=default_backend())
    d = cipher.decryptor()
    return d.update(ct) + d.finalize()

def make_token(key: bytes, word: str) -> bytes:
    return hmac.new(key, word.strip().lower().encode(), hashlib.sha256).digest()

def tok_str(t: bytes) -> str:
    return "enc_" + t.hex()[:12] + "..."

# SQLITE database to store the data and keywords in encrypted form 

SCHEMA = """
CREATE TABLE IF NOT EXISTS documents (
    enc_doc_id   BLOB PRIMARY KEY,
    filename     TEXT NOT NULL,
    enc_content  BLOB NOT NULL,
    indexed_at   TEXT DEFAULT (datetime('now'))
);
CREATE TABLE IF NOT EXISTS encrypted_index (
    keyword_token  BLOB NOT NULL,
    enc_doc_id     BLOB NOT NULL,
    PRIMARY KEY (keyword_token, enc_doc_id),
    FOREIGN KEY (enc_doc_id) REFERENCES documents(enc_doc_id)
);
CREATE INDEX IF NOT EXISTS idx_keyword ON encrypted_index(keyword_token);
"""

# function for the database connection
def get_conn():
    conn = sqlite3.connect(DB_PATH)
    conn.row_factory = sqlite3.Row
    conn.executescript(SCHEMA)
    conn.commit()
    return conn

# function to parse the patient data files 

def extract_keywords_from_value(value: str, keywords: set):
    """Tokenise a field value and add every meaningful token to keywords."""
    # Add the full value (lowercased, stripped) for exact-phrase searching
    full = value.strip().lower()
    if len(full) >= 2:
        keywords.add(full)

    # Split on common delimiters and add each token
    for tok in re.split(r"[\s,/_\-]+", value):
        tok = tok.strip(".()\r\n ")
        if len(tok) >= 2:
            keywords.add(tok.lower())


def extract_special_entities(raw: str, keywords: set):
    """
    Run regex passes over the raw text to pull out entities that may not
    appear on labelled colon-separated lines:
      • Dates  (DD/MM/YYYY, MM-DD-YYYY, YYYY-MM-DD, "12 March 2024", etc.)
      • Ages   ("45 years", "Age: 30", bare numbers 1-120 near 'year/old')
      • Names  (Dr. / Doctor / Mr. / Mrs. / Ms. followed by capitalised words)
      • IDs    (alphanumeric codes: P-1042, PT001, MRN-2991, …)
      • Medicines / dosages  (words ending in mg, ml, mcg, units)
    """

    # ── Dates ──────────────────────────────────────────────────────────────
    date_patterns = [
        r'\b\d{1,2}[\/\-]\d{1,2}[\/\-]\d{2,4}\b',            # 12/03/2024
        r'\b\d{4}[\/\-]\d{1,2}[\/\-]\d{1,2}\b',              # 2024-03-12
        r'\b\d{1,2}\s+(?:Jan(?:uary)?|Feb(?:ruary)?|Mar(?:ch)?|Apr(?:il)?|'
        r'May|Jun(?:e)?|Jul(?:y)?|Aug(?:ust)?|Sep(?:tember)?|'
        r'Oct(?:ober)?|Nov(?:ember)?|Dec(?:ember)?)\s+\d{2,4}\b',  # 12 March 2024
        r'\b(?:Jan(?:uary)?|Feb(?:ruary)?|Mar(?:ch)?|Apr(?:il)?|'
        r'May|Jun(?:e)?|Jul(?:y)?|Aug(?:ust)?|Sep(?:tember)?|'
        r'Oct(?:ober)?|Nov(?:ember)?|Dec(?:ember)?)\s+\d{1,2},?\s+\d{2,4}\b',  # March 12, 2024
    ]
    for pat in date_patterns:
        for m in re.finditer(pat, raw, re.IGNORECASE):
            val = m.group().strip()
            keywords.add(val.lower())
            # Also add individual parts (year, month name, day)
            for tok in re.split(r'[\s/\-,]+', val):
                if len(tok) >= 2:
                    keywords.add(tok.lower())

    # ── Ages ───────────────────────────────────────────────────────────────
    age_patterns = [
        r'\b(?:age|aged)[:\s]+(\d{1,3})\b',                   # Age: 45 / aged 45
        r'\b(\d{1,3})\s*(?:year[s]?[\s\-]old|yr[s]?[\s\-]old)\b',  # 45 years old
        r'\b(\d{1,3})\s*(?:years?|yrs?)\b',                   # 45 years
    ]
    for pat in age_patterns:
        for m in re.finditer(pat, raw, re.IGNORECASE):
            age_val = m.group(1) if m.lastindex else m.group()
            keywords.add(age_val.strip().lower())
            keywords.add(m.group().strip().lower())  # full phrase too

    # ── Doctor / staff names ───────────────────────────────────────────────
    name_patterns = [
        r'\b(?:Dr\.?|Doctor|Prof\.?|Professor)\s+([A-Z][a-z]+(?:\s+[A-Z][a-z]+)*)',
        r'\b(?:Mr\.?|Mrs\.?|Ms\.?|Miss)\s+([A-Z][a-z]+(?:\s+[A-Z][a-z]+)*)',
        r'\b(?:Physician|Surgeon|Nurse|Consultant|Specialist)[:\s]+([A-Z][a-z]+(?:\s+[A-Z][a-z]+)*)',
    ]
    for pat in name_patterns:
        for m in re.finditer(pat, raw):
            full_name = m.group().strip()
            keywords.add(full_name.lower())
            # Each part of the name separately
            for part in full_name.split():
                part = part.strip(".")
                if len(part) >= 2:
                    keywords.add(part.lower())

    # ── Patient / record IDs ───────────────────────────────────────────────
    id_patterns = [
        r'\b(?:P|PT|MRN|ID|Ref)[.\-]?\d{2,10}\b',            # P-1042, MRN12345
        r'\b[A-Z]{1,4}[\-_]\d{2,10}\b',                       # AB-00123
    ]
    for pat in id_patterns:
        for m in re.finditer(pat, raw, re.IGNORECASE):
            keywords.add(m.group().strip().lower())

    # ── Dosages / medications ──────────────────────────────────────────────
    dosage_pat = r'\b\d+(?:\.\d+)?\s*(?:mg|ml|mcg|µg|units?|tablets?|caps?|IU)\b'
    for m in re.finditer(dosage_pat, raw, re.IGNORECASE):
        keywords.add(m.group().strip().lower())
        # Also keep the numeric part alone
        num = re.match(r'[\d.]+', m.group())
        if num:
            keywords.add(num.group())


def parse_patient_file(path: str) -> dict:
    with open(path, "r", encoding="utf-8", errors="replace") as f:
        raw = f.read()

    lines = [l.strip() for l in raw.splitlines() if l.strip()]
    data = {"raw": raw, "keywords": set(), "fields": {}}

    for line in lines:
        # ── Pipe-separated structured header  ─────────────────────────────
        # e.g.  Patient ID: P-001 | Name: John Doe | DOB: 12/03/1980 | Age: 44
        if "Patient ID:" in line or ("|" in line and ":" in line):
            for part in line.split("|"):
                part = part.strip()
                if ":" in part:
                    k, _, v = part.partition(":")
                    field_key = k.strip().lower()
                    field_val = v.strip()
                    data["fields"][field_key] = field_val
                    extract_keywords_from_value(field_val, data["keywords"])
                    # Field name itself as keyword (e.g. "name", "dob", "age")
                    if len(field_key) >= 2:
                        data["keywords"].add(field_key)

        # ── Plain  Key: Value  lines  ──────────────────────────────────────
        elif ":" in line:
            k, _, v = line.partition(":")
            field_key = k.strip().lower()
            field_val = v.strip()
            data["fields"][field_key] = field_val
            extract_keywords_from_value(field_val, data["keywords"])
            if len(field_key) >= 2:
                data["keywords"].add(field_key)

        # ── Free-text / narrative lines  ───────────────────────────────────
        else:
            for tok in re.split(r"[\s,;.!?]+", line):
                tok = tok.strip("\"'()\r\n ")
                if len(tok) >= 2:
                    data["keywords"].add(tok.lower())

    # ── Entity extraction pass over the entire raw text  ───────────────────
    extract_special_entities(raw, data["keywords"])

    # ── Re-scan every captured field value  ───────────────────────────────
    for v in data["fields"].values():
        extract_keywords_from_value(v, data["keywords"])

    # Remove pure punctuation / single chars that slipped through
    data["keywords"] = {
        kw for kw in data["keywords"]
        if len(kw) >= 2 and not re.fullmatch(r'[^a-z0-9]+', kw)
    }

    return data


# Indexing the the parsed data 

def feature_index(key: bytes):
    header("INDEX DOCUMENTS")

    print("\n  Enter file path(s) to index.")
    print( "  Examples:")
    print( "    data/*.txt                       ← index all at once")
    print( "    data/patient_001.txt             ← single file")

    raw_input = ask("File path(s)")
    if not raw_input:
        print( "  No input given. Returning to menu.")
        return

    # Expand globs and spaces
    files = []
    for part in raw_input.split():
        expanded = glob.glob(part)
        if expanded:
            files.extend(expanded)
        elif os.path.isfile(part):
            files.append(part)
        else:
            print( f"  [WARN] No files matched: {part}")

    files = sorted(set(files))
    if not files:
        print( "  No valid files found.")
        return

    print(f"\n  { str(len(files))} file(s) found:")
    for f in files:
        print(f"    { '•'} {f}")

    if not confirm("Proceed with indexing?"):
        return

    conn = get_conn()
    hr()
    skipped = 0

    for path in files:
        fname = os.path.basename(path)
        enc_doc_id = make_token(key, fname)

        existing = conn.execute(
            "SELECT filename FROM documents WHERE enc_doc_id = ?", (enc_doc_id,)).fetchone()
        if existing:
            print( f"  [SKIP] {fname} — already indexed")
            skipped += 1
            continue

        parsed = parse_patient_file(path)
        keywords = parsed["keywords"]
        enc_content = aes_encrypt(key, parsed["raw"].encode())

        conn.execute(
            "INSERT INTO documents (enc_doc_id, filename, enc_content) VALUES (?, ?, ?)",
            (enc_doc_id, fname, enc_content)
        )
        for word in keywords:
            kw_token = make_token(key, word)
            conn.execute(
                "INSERT OR IGNORE INTO encrypted_index (keyword_token, enc_doc_id) VALUES (?, ?)",
                (kw_token, enc_doc_id)
            )
        conn.commit()

        print( f"  [OK] {fname}")
        print(f"       Keywords : {len(keywords)}  |  "
              f"Doc token : {tok_str(enc_doc_id)}  |  "
              f"Content   : {tok_str(enc_content[:16])}")
        # Show a few keyword → token mappings
        sample = list(keywords)[:3]
        for w in sample:
            print(f"       { 'E(K,'}\"{w}\"{ ')'} = {tok_str(make_token(key, w))}")

    doc_count = conn.execute("SELECT COUNT(*) FROM documents").fetchone()[0]
    idx_count = conn.execute("SELECT COUNT(*) FROM encrypted_index").fetchone()[0]
    conn.close()

    hr()
    print( f"  ✓ Indexing complete")
    print(f"    Total docs in DB   : { str(doc_count)}")
    print(f"    Total index rows   : { str(idx_count)}")
    print(f"    Skipped (dup)      : {skipped}")
    pause()

# FEATURE: Searching the database of keywords

def feature_search(key: bytes):
    header("SEARCH ENCRYPTED DATABASE")

    conn = get_conn()
    doc_count = conn.execute("SELECT COUNT(*) FROM documents").fetchone()[0]
    if doc_count == 0:
        print( "\n  Database is empty. Please index some files first.")
        conn.close()
        pause()
        return

    print( f"\n  Database contains {doc_count} encrypted document(s).")
    print( "  Supports: single keyword, AND, OR")
    print( "  Examples: diabetes  |  diabetes AND insulin  |  asthma OR hypertension")

    query = ask("Search query")
    if not query:
        conn.close()
        return

    # Parse boolean mode
    if re.search(r'\bAND\b', query, re.I):
        mode = "AND"
        keywords = [k.strip() for k in re.split(r'\s+AND\s+', query, flags=re.I)]
    elif re.search(r'\bOR\b', query, re.I):
        mode = "OR"
        keywords = [k.strip() for k in re.split(r'\s+OR\s+', query, flags=re.I)]
    else:
        mode = "SINGLE"
        keywords = [query.strip()]

    hr()
    print(f"  Mode     : { mode}")
    print(f"  Keywords : {keywords}")

    # Step 1 — Generate the encrypted token
    print(f"\n  { 'Step 1'} Token Generation { '(client-side)'}")
    tokens = {}
    for kw in keywords:
        t = make_token(key, kw)
        tokens[kw] = t
        print(f"    E(K, \"{kw}\") = { tok_str(t)}")
    print( "  → Server never sees plaintext keywords")

    # Step 2 — Database lookup 
    print(f"\n  { 'Step 2'} Encrypted Index Lookup { '(SQLite, server-side)'}")
    t0 = time.perf_counter()
    result_sets = []
    for kw, tok in tokens.items():
        rows = conn.execute(
            "SELECT enc_doc_id FROM encrypted_index WHERE keyword_token = ?", (tok,)
        ).fetchall()
        matched = {bytes(r["enc_doc_id"]) for r in rows}
        print(f"    {tok_str(tok)}  →  { str(len(matched))} match(es)")
        result_sets.append(matched)

    if mode == "AND":
        final_ids = result_sets[0]
        for s in result_sets[1:]: final_ids &= s
        print(f"    AND intersection → { str(len(final_ids))} document(s)")
    elif mode == "OR":
        final_ids = set()
        for s in result_sets: final_ids |= s
        print(f"    OR union → { str(len(final_ids))} document(s)")
    else:
        final_ids = result_sets[0] if result_sets else set()

    search_ms = (time.perf_counter() - t0) * 1000
    print(f"    Search time: { f'{search_ms:.4f}ms'}")

    # Step 3 — Retrieve encrypted blobs ie files 
    print(f"\n  { 'Step 3'} Retrieve Encrypted Docs from DB")
    enc_docs = []
    for enc_id in final_ids:
        row = conn.execute(
            "SELECT filename, enc_content FROM documents WHERE enc_doc_id = ?", (enc_id,)
        ).fetchone()
        if row:
            enc_docs.append((row["filename"], bytes(row["enc_content"])))
            print(f"    { '↓'} {row['filename']}  blob: {tok_str(bytes(row['enc_content'])[:16])}")

    # Step 4 — Decrypt client side and display 
    print(f"\n  { 'Step 4'} Client-side Decryption")
    t1 = time.perf_counter()
    results = []
    for fname, enc_content in enc_docs:
        plaintext = aes_decrypt(key, enc_content).decode(errors="replace")
        results.append((fname, plaintext))
    dec_ms = (time.perf_counter() - t1) * 1000
    print(f"    Decrypted { str(len(results))} do s) in { f'{dec_ms:.4f}ms'}")

    # Results
    hr("─")
    print( f"  RESULTS  ({len(results)} document(s) matched)")
    hr("─")

    if not results:
        print( f"\n  - No documents matched \"{query}\"")
    else:
        for i, (fname, text) in enumerate(results, 1):
            print(f" ─ Document {i}: {fname}")

    hr()
    print(f"  Query        : \"{ query}\"")
    print(f"  Matched      : { str(len(results))} document(s)")
    print(f"  Index lookup : {search_ms:.4f}ms  |  Decryption: {dec_ms:.4f}ms")
    
    conn.close()
    pause()

# FEATURE: STATS

def feature_stats():
    header("DATABASE STATISTICS")

    if not os.path.exists(DB_PATH):
        print( "\n  No database found. Index some files first.")
        pause()
        return

    conn = get_conn()
    doc_count = conn.execute("SELECT COUNT(*) FROM documents").fetchone()[0]
    idx_rows  = conn.execute("SELECT COUNT(*) FROM encrypted_index").fetchone()[0]
    uniq_kw   = conn.execute("SELECT COUNT(DISTINCT keyword_token) FROM encrypted_index").fetchone()[0]
    db_size   = os.path.getsize(DB_PATH)
    key_exists = os.path.exists(KEY_FILE)

    print(f"\n  {'DB file':<22}: {os.path.abspath(DB_PATH)}")
    print(f"  {'DB size':<22}: {db_size:,} bytes  ({db_size/1024:.1f} KB)")
    print(f"  {'Key file':<22}: {KEY_FILE}  ({'found' if key_exists else  'MISSING'})")
    print(f"  {'Documents stored':<22}: { str(doc_count)}")
    print(f"  {'Index rows':<22}: { str(idx_rows)}")
    print(f"  {'Unique keyword tokens':<22}: { str(uniq_kw)}")

    print(f"\n  { 'Documents in DB:'}")
    rows = conn.execute(
        "SELECT filename, indexed_at FROM documents ORDER BY indexed_at"
    ).fetchall()
    for r in rows:
        print(f"    { '•'} {r['filename']:<30}  indexed: { r['indexed_at']}")

    conn.close()
    pause()

# FEATURE: RESET

def feature_reset():
    header("RESET DATABASE")
    print( "\n  -  This will permanently delete the database and encryption key.")
    print( "     All indexed documents and tokens will be lost.")

    if not confirm("Are you sure you want to reset?"):
        print( "  Cancelled.")
        pause()
        return

    for f in [DB_PATH, KEY_FILE]:
        if os.path.exists(f):
            os.remove(f)
            print( f"  Deleted: {f}")
        else:
            print( f"  Not found: {f}")

    print( "\n  ✓ Database and key wiped. Fresh start on next run.")
    pause()

# MAIN MENU

MENU = [
    ("1", "Index files into encrypted DB",  "index"),
    ("2", "Search the encrypted database",  "search"),
    ("3", "Show database statistics",       "stats"),
    ("4", "Reset database & key",           "reset"),
    ("0", "Exit",                           "exit"),
]

def print_menu(key_loaded: bool, doc_count: int):
    header("SYMMETRIC SEARCHABLE ENCRYPTION  [SQLite]")
    print(f"\n  Key   : { KEY_FILE + ' (loaded)' if key_loaded else  'Not found — will create on first use'}")
    print(f"  DB    : { DB_PATH}  ({ str(doc_count) + ' documents'})")
    print()
    for num, label, _ in MENU:
        bullet =  f"  [{num}]" 
        print(f"{bullet}  {label}")
    print()
    hr()

def get_doc_count():
    if not os.path.exists(DB_PATH):
        return 0
    try:
        conn = sqlite3.connect(DB_PATH)
        conn.executescript(SCHEMA)
        n = conn.execute("SELECT COUNT(*) FROM documents").fetchone()[0]
        conn.close()
        return n
    except Exception:
        return 0

def main():
    print( "\n" + "═"*60)
    print( "  SYMMETRIC SEARCHABLE ENCRYPTION")
    print( "  AES-256-CTR + HMAC-SHA256 + SQLite")
    print( "═"*60)

    while True:
        key_loaded = os.path.exists(KEY_FILE)
        doc_count  = get_doc_count()
        print_menu(key_loaded, doc_count)

        choice = ask("Choose an option", "0")

        if choice == "1":
            key = load_or_create_key()
            feature_index(key)
        elif choice == "2":
            key = load_or_create_key()
            feature_search(key)
        elif choice == "3":
            feature_stats()
        elif choice == "4":
            feature_reset()
        elif choice in ("0", "q", "quit", "exit"):
            print( "\n  Goodbye.\n")
            sys.exit(0)
        else:
            print( f"\n  Invalid choice: '{choice}'. Enter 1-4 or 0.")
            time.sleep(0.5)

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print( "\n\n  Interrupted. Goodbye.\n")
        sys.exit(0)
