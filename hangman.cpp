//Sneaky hangman game that narrows down wordlist so that user needs a large number of guesses
//@author  Conor Hughes

#include "hangman.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream> 
#include <list>
#include <time.h>
#include <algorithm>
#include <map>

using std::cout;
using namespace std;

int wordLength;
int numGuess;
int largest;
char userGuess;
char choice;
char again;
bool debug = false;
vector<string> wordlist;
vector<char> used;
map< string, vector<string> > sneakylist;
string displayWord;
string sneakyWord;
string theword;
string pattern;
string usedLetters = " ";

void hangman::start()//starts the game and checks if user wants to run debug mode (Shows how word is chosen and possible words)
{
	cout <<"Welcome To My Evil Hangman!" << endl << "_________" << endl << "|       |" << endl << "|       O" << endl << "|      /|\\ " << endl << "|      / \\ " << endl << "|" << endl << endl;
	cout << "Would You Like To Run The Game In Debug Mode? (y/n)" << endl;
	cin >> choice;
	if(choice == 'y')
	{
		debug = true;
	}
	picklength();
}

void hangman::picklength()//asks the users for the word length they want (dictionary contains 2-29 but 3-12 have a larger wordlist)
{
	cout << "What Word Length Do You Want? (3-12)" << endl;
	cin >> wordLength;
	//fill words with dashes
	displayWord.assign(wordLength,'-');
	sneakyWord.assign(wordLength,'-');
	pattern.assign(wordLength,'-');
	pickguesses();
}

void hangman::pickguesses()//asks the users for how many guesses they want
{
	cout << "How Many Guesses Do You Want? (1-26)" << endl;
	cin >> numGuess;
	cout << endl << endl;
	readfile();
}

void hangman::readfile()//reads in the dictionary file, only words of the selected length are added to the word list
{
	std::ifstream infile("dictionary.txt");
	std::string word;
	
	while (std::getline(infile, word))
	{
		int templength = word.length();
		
		if(templength == wordLength)
		{
			wordlist.push_back(word);
		}
	}
	
	display();
}

void hangman::display()//display the word and the guessed letters
{
	cout << "The Word: " << displayWord << endl;
	guessed();
	input();
}

void hangman::guessed()// function to display the letters already guessed
{
	cout << "Guesses Left: " << numGuess << endl;
	cout << "Letters Guessed: ";
	int tempsize = used.size();
	for(int i = 0; i < tempsize; i++)
	{
		cout << used[i] << " ";
	}
	cout << endl;
}

void hangman::input()//asks the users for a guess, loops until unguessed letter is chosen
{
	cout << "Enter Your Guess: " << endl;
	cin >> userGuess;
	
	if(usedLetters.find(userGuess)!=std::string::npos)//checks if guess is already contained within string usedLetters
	{
		input();
	}
	
	usedLetters.append(1, userGuess);
	used.push_back(userGuess);
	numGuess--;
	
	sneaky();
}

void hangman::changedisplay()//changes the display word if a letter is correctly guessed
{
	if(pattern.find(userGuess)!=std::string::npos)
	{
		cout << "Correct!" << endl << endl;
		for(int m = 0; m < wordLength; m++)
		{
			if(pattern.at(m) == userGuess)
			{
				displayWord.replace(m,1,1,userGuess);//replaces dashes with correctly guessed letter
			}
		}
		check();
	}
	
	else
	{
		cout << "Incorrect!" << endl << endl;
		check();
	}
}

void hangman::sneaky()//creates a map with a string key (different patterns possible with guessed letter) and string vector value (all words that match that pattern)
{
	int listsize = wordlist.size();
	sneakyWord = displayWord;
	
	for(int k = 0; k < listsize; k++)
	{
		for(int l = 0; l < wordLength; l++)
		{
			if(wordlist[k].at(l) == userGuess)
			{
				sneakyWord.replace(l,1,1,userGuess);//creates key pattern for each word
			}
		}
		sneakylist[sneakyWord].push_back(wordlist[k]);//adds to map
		sneakyWord = displayWord;
	}
	newlist();
}

void hangman::newlist()//finds the key and value with the most words and puts it into the wordlist
{
	largest = -1;
	wordlist.clear();//empty vector for new wordlist
	for (std::map<string, vector<string> >::iterator it=sneakylist.begin(); it!=sneakylist.end(); ++it)
	{
		int tempSize = it->second.size();
		
		if(tempSize > largest)
		{
			largest = tempSize;//find pattern with largest vector size
			pattern = it->first;
		}
		
		if(debug)
		{
			cout << "The Pattern: " << it->first << endl << "The Size: " << it->second.size() << endl;
		}
	}
	
	if(debug)
	{
		cout << "The Most Common Pattern: " << pattern << endl << "Number of possible words: " << largest << endl;
	}

	for (std::map<string, vector<string> >::iterator it=sneakylist.begin(); it!=sneakylist.end(); ++it)
	{
		if(it->first == pattern)
		{
			it->second.swap(wordlist);//new wordlist created
		}
	}

	sneakylist.clear();//empty map for next guess
	changedisplay();
}

void hangman::check()//checks if the word is correctly guessed, the user has ran out of guesses or whether to get another input.... Also allows user to play again
{
	if(numGuess <= 0)
	{
		//prints a random word from the wordlist
		srand ( (unsigned int)time(NULL) );
		theword = wordlist[rand()% wordlist.size()];
		cout << "You Ran Out Of Guesses!" << endl << "The Word Was: " << theword << endl << endl << "Would You Like To Play Again? (y/n)" << endl;
		cin >> again;
		
		if(again == 'y')
		{
			cout << "Great! Lets Play Again!" << endl << endl;
			start();
		}
		else
		{
			cout << "Thanks For Playing My Evil hangman Game!";
			exit(EXIT_SUCCESS);
		}
	}
	
	else if(displayWord.find('-')!=std::string::npos)//asks for another input
	{
		display();
	}
	
	else//user guessed the word correctly
	{
		cout << "You Win!!" << endl << "The Word Was: " << displayWord << endl << endl << "Would You Like To Play Again? (y/n)" << endl;
		cin >> again;
		
		if(again == 'y')
		{
			cout << "Great! Lets Play Again!" << endl << endl;
			start();
		}
		else
		{
			cout << "Thanks For Playing My Evil Hangman Game!";
			exit(EXIT_SUCCESS);
		}
	}
}