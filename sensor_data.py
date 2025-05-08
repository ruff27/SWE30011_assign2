import serial
import pymysql
import time

device = '/dev/ttyACM0'
baud_rate = 9600

arduino = serial.Serial(device, baud_rate, timeout=1)
time.sleep(2)

try:
    dbconn = pymysql.connect(
        host="localhost",
        user="pi",
        password="",
        database="env_monitor_db"
    )
    print("Connected to DB.")

    while True:
        raw_data = arduino.readline().decode('utf-8').strip()
        if raw_data.count(",") == 2:
            try:
                pot_value, motion, temperature = raw_data.split(",")
                cursor = dbconn.cursor()
                cursor.execute(
                    "INSERT INTO environment_log (potValue, motionDetected, temperature) VALUES (%s, %s, %s)",
                    (pot_value.strip(), motion.strip(), temperature.strip())
                )
                dbconn.commit()
                cursor.close()
                print(f"Logged: {raw_data}")
            except Exception as e:
                print(f"Parse/Insert error: {e}")
        time.sleep(1)

except pymysql.MySQLError as e:
    print(f"DB error: {e}")
finally:
    if dbconn:
        dbconn.close()
