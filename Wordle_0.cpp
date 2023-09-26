/**
 * @file Wordle_0.cpp
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
using namespace std;

void getWords(string wordList[])
{
    wordList[5757]; // this is really inefficence because I'm just using the exact length of the textfile
    // thought: I think that we can define this later...  update: i'm investigating vectors
    // >> I think we could use a vector cause it is like a mutable array, wihtout being a array with nice functions attached
    // pushback() <- how we can append items
    
    string line; // so if you have a better way to do this lmk cuz there isn't an Array with functions
    ifstream myfile;

    myfile.open("sgb-words.txt" , ios::in); // this opens the file
    while(getline(myfile , line))
    {
        for (int n = 0; n < 5757; n++)
        {
            wordList[n] = line; // how do we know how many words there are, there isn't an add function for arrays so I can't
                                //change the size of the array by adding to it
        }
        
    }
    myfile.close();
}
void chooseTodaysWord(string theWord)
{
    // how do you chose a word daily, use a timer 
}

//FUNCTIONS:

// - read function into array - this is what I'm trying to do right now, we'll see if I'm doing it right 
// - select word
// - check letters
// - check if words have been used in the passed
// - check if word was already gussed today
// - check if word is in list
// - check check user answers