int dosang;
int timer_red;
String error_Input = "Sai cú pháp. Nhập lại:";
void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  Serial.println("Nhập: ");
  Serial.print("Cú pháp  B_[0-100] ");
  Serial.println("  /Tùy chọn độ sáng.");
  Serial.println("Ví dụ: B_60 (Độ sáng 60%)"); 
  Serial.print("Cú pháp  R_[3-100] ");
  Serial.println("  /Tùy chọn thời gian hiển thị đèn đỏ.");
  Serial.println("Ví dụ: R_50 (Thời gian hiển thị đèn đỏ 50s)");
  Serial.println("");
}

void loop() {
  // put your main code here, to run repeatedly:

  char c = "";
  String strInput;
  String numString;
  int checknum=0;
  
  
  if(Serial.available()){
    delay(100);
    strInput = Serial.readString();
    
  } 
  
  if (strInput.length() > 0) {
    Serial.println(strInput);
    c = strInput[0];
    if(c == 'B') {
        if(strInput[1] != '_') {
          Serial.println(error_Input);
        } else {
          int i = 2;
          while(strInput[i] != '\0') {
            numString += strInput[i];
            i++;
          }
          i=0;
          int check = numString.length();
          int j=0;
          while(i < (check - 2)) {
            if(numString[i] != '0' && numString[i] != '1' &&
            numString[i] != '2' && numString[i] != '6' &&
            numString[i] != '3' && numString[i] != '7' &&
            numString[i] != '4' && numString[i] != '8' &&
            numString[i] != '5' && numString[i] != '9') {
              j=1;
              break;
            }
            i++;
          }
          
          dosang = numString.toInt();
          if( dosang > 101 || dosang < 0 || j == 1 ) {
            dosang = 0;
            Serial.println(error_Input);
          } else {
            Serial.print("Độ sáng được thiết lập: ");
            Serial.print(dosang);
            Serial.println("%");
          }
        }
    }
    else if(c == 'R') {
      if(strInput[1] != '_') {
          Serial.println(error_Input);
        } else {
          int i = 2;
          while(strInput[i] != '\0') {
            numString += strInput[i];
            i++;
          }
          i=0;
          int check = numString.length();
          int j=0;
          while(i < (check - 2)) {
            if(numString[i] != '0' && numString[i] != '1' &&
            numString[i] != '2' && numString[i] != '6' &&
            numString[i] != '3' && numString[i] != '7' &&
            numString[i] != '4' && numString[i] != '8' &&
            numString[i] != '5' && numString[i] != '9') {
              j=1;
              break;
            }
            i++;
          }
          
          timer_red = numString.toInt();
          if( timer_red > 101 || timer_red < 3 || j == 1 ) {
            timer_red = 0;
            Serial.println(error_Input);
          } else {
            Serial.print("Thời gian đèn đỏ: ");
            Serial.print(timer_red);
            Serial.println("s");
          }
        }
    }
    else {
      Serial.println(error_Input);
    }
    strInput = "";
  }
}
