from flask import Flask, render_template, request
import pymysql

app = Flask(__name__)

@app.route('/')
def index():
    db = pymysql.connect(host="localhost", user="pi", password="", database="env_db")
    cursor = db.cursor()
    # Get last 10 records
    cursor.execute("SELECT * FROM sensor_data ORDER BY timestamp DESC LIMIT 10")
    data = cursor.fetchall()
    db.close()
    return render_template('index.html', data=data)

@app.route('/control', methods=['POST'])
def control():
    action = request.form['action']
    if action == "fan_on":
        print("Fan ON command received (simulated)")
    elif action == "fan_off":
        print("Fan OFF command received (simulated)")
    return "Command sent. <a href='/'>Return to Dashboard</a>"

if __name__ == "__main__":
    app.run(host='0.0.0.0', port=5000, debug=True)
