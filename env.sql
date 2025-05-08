-- Create database
CREATE DATABASE IF NOT EXISTS env_monitor_db;
USE env_monitor_db;

-- Create table
CREATE TABLE IF NOT EXISTS environment_log (
  id INT AUTO_INCREMENT PRIMARY KEY,
  potValue VARCHAR(50) NOT NULL,
  motionDetected BOOLEAN NOT NULL,
  temperature VARCHAR(50),
  created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
