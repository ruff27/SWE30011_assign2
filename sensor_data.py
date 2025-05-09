import serial
import pymysql

device = '/dev/ttyACM0'
arduino = serial.Serial(device, 9600)

try:
    dbconn = pymysql.connect(
        host="localhost",
        user="pi",
        password="",
        database="env_db"
    )
    print("Connected to Arduino on", device)
    print("Connected to database.")

    while True:
        data = arduino.readline().decode('utf-8').strip()

        if data.startswith("Temp:"):
            # Expected format: Temp: 25.42 °C | LED: GREEN | Fan: ON
            parts = data.split("|")
            temp = float(parts[0].split(":")[1].strip().split(" ")[0])
            led_status = parts[1].split(":")[1].strip()
            fan_status = parts[2].split(":")[1].strip()

            cursor = dbconn.cursor()
            cursor.execute(
                "INSERT INTO sensor_log (temperature, led_status, fan_status) VALUES (%s, %s, %s)",
                (temp, led_status, fan_status)
            )
            dbconn.commit()
            cursor.close()
            print(f"Inserted: Temp={temp}, LED={led_status}, Fan={fan_status}")

except pymysql.MySQLError as e:
    print("Database error:", e)

finally:
    if dbconn:
        dbconn.close()
