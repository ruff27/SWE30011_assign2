from flask import Flask, render_template
import pymysql

app = Flask(__name__)

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
            "SELECT temperature, led_status, fan_status, timestamp FROM sensor_data ORDER BY id DESC LIMIT 5"
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

if __name__ == "__main__":
    app.run(debug=True, host="0.0.0.0", port=8080)
