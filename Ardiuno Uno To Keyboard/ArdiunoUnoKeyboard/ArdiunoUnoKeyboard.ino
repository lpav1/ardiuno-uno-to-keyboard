class unoKeyboard{
    private:
      uint8_t buf[8] = { 0 };
      
    public:
   
      updatedKeyboard(){}
  
      void print(String message){
          for(int i = 0; i<message.length(); i++){           
               
              int character = getHex(message[i]);           
              int eKey = shiftNeeded(message[i]);
              buf[0] = eKey;
              buf[2] = character;            
              Serial.write(buf, 8);
              releaseKey();
          }
      }
      
      void print(String message, int delays){
          for(int i = 0; i<message.length(); i++){           
               
              int character = getHex(message[i]);           
              int eKey = shiftNeeded(message[i]);
              buf[0] = eKey;
              buf[2] = character;            
              Serial.write(buf, 8);
              releaseKey();
              delay(delays);
          }
      }
      
  
      int shiftNeeded(char character){
  
        int dataSetSize;
        String dataSets[dataSetSize];
        int midLength;
        int placeInSet;
        
        if ((isUpperCase(character)) || (character == '"')){
          return 2;
        }
        else if (isLowerCase(character)){
          return 0;
        }
        else if(isWhitespace(character)){
          return 0;
        }
             
        else{
          int dataSetSize = 2;
          String dataSets[dataSetSize] = {"1234567890!@#$%^&*()", "-=[]\\ ;'`,./_+{}|~:  <>?"};
         
          String setToIndex;
          for(int set = 0; set<dataSetSize ; set+=1){
             setToIndex = dataSets[set];
             placeInSet = setToIndex.indexOf(character);
            
             if(placeInSet > -1){
               
                midLength = setToIndex.length()/2;
                break;
             }
          }
       }
      
       if(placeInSet >= midLength){
          return 2;
       }
       else{
          return 0;
       }
      }
      
      int getHex(char character){
        character = toLowerCase(character);
        
        if(character == '\\'){
          return 0x31;
        }
        if(character == '"'){
          return 0x34;
        }
        
        String dataSet;
        int offset;
        int midLength;
        
        if(isWhitespace(character)){
              return 44;
        }
        else{
          int dataSetSize = 3;                                                                                                 
          String dataSets[dataSetSize] = {"abcdefghijklmnopqrstuvwxyz", "1234567890!@#$%^&*()", "-=[]NN;'`,./_+{}|N:N~<>?"};
          int offsets[dataSetSize] = {4, 30, 45};
          String setToIndex;
          
          for(int set = 0; set<dataSetSize ; set+=1){
             setToIndex = dataSets[set];
             if(setToIndex.indexOf(character) > -1){
                dataSet = setToIndex;
                offset = offsets[set];
                midLength = dataSet.length()/2;
             }
          }
       }
       
       if(offset<30){
           return dataSet.indexOf(character) + offset;     
       }
       else{
           return (dataSet.indexOf(character)%midLength) + offset;
       }
      }
  
      void enter(){
        buf[0] = 0;
        buf[2] = 40;
        Serial.write(buf, 8);
        releaseKey();
      }
  
      void ctrlPrint(char character){
        buf[0] = 1;
        buf[2] = getHex(character);
        Serial.write(buf, 8);
        releaseKey();
      }
      
      void shiftPrint(char character){
        buf[0] = 2;
        buf[2] = getHex(character);
        Serial.write(buf, 8);
        releaseKey();
      }

      void windowsPrint(){
        buf[0] = 8;
        buf[2] = 0;
        Serial.write(buf, 8);
        releaseKey();
      }
      
      void windowsPrint(char character){
        buf[0] = 8;
        buf[2] = getHex(character);
        Serial.write(buf, 8);
        releaseKey();
      }

      void releaseKey(){
        buf[0] = 0;
        buf[2] = 0;
        Serial.write(buf, 8);
      }
      
};

unoKeyboard uk = unoKeyboard();

void setup(){
   /*    EXAMPLE    */  

    Serial.begin(9600);
    
    uk.windowsPrint();
    delay(500);
    
    int delays = 500;
    uk.print("notepad", delays);
    
    uk.enter();
    delay(500);
    
    uk.print("Hello World!");

}
void loop() {}
