void buttonPress(){
	button = digitalRead(2);
	if(button == HIGH && buttonStore == LOW && millis()-time > debounce){
		if(state){
			state = 0;
		}else{
			state = 1;
		}
		time = millis();
	}
	buttonStore = button;
}