-- Create database
CREATE DATABASE env_db;
USE env_db;

CREATE TABLE sensor_data (
    id INT AUTO_INCREMENT PRIMARY KEY,
    timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    temperature FLOAT NOT NULL,
    led_status VARCHAR(10),
    fan_status VARCHAR(5)
);