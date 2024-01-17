//Create a game that's like the word list recall game.  15 words will be drawn at random from a list of 100 or so (currently 141).  When a word appears, it will then disappear.  This could be tricky to program,
//but it's worth looking up a function that lets a word appear on screen and then disappear.  Once 15 have appeared in this way, the game pauses until user tells it to continue.
//Continuing shows the full list of words that showed up, to see how many he was able to recall.  I'll include a note at the end that
//says that a 20 year old usually recalls 7 words, and an old person recalls 4.  User can then type y to play again or q to quit.




/*
BRAIN STORM

1. It should keep track of each word as a token.  When player guesses, the token will flag as correct or incorrect.
; A guess made by player can be in any order.

2. Number of correct guesses counter will be counted by looping through words and ticking up for every one flagged correct.
; A guess should match any word that is listed, but if it matches a word already flagged
as correct, then the guess will be wasted.  The number of correct words counter will not increase.

3. After this, words will display with a marker next to them
for each correct and incorrect word.

4. Player can then y/n for "play again?"


PSEUDO

vector with words available

play vector is empty, for pushing to during play

void clearscreen()
    prints blank space

string random_word()
    int num = rand%word_list.size()
    return word_list[num]

string checkrepeat(string randword)
    loops through words in play vector
        if word in loop is word that was returned from random_word
            recurse random_word again



GRAMMAR

displayword:
    checkrepeat

checkrepeat:
    randword


FIXES

1. "Is implicitly deleted because default definition is ill defined" - https://stackoverflow.com/questions/68490247/compilation-error-is-implicitly-deleted-because-the-default-definition-would-be
I can't remember how exactly I fixed this, but whatever.

2. Right now, it only marks the first word as correct and then it continually asks for user input beyond the 15 words.  It will notice when a word is repeated though.
Fixed with get().

3. When a repeat word is entered for user guess, it will print that it's a repeat 15 times.  I only want it to print once.
Fixed by making it return "(repeat entry)".  The problem was that it only print that it was repeat before without return, so the loop would keep going.
It always printed 15 times, because it only relied on check_repeat returning true, and check_repeat already looped through the full list after one go.

4. When typing "giveup" it concludes user guess inputs and lists correct words and user guesses, however it then throws a range error before arriving at score.
Fixed by replacing word_round with word_guesses in final loop of play_round.


BUGS

1. I can't enter words with spaces.  The one two word entry in the big list is "prince bishopric", but this will check for whether it's correct or not twice, due to the blankspace terminating a
cin in get(); not to mention that "prince" is itself a word.  I'm not sure how to get around this currently, unless there's a way to terminate cin with a new line only.  Even if only new line terminates,
there should be a limit to number of words, or in effect, number of spaces.
Perhaps mention to user that each entry has a maximum of two words, so that a second space character will also terminate cin?
I'll have to learn more about getline() and string streams first, if I want to make a more custom character termination.  Learning more about string streams is probably necessary for natural language
processing too.



TO DO

If I want to introduce ignore and clean_up_mess and other stuff, I should include it in a new version rather than same file.

2. It might be useful to find a way for player to "give up" if they're done guessing at words.  That way, they don't have to iterate through all 15 guesses before seeing the results.
They can give up, and view them immediately.
 */

#include "std_lib_facilities.h"
#include <ctime>
#include <chrono>
#include <thread>


vector<std::string> word_list = {"flower", "person", "stem", "water", "rest", "place", "alligator", "jeans", "jewel", "journey", "truck", "solid", "brick", "amber", "veranda", "cricket", "culture", "agreement",
"element", "morbidity", "compound", "function", "prolong", "galavant", "chivalric", "chore", "gumption", "yesterday", "tomorrow", "dumb", "potato", "sack", "archway", "temerity", "tactical", "sexy", "hominid",
"stare", "staple", "stimulate", "stammer", "ground", "blame", "bloated", "burst", "anarchic", "compare", "spar", "spin", "temperature", "clamour", "kitten", "entertain", "intrigue", "intern", "instant", "aspire",
"amputate", "ample", "flummox", "fit", "athletic", "mistake", "mentor", "member", "mark", "mustard", "ketchup", "limp", "lame", "lettuce", "learn", "lurk", "lustre", "finish", "frozen", "freedom", "frank",
"enrapture", "embroil", "empire", "broach", "dire", "dig", "dental", "deterrent", "deadly", "steam", "steep", "pig", "pristine", "pretty", "prepare", "preoccupy", "orange", "opera", "order", "orient", "hope",
"howel", "shovel", "harness", "house", "joke", "jump", "jowels", "near", "nag", "night", "naughty", "quiz", "quandary", "quarter", "rotunda", "rope", "rusty", "rent", "undue", "underestimate", "understand",
"undertaker", "impolite", "vibrant", "vixen", "variable", "versatile", "wild", "whimper", "wear", "want", "winter", "spring", "summer", "autumn", "fall", "xylophone", "zombie", "yield", "yet", "zebra", "generation",
"mystery", "kingdom", "duchy", "earldom", "county", "estate", "manor", "viceroyalty", "royalty", "right", "left", "governorship", "bishop", "prince", "bishopric", "kick", "punch", "throw", "chaos", "hovel", "flexible",
"gymnast", "dancer", "gamer", "player", "mate", "teamster", "toned", "firm", "upright", "stooped", "stiff", "guest", "gesture", "jig", "tilt", "splits", "pretzel", "experienced", "orgasm", "stamina", "tight", "happy",
"pleasant", "amenable", "melodic", "comely", "attractive", "depressed", "cuddle", "comfort", "pain", "able", "erect", "possible", "muscular", "earnest", "eager", "disciplined", "determined", "crocodile", "bird",
"hippopotamus", "cheetah", "rhinocerous", "lion", "body", "bikini", "lingerie", "model", "sporty", "princess", "queen", "duchess", "baron", "baroness", "countess", "empress", "sultan", "sultana", "caesar", "kaiser",
"tsar", "tsarina", "caliph", "pope", "peasant", "knight", "merchant", "sheikh", "emir", "leotard", "monokini", "cute", "violent", "violet", "flower", "rose", "thorn", "november", "april", "blush", "brazen", "begin",
"print"};
//^ big list of words that serve as the game's full data base

class wordcorrect  //for marking whether a guess from user is correct or not
{
public:
    std::string cor;  //correct or incorrect
    std::string wstr;  //word

    wordcorrect (std::string w, std::string c) :wstr(w), cor(c) {}
};


vector<std::string> word_round;  //randomly push up to 15 words here for display during play

vector <wordcorrect> word_guesses;  //guesses that user user made, marked correct or incorrect

void clearscreen()  //clears the screen so that user can only view one word at a time
{
    cout << string(30, '\n');
}

bool checkrepeat(std::string wordstr, bool userguess);

std::string random_word()  //draws random word from big word list, if it's a repeat of what's already there, it will do this again until there's no repeat
{
    while (true)
    {
    int i = rand()%(word_list.size());
    bool check = checkrepeat(word_list[i], false);
    //std::cout << "random_word called" << std::endl; //print test

    if (check == false)  //false is desirable, it means no repeats and word can get pushed
        return word_list[i];  //show random word from list
    }
}

bool checkrepeat(std::string wordstr, bool userguess)  //return true if a random word is a repeat of what's already in the word_round or word_guesses list
{
    if (userguess == false)  //false here means that word is not a userguess but a random word being pushed into word_round list, for set up
    {
        for (int i = 0; i < word_round.size(); i++)
        {
            if (word_round[i] == wordstr)
                return true;
        }
        return false;
    }

    if (userguess == true)  //this is if word is being compared to a word that user has guessed, as checkrepeat can be used for either purpose
    {
        for (int i = 0; i < word_guesses.size(); i++)
        {
            if (word_guesses[i].wstr == wordstr)
                return true;
        }
        return false;
    }
}

void word_round_push()  //pushes 15 random words to word_round list
{
    //std::string randword = random_word();
    for (int i = 0; i < 15; i++)
        word_round.push_back(random_word());
}

std::string display_word()  //populates word_round list and then displays a word every 2 seconds and clearing screen, repeating for 15 words
{
    //std::cout << "calling display_word()" << std::endl;  //print test
    word_round_push();

    for (int i = 0; i < word_round.size(); i++)
    {
        std::cout << word_round[i];
        std::this_thread::sleep_for(2s);  //proper time is 2s, sped up for testing
        clearscreen();
    }
}


class wordstream  //for accepting input from user and handling it
{
public:
    bool full = false;
    wordcorrect buffer;

    wordstream() : full(0), buffer("0", "0") { }
    wordcorrect get();
    void unget(wordcorrect wc);  //ungets a wordcorrect object
    void ignore(std::string s);  //ignores a string
};

wordstream wordstr;  //creating a wordstream object globally here


//void wordstream::ignore(std::string s) //returns as void, so that it flushes out user input.  Only meant to be called after an error via clean_up_mess
//{
//    if (full && s == buffer.wstr) {
//		full = false;
//		return;
//	}
//	full = false;
//
//	std::string str;
//	while (cin >> str)
//		if (str == s) return;
//}
//^commented out the above, as user can enter anything as a word, so there's no reason for any errors or flushing due to them

wordcorrect wordstream::get()  //takes user input to return as correct or incorrect
{
    if (full) {full = false; return buffer; }

    std::string str;

    std::cin >> str;

    bool repeat = checkrepeat(str, true);  //call only once because it only cins str once.  true means that it's checking the userguess vector rather than the random word vector

    if (str == "giveup")
        return wordcorrect(str, "(gave up)");

    for (int i = 0; i < word_round.size(); i++)  //
    {
        //std::cout << "iterating through word_round in get()" << std::endl;  //print test
        if (word_round[i] == str && repeat == false)  //check_repeat is given true arg, so that it will check if it was same str as before, so that user doesn't do repeat guesses
            return wordcorrect(str, "(correct)");
        else if (repeat == true)
            return wordcorrect(str, "(repeat entry)");
    }

    return wordcorrect(str, "(wrong)");
}

void wordstream::unget(wordcorrect wc)  //puts user input back in buffer for future handling if necessary
{
    if (full) error("unget into full buffer");
    buffer = wc;
    full = true;
}

void play_round()  //displays words one after the other and then user has remember and guess which words were displayed
{
    while(true)
    {
    display_word();

    std::cout << "Which words showed up?  15 tries to guess all 15.  Type \"giveup\" if you can't remember and would prefer to skip to the end." << std::endl;

    for (int i = 0; i < word_round.size(); i++)  //user guesses 15 times
    {
        wordcorrect wc = wordstr.get();
        word_guesses.push_back(wc);
        std::cout << wc.cor << "\n" << std::endl;
        if (wc.cor == "(gave up)")
            i = word_round.size();  //break out of loop in a way that breaks all further loops
     }

    std::cout << "----- \nThe words were... " << std::endl;
    for(int i = 0; i < word_round.size(); i++)  //words that were randomly displayed listed again
    {
        std::cout << word_round[i] << " ";
        if ((i+1)%3 == 0)
            std::cout << "\n";  //every 3 words demands a new line for readability
    }
    std::cout << "\nUser guessed..." << std::endl;
    int k = 0;
    for(int i = 0; i < word_guesses.size(); i++)  //words that user guessed are displayed as correct or incorrect
    {
        std::cout << word_guesses[i].wstr << " " << word_guesses[i].cor << " | ";
        if ((i+1)%3 == 0)
            std::cout << "\n";
        if (word_guesses[i].cor == "(correct)")
            k++;
    }

    std::cout << "\n" << k << " out of 15 guesses were correct." << std::endl;
    std::cout << "The above words were taken from a " << word_list.size() << " word list.  \nTypically, a 90 year old might remember 4 words, while a 20 year old remembers 7.  \nDon't take these results too seriously.  If you are worried, contact a professional.";
    word_round.clear();
    word_guesses.clear();
    break;
    }
}

void clean_up_mess()  //clears out buffer, but if calling ignore, it would also clear out user input stream
{
//	wordstr.ignore();
    wordstr.buffer = {"0", "0"};
    wordstr.full = false;
}

int main()
{
    srand(time(0));

    char play;

    while (true) try
    {
        play_round();
        std::cout << "\n\nplay again?  y/n ";
        std::cin >> play;
        if (play != 'y')
            break;
    }
    catch (runtime_error& e) {
		cerr << e.what() << endl;
		clean_up_mess();
    }
}
