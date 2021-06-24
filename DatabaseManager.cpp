// 
// 
// 

#include "DatabaseManager.h"

void logToDatabase(int raw1, char* txt1, int raw2, char* txt2, int act, bool water) {
	MySQL_Connection conn((Client*)&client);
	Serial.println("SQL-Server connecting...");
	if (conn.connect(SQL_IP, SQL_PORT, SQL_USER, SQL_PASS)) {
		Serial.println("SQL-Server connection OK.");
		delay(1000);

		// build SQL query
		char query[256];
		char INSERT_DATA[] = "INSERT INTO %s.%s (Timestamp, Sensor1_raw, Sensor1_txt, Sensor2_raw, Sensor2_txt, WaterAmount, Waterlevel) VALUES ('%s',%d,'%s',%d,'%s',%d,%d)";
		char timeStr[20];
		sprintf(timeStr, "20%02d-%02d-%02d %02d:%02d:%02d", rtc.getYear(), rtc.getMonth(), rtc.getDay(), rtc.getHours(), rtc.getMinutes(), rtc.getSeconds());
		sprintf(query, INSERT_DATA, SQL_DB, SQL_TABLE, timeStr, raw1, txt1, raw2, txt2, act, water);

		// Initiate the query class instance
		MySQL_Cursor* cur_mem = new MySQL_Cursor(&conn);
		// Execute the query
		cur_mem->execute(query);
		delete cur_mem;
	}
	else {
		Serial.println("SQL-Server connection failed.");
	}
	conn.close();
}
