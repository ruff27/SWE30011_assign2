from flask import Flask, render_template, request, redirect, url_for
import pymysql
import serial

app = Flask(__name__)

arduino = serial.Serial('/dev/ttyACM0', 9600)
last_command = None  # Track last fan command

@app.route('/', methods=['GET', 'POST'])
def index():
    global last_command
    if request.method == 'POST':
        action = request.form['action']
        if action == "fan_on":
            arduino.write(b"FAN_ON\n")
            last_command = "ON"
            print("Sent FAN_ON to Arduino")
        elif action == "fan_off":
            arduino.write(b"FAN_OFF\n")
            last_command = "OFF"
            print("Sent FAN_OFF to Arduino")
        return redirect(url_for('index'))

    db = pymysql.connect(host="localhost", user="pi", password="", database="env_db")
    cursor = db.cursor()
    cursor.execute("SELECT temperature, timestamp FROM sensor_data ORDER BY timestamp DESC LIMIT 5")
    data = cursor.fetchall()
    db.close()
    return render_template('index.html', data=data, fan_status=last_command)

if __name__ == "__main__":
    app.run(host='0.0.0.0', port=8080, debug=True)
