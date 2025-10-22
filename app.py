from flask import Flask, render_template, request, jsonify

app = Flask(__name__)

patients = [
    {'id': 101, 'name': 'Alice', 'age': 25},
    {'id': 102, 'name': 'Bob', 'age': 45},
]

@app.route('/')
def home():
    return render_template('index.html')

@app.route('/patients', methods=['GET'])
def get_patients():
    return jsonify(patients)

@app.route('/patients', methods=['POST'])
def add_patient():
    data = request.get_json()
    patients.append(data)
    return jsonify({"message": "Patient added"}), 201

if __name__ == '__main__':
    app.run(debug=True)
