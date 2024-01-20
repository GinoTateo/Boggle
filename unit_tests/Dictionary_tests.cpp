//
// Created by Varick Erickson on 2/24/2021.
//
#include "catch.hpp"
#include <fstream>
#include <vector>
#include <stdlib.h>
#include "../Dictionary.h"
#include "unit_util_dictionary.h"


TEST_CASE("Dictionary Tests") {
    SECTION("Default Constructor - 2pt") {
        Dictionary dict;

        // Should be 0
        REQUIRE(dict.WordCount() == 0);
    }

    SECTION("Constructor with file - 3pt") {
        int numChecks = 100;
        string dictName = "dictionary.txt";
        Dictionary dict(dictName);

        // Should be 0
        ifstream dictFile;
        dictFile.open(dictName);
        if (dictFile.fail()) {
            INFO(dictName << " did not open.");
            INFO("");
            INFO("Possible causes:");
            INFO("$PROJECT_DIR$ was not set in the run configuration.");
            INFO("File was overwritten by accident and is now empty.");
            INFO("File was moved.");
            FAIL();
        }

        vector<string> words = read_words(dictName);

        // Make sure all the words are in the dictionary
        for (int i = 0; i < words.size(); i++) {
            REQUIRE(dict.IsWord(words[i]));
        }

        // This is not a very extensive test.
        // Passing this test does not mean your addWord or IsWord works.
    }

    SECTION("AddWord and IsWord test - 8 pts") {
        int numChecks = 1000;
        string word;
        string dictName = "dictionary.txt";

        // Loads words from dictionary into a vector
        vector<string> words = read_words(dictName);

        // Loads pokemon from file into a vector
        vector<string> pokemon = read_words("test_data/pokemon.txt");

        // Manually add words to dictionary
        Dictionary dict;
        for (int i = 0; i < words.size(); i++) {
            dict.AddWord(words[i]);
        }

        // Check the empty string
        REQUIRE(!dict.IsWord(""));  // root->isWord should be false

        // Make sure all the words are in the dictionary
        for (int i = 0; i < words.size(); i++) {
//            REQUIRE(dict.IsWord(words[i]));
            if(!dict.IsWord(words[i])) {
                INFO("Expected " << words[i] << " to exist in dictionary");
                INFO("It is possible AddWord is not working correctly");
                FAIL();
            }
        }

        // Check 1000 random words (make sure order does not affect IsWord)
        for (int i = 0; i < numChecks; i++) {
            // Get random word
            int randWordInd = rand()%words.size();
            string randWord = words[randWordInd];

//            REQUIRE(dict.IsWord(randWord));
            if(!dict.IsWord(randWord)) {
                INFO("Expected " << words[i] << " to exist in dictionary");
                INFO("It is possible AddWord is not working correctly");
                FAIL();
            }
        }

        // Make sure pokemon are NOT in the dictionary.
        // Note some pokemon like "ditto" have been removed since
        // they actually are words found in a dictionary.
        for (int i = 0; i < pokemon.size(); i++) {
//            REQUIRE(!dict.IsWord(pokemon[i]));
            if (pokemon[i] == "the pokemon you found") {
               cout << "At the pokemon" << endl;
            }

            if(dict.IsWord(pokemon[i])) {
                INFO(pokemon[i] << " was found in the dictionary");
                INFO("It is possible IsWord or AddWord is not working correctly");
                FAIL();
            }
        }
    }

    SECTION("Bad Character Test - 2pts") {
        vector<string> words = read_words("test_data/bad_words.txt");

        // Try several words with bad characters
        for (int i = 0; i < words.size(); i++) {
            try {
                Dictionary dict;
                dict.AddWord(words[i]);
            }
            catch (DictionaryError e) {
                //cerr << e.Msg() << endl;
                if (e.Msg() != "Invalid character") {
                    INFO("DictionaryError message not \"Invalid character\"");
                    FAIL();
                }
            }
            catch (...) {
                INFO("Your program gave an unexpected throw when adding the word " << words[i]);
                INFO("Check your default constructor and AddWord.");
                INFO("Make sure you are using DictionaryError to throw errors.");
                INFO("Make sure the DictionaryError is also using the correct message.");
                FAIL();
            }
        }
    }

    SECTION("IsPrefix test - 4pts") {
        int numChecks = 1000;

        string dictName = "dictionary.txt";
        Dictionary dict;

        // Loads words from file into a vector
        vector<string> words = read_words(dictName);
        for (int i = 0; i < words.size(); i++) {
            dict.AddWord(words[i]);
        }

        ifstream pokeFile;
        pokeFile.open("test_data/pokemon.txt");
        if (pokeFile.fail()) {
            INFO("pokemon.txt did not open.");
            INFO("");
            INFO("Possible causes:");
            INFO("$PROJECT_DIR$ was not set in the run configuration.");
            INFO("File was overwritten by accident and is now empty.");
            INFO("File was moved");
            FAIL();
        }

        // Check 1000 random words
        for (int i = 0; i < numChecks; i++) {
            // Get random word and prefix
            int randWordInd = rand()%words.size();
            string randWord = words[randWordInd];
            string randPrefix = randWord.substr(0,rand()%randWord.length());

            // REQUIRE(dict.IsPrefix(randPrefix));
            if(!dict.IsPrefix(randPrefix)) {
                INFO("Expected " << randPrefix << " to be a valid prefix.");
                INFO("It is possible IsPrefix or AddWord is not working correctly.");
                FAIL();
            }
        }
    }



    SECTION("LoadDictionary test - 4pts") {
        int numChecks = 1000;
        string dictName = "dictionary.txt";

        // Create Dictionary and load file
        Dictionary dict;
        dict.LoadDictionaryFile(dictName);

        // Loads words from file into a vector
        vector<string> words = read_words(dictName);

        // Make sure all the words are in the dictionary
        // Assuming AddWord and IsWord works, then this should work fine.
        for (int i = 0; i < words.size(); i++) {
//            REQUIRE(dict.IsWord(words[i]));
            if(!dict.IsWord(words[i])) {
                INFO("Expected " << words[i] << " to exist in dictionary");
                INFO("It is possible LoadDictionaryFile is not working correctly");
                FAIL();
            }
        }
    }

    SECTION("SaveDictionary test - 4pts") {
        int numChecks = 1000;

        // Create and then save the dictionary
        Dictionary dict("test_data/pokemon.txt");
        dict.SaveDictionaryFile("saved_pokemon_dictionary.txt");

        // Load both files
        vector<string> poke_test = read_words("test_data/pokemon.txt");
        vector<string> poke_saved = read_words("saved_pokemon_dictionary.txt");

        // Make sure they both have the same number of words
        REQUIRE(poke_test.size() == poke_saved.size());

        // Check to make sure the lists match
        for (int i = 0; i < poke_test.size(); i++) {
            //REQUIRE(poke_test[i] == poke_saved[i]);
            if(poke_test[i] != poke_saved[i]) {
                INFO("Your saved pokemon list does not match the expected pokemon list.");
                INFO("It is possible your SaveDictionaryFile does not work.");
                INFO("Expected Pokemon: " << poke_test[i]);
                INFO("    Your Pokemon: " << poke_saved[i]);
                FAIL();
            }
        }

    }

    SECTION("MakeEmpty tests - 4pts") {
        // Loads words from file into a vector
        vector<string> words = read_words("dictionary.txt");

        // Manually add words to dictionary
        Dictionary dict1;
        for (int i = 0; i < words.size(); i++) {
            dict1.AddWord(words[i]);
        }

        dict1.MakeEmpty();

        // No words should exist afterwards
        // REQUIRE(dict1.WordCount() == 0);
        if (dict1.WordCount() != 0) {
            INFO("Expected 0 words after MakeEmpty");
            INFO("It is possible MakeEmpty is not working correctly");
            FAIL();
        }

        for (int i = 0; i < words.size(); i++) {
            //REQUIRE(!dict1.IsWord(words[i]));
            if(dict1.IsWord(words[i])) {
                INFO(words[i] << " was found in dictionary after call to MakeEmpty().");
                INFO("Expected" << words[i] << " not to be found.");
                FAIL();
            }
        }

        try {
            dict1.MakeEmpty();
        } catch (...) {
            INFO("An error occurred when trying to empty a dictionary");
            INFO("that was already empty.");
            INFO("Check your MakeEmpty() function.");
            FAIL();
        }
    }

    SECTION("Assignment Operator test - 4pts") {
        // Loads words from dictionary into a vector
        vector<string> words = read_words("dictionary.txt");

        // Loads words from a small dictionary into a vector
        vector<string> words2 = read_words("smallDictionary.txt");

        // Loads pokemon from file into a vector
        vector<string> pokemon = read_words("test_data/pokemon.txt");
        Dictionary dict1, dict2;

        // Manually add words to dict1
        for (int i = 0; i < words.size(); i++) {
            dict1.AddWord(words[i]);
        }

        // Manually add words to dict2
        for (int i = 0; i < words2.size(); i++) {
            dict2.AddWord(words2[i]);
        }

        // Make a copy of dict1 into dict2.  This should remove
        // words from dict2 and place words from dict1 into dict2.
        dict2 = dict1;

        // Add the pokemon list to the original dictionary
        for (int i = 0; i < words.size(); i++) {
            REQUIRE(dict2.IsWord(words[i]));
        }

        // Add the pokemon list to the original dictionary
        for (int i = 0; i < pokemon.size(); i++) {
            dict1.AddWord(pokemon[i]);
        }

        // The pokemon should NOT exist in the second dictionary
        for (int i = 0; i < pokemon.size(); i++) {
            if (dict2.IsWord(pokemon[i])) {
                INFO(pokemon[i] << " was found hiding in dict2.");
                INFO("Sadly, this was not very effective.");
                INFO("Make sure the assignment operator function is not just setting the roots equal.");
                FAIL();
            }
        }
    }
}