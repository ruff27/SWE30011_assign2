import serial
import pymysql

device = '/dev/ttyACM0'
arduino = serial.Serial(device, 9600)

dbconn = None

try:
    dbconn = pymysql.connect(host="localhost", user="root", password="your_password", database="env_db")
    cursor = dbconn.cursor()
    print("Connected to database.")
    print("Connected to Arduino. Reading data...")

    while True:
        data = arduino.readline().decode('utf-8').strip()
        if data:
            print(data)
            parts = data.split('|')
            temperature = float(parts[0].split(':')[1].replace('°C','').strip())
            potentiometer = int(parts[1].split(':')[1].strip())
            button_state = parts[2].split(':')[1].strip()

            cursor.execute(
                "INSERT INTO sensor_data (temperature, potentiometer, button_state) VALUES (%s, %s, %s)",
                (temperature, potentiometer, button_state)
            )
            dbconn.commit()
            cursor.close()
            cursor = dbconn.cursor()  # reopen cursor for next loop

except pymysql.MySQLError as e:
    print(f"Database error: {e}")

except serial.SerialException as e:
    print(f"Serial connection error: {e}")

except Exception as e:
    print(f"Unexpected error: {e}")

finally:
    if dbconn:
        dbconn.close()
        print("Database connection closed.")
    if arduino.is_open:
        arduino.close()
        print("Serial connection closed.")
