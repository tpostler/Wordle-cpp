/**
 * @file Wordle_0.1.cpp
 * @author Tori Postler & Abigail Ogden
 * @brief 
 * @version 0.1
 * @date 2022-03-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <string>
#include <string.h>
#include <vector>
#include <ctime>

using namespace std;

/* GLOBAL VARAIBLES */
vector<string> wordList;
vector<string> dateList;
vector<string> saveList;
bool DEBUG      = false;
bool DEBUG_WORD = false;
int guessLeft   = 6;
string todayWord;
string usrGuess = "";
string guesses[12];


/**
 * @brief read 5 word file
 * 
 */
void readFile()
{   
    string   line;   // holds the word that myFile is "reading"
    ifstream myfile; // obj that will read the file

    myfile.open("sgb-words.txt" , ios::in); // opens file

    while(myfile.peek() != EOF) { // need to do this so it doesn't read the last line twice! =)
        getline(myfile, line);    // reads the next line of file
        wordList.push_back(line); // adds to end of vector
        //cout << line << "\n";
    }

    if(DEBUG) {
        cout << "\n\n";
        for(vector<string>::iterator t=wordList.begin(); t!=wordList.end(); ++t) {
            cout << *t << "\n";
        }
        cout << "Size of vector: " << wordList.size() << "\n"; 
    }

    myfile.close();
}

/**
 * @brief 
 * chooses the word for the day
 */
void chooseTodaysWord()
{
    /* Generates random int for vector */
    srand((unsigned) time(0)); // so everytime there is a new random number generated
    int random = rand() % wordList.size(); // range: 0 - wordList.size()-1

    todayWord = wordList.at(random); // finds word at rando index

    if(DEBUG) {
        cout << "random word: " << todayWord << "\n";
        cout << random << "\n";
    }
    // NEED TO DO: how do you chose a word daily, use a timer 

}
/**
 * @brief 
 * takes previous list of dates played, adds in current date and save back to text file
 */
void recordPlayingDate()  
{                       
    time_t currTime;  
    tm * currTm;      
    char dateString[100]; // current time as a string

    time(&currTime);
    currTm = localtime(&currTime);
    strftime(dateString , 50 , "%F" , currTm);

    fstream myRecordFile; // which will be to get date.txt dates
    string wordSave;
    myRecordFile.open("date.txt" , ios::in);
    while(myRecordFile.peek() != EOF) 
    { 
        getline(myRecordFile, wordSave);   
        saveList.push_back(wordSave);
    }
    saveList.push_back(dateString); // saves all past dates into vector
    myRecordFile.close();

    ofstream myRecordFile2;
    myRecordFile2.open("date.txt" , ios::out);
    for (vector<string>::iterator t=saveList.begin(); t!=saveList.end(); ++t) {
        string tt = *t;
        myRecordFile2 << tt << endl;
    }
    myRecordFile2.close();
    
}
/**
 * @brief 
 * takes full list of dates played in past and compares to todays date 
 * @return if played today true if not false
 */
bool checkIfPlayedToday()
{
    string date;
    time_t currTime;
    tm * currTm;
    char dateString[100];
    time(&currTime);
    currTm = localtime(&currTime);
    strftime(dateString , 50 , "%F" , currTm); // todays date

    ifstream myDateFile;
    myDateFile.open("date.txt" , ios::in);
    while(myDateFile.peek() != EOF) 
    { 
        getline(myDateFile, date);   
        dateList.push_back(date);
    }
    myDateFile.close(); // list of dates recorded in date.txt
    
    string newDate = dateString;
    int yesCounter = 0;
    for (vector<string>::iterator t=dateList.begin(); t!=dateList.end(); ++t) { // compares all dates to today
        string tt = *t;
        if (tt == newDate) {
            yesCounter += 1;
        }
    }
    if (yesCounter > 0) {
        return true;
    }  
    else {
        return false;
    }
}

/**
 * @brief 
 * inserts spaces between the letters of the guesses. I did this for readability.
 * There is definetly a better way of doing this =/
 * @param str : string to be spaced out
 * @return string : the spaced out string
 */
string insertSpaces(string str) {
    string frankenString = " ";   // starts with a space for readability. String to be contructed
    char letters[str.size()];     // array to hold all of the characters in str

    // loops through the str and gets each char and puts in list
    for(int i=0; i < str.size(); i++) {
        letters[i] = str[i];
    }

    // contructing the spaced out string
    for(int i=0; i< str.size(); i++ ) {
        frankenString += letters[i];
        frankenString += "  ";
    }

    if(DEBUG) {
        for(int i=0; i < str.size(); i++) {
            cout << "Array at index: i=" << i << ": " << letters[i] << "\n";
        }
    }
    return frankenString;
}

/**
 * @brief checkIfWord
 * goes through the whole 5-word list and checks that it qualifies
 * @param guess : usr guess to be checked
 * @return true : found in wordList, it is a word
 * @return false : not found in wordList, it is not a word
 */
bool checkIfWord(string guess) {
    // looping through the whole file and if guess is found then it will exit the loop
    for(int i=0; i < wordList.size(); i++){
        if (wordList.at(i) == guess)
            return true;
    }
    return false;
}

/**
 * @brief checkIfNum
 * checks if there is a number in the usr guess
 * @param str : string to be checked
 * @return true : there is a num in the str
 * @return false : there is not a num in the str
 */
bool checkIfNum(string str) {
    for (int i = 0; i < str.length(); i++) {
    if (isdigit(str[i]) == true)
        return true;
    }
    return false;
}

// return string with corresponding emojis
// ✅, ⚠️ ❌
/**
 * @brief checkLetters
 * THERE IS A BUG IN YELLOW!!!
 * Goes through all of the characters and then will compare to the same char at today's
 * word's index. If letter in the right place then: ✅ If the letter is in the word but
 * in the wrond place: 🌝 If the letter isn't in the word: ❌
 * @param guess : the str that usr guesses, being compared against todayWord
 * @param guessNum 
 */
void checkLetters(string guess, int guessNum) {
    /* Looping through each char */
    for (int i=0; i < 5; i++) {
        // letter in correct place
        if(todayWord[i] == guess[i]) {
            guesses[guessNum] += " 🟩";
            //cout << "reached green";
        }
        // letter in wrong place BUGGY
        else if(todayWord.find(guess[i]) != std::string::npos) {
            guesses[guessNum] +=  " 🟨";
            //cout << "reached yellow\n";
        }
        // letter not in word
        else {
            guesses[guessNum] += " ⬛️";
            //cout << "reached red";
        }
    }
}

/**
 * @brief printGuesses
 * will print the previous guesses, won't print if there are no current guesses
 */
void printGuesses() {
    // if there are no guesses then it will pass over
    if (guesses[0].empty()) {
        return;
    }
    else {
        cout << "\n---------------------\n\nCurrent Guesses:\n";
        for(int i=0; i < 12; i++) {
            // making sure it will only guesses, not the extra blank space
            if(!guesses[i].empty()) {
                cout << guesses[i] << "\n";
                // if there are 2+ guesses then it seperate with lines 
                if(i % 2 != 0 && !guesses[i+1].empty()) {
                    cout << "\n- - - - - - - -\n";
                }
            }
        }
        cout << "\n---------------------\n\n";
    }
}
/**
 * @brief getUsrGuess
 * takes the user's guess then also handles calling the check letter function 
 * and makes sures the valid guesses goes into the guess list.
 * 
 */
void getUsrGuess() {
    bool valid = false;
    /* will continue to loop through until there is a valid guess */
    while(!valid) {
        cout << "Please type your guess below:  "<< "_ _ _ _ _" << "\n";
        cout << ": ";
        getline(cin, usrGuess); // get user guess

        transform(usrGuess.begin(), usrGuess.end(), usrGuess.begin(), ::tolower); // changes it to all lowercase
        /* checking if it is a valid guess */
        // 5 letters in the guess
        if(usrGuess.length() != 5) {
            cout << "Must be 5 letters!\n\n";
            printGuesses();
            valid = false;
        }
        // no numbers in guess
        else if(checkIfNum(usrGuess)) {
            cout << "Only letters!\n\n";
            printGuesses();
            valid = false;
        }
        // real word not made up
        else if(!checkIfWord(usrGuess) ) {
            cout << "Not a word!\n\n";
            printGuesses();
            valid = false;
        }
        else 
            valid = true;
    }

    checkLetters(usrGuess, (12 - (guessLeft * 2) + 1));
    guesses[12 - (guessLeft * 2)] = insertSpaces(usrGuess);

    if (DEBUG) {
        cout << "index: " << (12 - (guessLeft * 2) + 1) << "\n";
        cout << "guessLeft: " << guessLeft << "\n";
        cout << "1st: " << 12 - (guessLeft * 2) << "\n";
        cout << "2nd: " << 12 - (guessLeft * 2) + 1 << "\n";
        cout << "Post insert space function: " << insertSpaces(usrGuess) << "\n";
    }

    //cout << feedback << "\n\n";
    printGuesses();
    if(guesses[(12 - (guessLeft * 2) + 1)] == " 🟩 🟩 🟩 🟩 🟩") {
        valid = true;
        return;
    }
    
    guessLeft -= 1;

    if(DEBUG_WORD) {
        cout << "You guessed: " << usrGuess << "\n";
    }
}

/**
 * @brief 
 * runner of the file
 * @return int 
 */
int main() 
{
    readFile();
    chooseTodaysWord();
    if(DEBUG_WORD) {
        cout << todayWord << "\n";
    }
    if (checkIfPlayedToday() == false)
    {
        recordPlayingDate();
        cout << "\n\n";
        while(guessLeft > 0) {
            getUsrGuess();
            if(guesses[(12 - (guessLeft * 2) + 1)] == " 🟩 🟩 🟩 🟩 🟩") {
                break;
            }
        }
        if (guesses[(12 - (guessLeft * 2) + 1)] == " 🟩 🟩 🟩 🟩 🟩") {
            cout << "congrats you got it";
        }
        else {
            cout << "Seems like you couldn't get it";
            cout << "\n\nTodays word: " << todayWord << "\n";
        }
    }
    else{
        cout << "\nSorry you already played today, come back tomorrow!" << endl;
    }
    

    //FUNCTIONS:

    // ✅ - read function into array 
    // ✅ - select word
    // ✅- check letters
    // ✅ - check if words have been used in the passed
    // ✅ check if word was already gussed today
    // ✅ - check if word is in list 
    // ✅ - check check user answers

    // Note: do we have anything to allow for capital letters? 
    return 0;
}
