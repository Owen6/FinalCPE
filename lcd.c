void displayLCD(float h, float t, int s){
	lcd.clear();
	if(s == 1){
		lcd.setCursor(0,0);
		lcd.print("IDLE");
	}
	if(s == 3){
		lcd.setCursor(0,0);
		lcd.print("Error:     ");
		lcd.setCursor(0,1);
		lcd.print("Water Level Low");
		return;
	}
	lcd.setCursor(0,0);
	lcd.print("HUMID: ");
	lcd.print('\t' + h);
	lcd.setCursor(0,1);
	lcd.print("TEMP: ");
	lcd.print('\t' + t);
	return;
}