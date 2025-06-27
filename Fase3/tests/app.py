from flask import Flask, request, jsonify
from flask_cors import CORS

app = Flask(__name__)
CORS(app)  # ← Esto permite que cualquier origen acceda (modo desarrollo)


# Simulación de datos
MEMORY_STATS = {
    "memory": {
        "total_memory": 8192,
        "used_memory": 4096,
        "free_memory": 2048,
        "cached_memory": 1024
    },
    "swap": {
        "total_swap": 2048,
        "used_swap": 1024,
        "free_swap": 1024
    }
}

PROCESSES = [
    {"pid": 123, "name": "python", "mem_percent": 2.5},
    {"pid": 456, "name": "chrome", "mem_percent": 8.1},
]

ANTIVIRUS_STATS = {
    "processes_monitored": 10,
    "files_scanned": 100,
    "quarantined_files": 5
}

QUARANTINE_FILES = {
    "files": [{"name": "malware1.exe"}, {"name": "malware2.exe"}]
}

PAGES_STATS = {
    "active_pages": 500,
    "active_pages_mem": 250,
    "inactive_pages": 300,
    "inactive_pages_mem": 150
}

# Rutas GET
@app.route('/api/mem_stats', methods=['GET'])
def get_mem_stats():
    return jsonify(MEMORY_STATS)

@app.route('/api/processes', methods=['GET'])
def get_processes():
    return jsonify({"processes": PROCESSES})

@app.route('/api/antivirus_stats', methods=['GET'])
def get_antivirus_stats():
    return jsonify(ANTIVIRUS_STATS)

@app.route('/api/quarantine_files', methods=['GET'])
def get_quarantine_files():
    return jsonify(QUARANTINE_FILES)

@app.route('/api/pages', methods=['GET'])
def get_pages():
    return jsonify(PAGES_STATS)

# Rutas POST
@app.route('/api/page-faults', methods=['POST'])
def post_page_faults():
    data = request.get_json()
    pid = data.get("pid")
    return jsonify({
        "pid": pid,
        "minor_faults": 123,
        "major_faults": 5
    })

@app.route('/api/scan_file', methods=['POST'])
def scan_file():
    data = request.get_json()
    return jsonify({
        "status": 200,
        "file_path": data.get("file_path"),
        "hash": data.get("hash")
    })

@app.route('/api/quarantine_file', methods=['POST'])
def quarantine_file():
    data = request.get_json()
    return jsonify({
        "status": 200,
        "file_path": data.get("file_path")
    })

@app.route('/api/restore_file', methods=['POST'])
def restore_file():
    data = request.get_json()
    return jsonify({
        "status": 200,
        "filename": data.get("filename")
    })

@app.route('/api/delete_file', methods=['POST'])
def delete_file():
    data = request.get_json()
    return jsonify({
        "status": 200,
        "filename": data.get("filename")
    })

@app.route('/api/add_signature', methods=['POST'])
def add_signature():
    data = request.get_json()
    return jsonify({
        "status": 200,
        "file_path": data.get("file_path"),
        "hash": data.get("hash")
    })

if __name__ == '__main__':
    app.run(debug=True)
