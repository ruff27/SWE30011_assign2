from flask import Flask, render_template, redirect, url_for
import pymysql
import serial

app = Flask(__name__)

# Arduino serial connection
device = '/dev/ttyACM0'
try:
    arduino = serial.Serial(device, 9600, timeout=1)
    print(f"Connected to Arduino on {device}")
except:
    arduino = None
    print("Arduino not connected")

def get_latest_data():
    try:
        dbconn = pymysql.connect(
            host="localhost",
            user="pi",
            password="",
            database="env_db"
        )
        cursor = dbconn.cursor()
        cursor.execute(
            "SELECT temperature, led_status, fan_status, timestamp FROM sensor_log ORDER BY id DESC LIMIT 5"
        )
        rows = cursor.fetchall()
        cursor.close()
        return rows
    except:
        return []

@app.route("/")
def index():
    data = get_latest_data()
    return render_template("index.html", sensor_data=data)

@app.route("/fan/on")
def fan_on():
    if arduino:
        arduino.write(b"FAN_ON\n")
    return redirect(url_for('index'))

@app.route("/fan/off")
def fan_off():
    if arduino:
        arduino.write(b"FAN_OFF\n")
    return redirect(url_for('index'))

if __name__ == "__main__":
    app.run(debug=True, host="0.0.0.0", port=8080)
