from flask import Flask, render_template, request
import pymysql

app = Flask(__name__)

dbconn = pymysql.connect(
    host="localhost",
    user="pi",
    password="",
    database="env_monitor_db"
)

@app.route("/", methods=["GET", "POST"])
def index():
    threshold = None
    latest_data = {"potValue": "N/A", "motion": "N/A", "temperature": "N/A", "time": "N/A"}

    if request.method == "POST":
        threshold = request.form.get("threshold")  # Optional logic
        print("Threshold submitted:", threshold)

    try:
        cursor = dbconn.cursor()
        cursor.execute("SELECT potValue, motionDetected, temperature, created_at FROM environment_log ORDER BY id DESC LIMIT 1")
        row = cursor.fetchone()
        if row:
            latest_data = {
                "potValue": row[0],
                "motion": "Yes" if row[1] == 1 else "No",
                "temperature": row[2],
                "time": row[3]
            }
        cursor.close()
    except Exception as e:
        print(f"DB error: {e}")

    return render_template("index.html", data=latest_data, threshold=threshold)

if __name__ == "__main__":
    app.run(debug=True, host="0.0.0.0", port=8080)
