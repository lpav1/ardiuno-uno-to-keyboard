/*
Very intro level program to make the Ardiuno Uno 
into a keystroke injector, written with comments
*/

//creates a class for better management and abilty to create multiple keyboards
class unoKeyboard{
    private:
      //initalize an array of bits, making a byte which will be returned with Serial.write();
      uint8_t buf[8] = { 0 };

    //all methods
    public:

      //create an instance
      updatedKeyboard(){}

      //types a given string 
      void print(String message){
          //loops over each character
          for(int i = 0; i<message.length(); i++){           
              //gets key-> usb hex code and if it needs a shift
              int character = getHex(message[i]);           
              int eKey = shiftNeeded(message[i]);

              //sets bits to the code and writes the byte to the computer
              buf[0] = eKey;
              buf[2] = character;
                          
              Serial.write(buf, 8);
              //reset buf to act like nothing is being pressed
              releaseKey();
          }
      }

      /*
      same logic as previous, but with delays inbetween letters
      sometimes the prints can move too fast, causing them to misprint,
      delays can also help to easily track your programs
      */
      void print(String message, int delays){
          for(int i = 0; i<message.length(); i++){           
  
              int character = getHex(message[i]);  
              //ekey short for extra key         
              int eKey = shiftNeeded(message[i]);
              
              buf[0] = eKey;
              buf[2] = character;  
                        
              Serial.write(buf, 8);
              releaseKey();
              
              //call the given delay
              delay(delays);
          }
      }
      
      //this determines if the print needs a shift key added to it, like a->A or [->{
      int shiftNeeded(char character){

        //sets variables from start to calculate if the placement is greater or less than the middle of their respective data set
        int dataSetSize;
        String dataSets[dataSetSize];
        int midLength;
        int placeInSet;

        //clears out any lowercase alphabet letters and characters than cannot be included in strings (" will break a string literal in ardiuno code)
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
          //loops through a array of remaining keys
          int dataSetSize = 2;
          //keys are grouped by similar groups as usb code groups (referenced in usb code file)
          String dataSets[dataSetSize] = {"1234567890!@#$%^&*()", "-=[]\\ ;'`,./_+{}|~:  <>?"};
         
          String setToIndex;
          for(int set = 0; set<dataSetSize ; set+=1){
             //finds the placement of the key
             setToIndex = dataSets[set];
             placeInSet = setToIndex.indexOf(character);

             //if it is in the data set
             if(placeInSet > -1){
                //makes the middle length for calculations later
                midLength = setToIndex.length()/2;
                break;
             }
          }
       }

       //is the place resides over half the length of the data set, it is a shifted key
       if(placeInSet >= midLength){
          return 2;
       }
       else{
          return 0;
       }
      }

      //returns the given char to its usb hex code
      int getHex(char character){
        //we can already figure out if it needs to be shifted, so we can set all characters to any capitalization
        character = toLowerCase(character);

        //first tests for characters that cannot be included in strings 
        if(character == '\\'){
          return 0x31;
        }
        if(character == '"'){
          return 0x34;
        }

        //similar logic to getHex(); but with more specific calculations (explained below)
        String dataSet;
        int offset;
        int midLength;
        
        if(isWhitespace(character)){
              return 44;
        }
        else{
          //makes an array of the special keys
          int dataSetSize = 3;                                                                      
          //use 'N' as a placeholder for characters we already checked before                           
          String dataSets[dataSetSize] = {"abcdefghijklmnopqrstuvwxyz", "1234567890!@#$%^&*()", "-=[]NN;'`,./_+{}|N:N~<>?"};
          //array to include the charcters hex code offsets from 0 ("abcd..." has an offset of 4 because 'a' starts at 0x04 (4) )
          int offsets[dataSetSize] = {4, 30, 45};
          String setToIndex;

          //loops through each data set to see if it contains the character
          for(int set = 0; set<dataSetSize ; set+=1){
             setToIndex = dataSets[set];
             if(setToIndex.indexOf(character) > -1){
                //sets the variables at first to the current data set attributes
                dataSet = setToIndex;
                midLength = dataSet.length()/2;

                //each data set has a offset, this is added to the fianl result
                offset = offsets[set]; 
             }
          }
       }
       /*
       if the index of the character occurs before the middle of the array, then that means
       the character is in the "first" line of data, meaning data without a shift (12345... -=[]...)
       else that menas the character relapsed the data since each key has another value when adding a shift 
       ex: 12345.. -> !@#$%..
        */
       if(offset<30){
           return dataSet.indexOf(character) + offset;     
       }
       else{
           //letters with both a regular and shifted value share the same code (buf[0] = _ makes it shifted or not)
           //we can use math to get its base key (!'s base key is 1)
           return (dataSet.indexOf(character)%midLength) + offset;
       }
      }

      //writes the enter key to the computer
      void enter(){
        buf[0] = 0;
        buf[2] = 40;
        Serial.write(buf, 8);
        releaseKey();
      }

      //writes the ctrl key to the computer with a char (useful for hotkeys like ctrl+z for undo)
      void ctrlPrint(char character){
        buf[0] = 1;
        buf[2] = getHex(character);
        Serial.write(buf, 8);
        releaseKey();
      }

      //shifts a single character (also for hotkeys like 
      void shiftPrint(char character){
        buf[0] = 2;
        buf[2] = getHex(character);
        Serial.write(buf, 8);
        releaseKey();
      }

      //writes the standalone windows key to the computer (opens the quick search)
      void windowsPrint(){
        buf[0] = 8;
        buf[2] = 0;
        Serial.write(buf, 8);
        releaseKey();
      }

      //writes the enter code to the computer with a character (hotkeys seen with win+r for run)
      void windowsPrint(char character){
        buf[0] = 8;
        buf[2] = getHex(character);
        Serial.write(buf, 8);
        releaseKey();
      }

      //resets the byte to act as no key presses
      void releaseKey(){
        buf[0] = 0;
        buf[2] = 0;
        Serial.write(buf, 8);
      }
      
};

//creates uk (uno Keyboard) object
unoKeyboard uk = unoKeyboard();

void setup(){
   /*    EXAMPLE    */  

    //begins serial, this is necessary since without it, Serial.write(); cant happen
    Serial.begin(9600);

    //open the quick search with windowsPrint(); with no parameters
    uk.windowsPrint();

    /*
    delays so we can see the program in action, without
    it the program runs instantaneously
    */
    delay(500);

    //example of the print method with pauses inbetween keys
    int delays = 500;
    uk.print("notepad", delays);

    //presses enter with a pause so that it allows for notepad to open
    uk.enter();
    delay(500);

    //print method without any pauses to see the speed of the printing
    uk.print("Hello World!");

}

void loop() {}
