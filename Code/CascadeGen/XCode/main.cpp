//
//  main.cpp
//  Tweet_Parser_Test
//
//  Created by Hesam Ipakchi on 28/09/2013.
//  Copyright (c) 2013 Hesam Ipakchi. All rights reserved.
//

#include "utility.h"
#include "tweet.h"
#include "tweet_parser.h"
#include "tweet_analyser.h"
#include "cascade.h"


int main(int argc, const char * argv[]) {
    
//    string inputTweetDataFilename = "3_tweets.txt";
//    string inputTweetDataFilename = "100_tweets.txt";
    string inputTweetDataFilename = "100k_tweets.txt";
    
//    string parsedTweetDataFilename = "3_tweets_parsed.txt";
//    string parsedTweetDataFilename = "100_tweets_parsed.txt";
    string parsedTweetDataFilename = "100k_tweets_parsed.txt";
    
    vector<Tweet> parsedTweetData;
    
    string cascadesDataFilename = "example-cascades.txt";
    
    vector<string> keywords;
    keywords.push_back("referee");
    keywords.push_back("red card");
    keywords.push_back("shelvey");
    keywords.push_back("lfc");
    keywords.push_back("mufc");
    keywords.push_back("blackberry");
    keywords.push_back("olympics");
    keywords.push_back("apple");
    keywords.push_back("iphone");
    keywords.push_back("twitter");
    keywords.push_back("sleep");
    keywords.push_back("school");
    keywords.push_back("uni");
    tweetAnalyserKeywordSearchOption option = OR;
    
    vector <string> screenNames;
    screenNames.push_back("108Lord");
    
    vector<int> screenNameIds;
    screenNameIds.push_back(18227);
    screenNameIds.push_back(11589);
    screenNameIds.push_back(3644);
    screenNameIds.push_back(3195);
    
    vector<int> tweetIds;
    tweetIds.push_back(21297);

    /* parse tweet data to a cleaner format */
//    TweetParser tweetParser;
//    try {
//        parsedTweetData = tweetParser.parseTweetData(inputTweetDataFilename);
//        tweetParser.writeParsedTweetDataToFile(parsedTweetDataFilename, parsedTweetData);
//    } catch (const char* msg) {
//        cerr << msg << endl;
//    }
    
    /* analysed parsed tweet data */
    TweetAnalyser tweetAnalyser;
    try {
        parsedTweetData = tweetAnalyser.readParsedTweetDataFromFile(parsedTweetDataFilename);
    } catch (const char* msg) {
        cerr << msg << endl;
    }
    
    /* test functionality: write cascades to file based on input tweets and keywords */
//    cout << "##########       TESTING CASCADES SECTION         #########" << endl;
//    for (int i = 0; i < keywords.size(); i++) {
//        if (i == 0 && i != keywords.size()-1)
//            cout << "##########      TESTING KEYWORDS: " << keywords.at(i);
//        else if (i == 0 && i == keywords.size()-1)
//            cout << "##########      TESTING KEYWORDS: " << keywords.at(i) << "      #########" << endl;
//        else if (i != 0 && i != keywords.size()-1)
//            cout << ", " << keywords.at(i);
//        else
//            cout << ", " << keywords.at(i) << "      #########" << endl;
//    }
//    tweetAnalyser.writeCascadesDataToFile(cascadesDataFilename, parsedTweetData, keywords);
    
    /* test functionality: return tweet data of tweets that contain certain combination of keywords */
//    vector<Tweet> twitterKeywordSelectionData = tweetAnalyser.extractKeywordsContainedTweetData(parsedTweetData, keywords, option);
//    cout << "##########       TESTING KEYWORD SECTION         #########" << endl;
//    for (int i = 0; i < keywords.size(); i++) {
//        if (i == 0 && i != keywords.size()-1)
//            cout << "##########      TESTING KEYWORDS with option '" << option << "': " << keywords.at(i);
//        else if (i == 0 && i == keywords.size()-1)
//            cout << "##########      TESTING KEYWORDS with option '" << option << "': " << keywords.at(i) << "      #########" << endl;
//        else if (i != 0 && i != keywords.size()-1)
//            cout << ", " << keywords.at(i);
//        else
//            cout << ", " << keywords.at(i) << "      #########" << endl;
//    }
//    cout << "##########         NUMBER OF TWEETS SELECTED: " << twitterKeywordSelectionData.size() << "         #########" << endl;
//    sort(twitterKeywordSelectionData.begin(), twitterKeywordSelectionData.end());
//    cout << twitterKeywordSelectionData << endl;
//    twitterKeywordSelectionData.clear();
    
     /* test functionality: return tweet data of tweets sent by certain accounts via screenName check */
    vector<Tweet> twitterSpecificScreenNameData = tweetAnalyser.extractSpecificScreenNameTweetData(parsedTweetData, screenNames);
    cout << "##########       TESTING SCREENAME SECTION         #########" << endl;
    for (int i = 0; i < screenNames.size(); i++) {
        if (i == 0 && i != screenNames.size()-1)
            cout << "##########      TESTING SCREENAMES: " << screenNames.at(i);
        else if (i == 0 && i == screenNames.size()-1)
            cout << "##########      TESTING SCREENAMES: " << screenNames.at(i) << "      #########" << endl;
        else if (i != 0 && i != screenNames.size()-1)
            cout << ", " << screenNames.at(i);
        else
            cout << ", " << screenNames.at(i) << "      #########" << endl;
    }
    cout << "##########         NUMBER OF TWEETS SELECTED: " << twitterSpecificScreenNameData.size() << "         #########" << endl;
    sort(twitterSpecificScreenNameData.begin(), twitterSpecificScreenNameData.end());
    cout << twitterSpecificScreenNameData << endl;
    twitterSpecificScreenNameData.clear();
    
    /* test functionality: return tweet data of tweets sent by certain accounts via sreenNameId check */
//    vector<Tweet> twitterSpecificScreenNameData = tweetAnalyser.extractSpecificScreenNameTweetData(parsedTweetData, screenNameIds);
//    cout << "##########       TESTING SCREENAME IDS SECTION         #########" << endl;
//    for (int i = 0; i < screenNameIds.size(); i++) {
//        if (i == 0 && i != screenNameIds.size()-1)
//            cout << "##########      TESTING SCREENAME IDS: " << screenNameIds.at(i);
//        else if (i == 0 && i == screenNameIds.size()-1)
//            cout << "##########      TESTING SCREENAME IDS: " << screenNameIds.at(i) << "      #########" << endl;
//        else if (i != 0 && i != screenNameIds.size()-1)
//            cout << ", " << screenNameIds.at(i);
//        else
//            cout << ", " << screenNameIds.at(i) << "      #########" << endl;
//    }
//    cout << "##########         NUMBER OF TWEETS SELECTED: " << twitterSpecificScreenNameData.size() << "         #########" << endl;
//    cout << twitterSpecificScreenNameData << endl;
//    twitterSpecificScreenNameData.clear();
    
    /* test functionality: return tweet data of particular tweet Ids */
//    vector<Tweet> twitterSpecificIdData = tweetAnalyser.extractSpecificIdTweetData(parsedTweetData, tweetIds);
//    cout << "##########       TESTING IDS SECTION         #########" << endl;
//    for (int i = 0; i < tweetIds.size(); i++) {
//        if (i == 0 && i != tweetIds.size()-1)
//            cout << "##########      TESTING IDS: " << tweetIds.at(i);
//        else if (i == 0 && i == tweetIds.size()-1)
//            cout << "##########      TESTING IDS: " << tweetIds.at(i) << "      #########" << endl;
//        else if (i != 0 && i != tweetIds.size()-1)
//            cout << ", " << tweetIds.at(i);
//        else
//            cout << ", " << tweetIds.at(i) << "      #########" << endl;
//    }
//    cout << "##########         NUMBER OF TWEETS SELECTED: " << twitterSpecificIdData.size() << "         #########" << endl;
//    sort(twitterSpecificIdData.begin(), twitterSpecificIdData.end());
//    cout << twitterSpecificIdData << endl;
//    twitterSpecificIdData.clear();
    
    /* test functionality: return statistics (number of tweets) sent that contain certain keywords */
//    vector<TweetKeywordOccurenceCacheItem> tweetKeywordOccurenceCache = tweetAnalyser.extractAmountTweetDataForKeywords(parsedTweetData, keywords);
//    cout << "##########       TESTING KEYWORD OCURENCE CACHE SECTION         #########" << endl;
//    cout << "##########         NUMBER OF KEYWORDS TESTING: " << keywords.size() << "         #########" << endl;
//    sort(tweetKeywordOccurenceCache.begin(), tweetKeywordOccurenceCache.end());
//    cout << tweetKeywordOccurenceCache << endl;
//    tweetKeywordOccurenceCache.clear();
    
     /* test functionality: return statistics (number of tweets) sent by all distinct accounts */
//    vector<TweetScreenNameOccurenceCacheItem> tweetScreenNameOccurenceCache = tweetAnalyser.extractAmountTweetDataForAllScreenNames(parsedTweetData);
//    cout << "##########       TESTING SCREENAME OCCURENCE CACHE SECTION         #########" << endl;
//    cout << "##########         NUMBER OF DISTINCT SCERENNAMES: " << tweetScreenNameOccurenceCache.size() << "         #########" << endl;
//    sort(tweetScreenNameOccurenceCache.begin(), tweetScreenNameOccurenceCache.end());
//    cout << tweetScreenNameOccurenceCache << endl;
//    tweetScreenNameOccurenceCache.clear();
    
    /* test functionality: return tweets that have been @username mentioned by and to users, that have actively tweeted, and messages contains combination of the keywords (depending on the option) */
//    vector<TweetKeywordUsernameMentionedCacheItem> twitterKeywordContainedUsernameMentionedCache = tweetAnalyser.extractKeywordsContainedUsernameMentionedTweetData(parsedTweetData, keywords, option);
//    cout << "##########       TESTING KEYWORD USERNAME MENTIONED SECTION         #########" << endl;
//    for (int i = 0; i < keywords.size(); i++) {
//        if (i == 0 && i != keywords.size()-1)
//            cout << "##########      TESTING KEYWORDS with option '" << option << "': " << keywords.at(i);
//        else if (i == 0 && i == keywords.size()-1)
//            cout << "##########      TESTING KEYWORDS with option '" << option << "': " << keywords.at(i) << "      #########" << endl;
//        else if (i != 0 && i != keywords.size()-1)
//            cout << ", " << keywords.at(i);
//        else
//            cout << ", " << keywords.at(i) << "      #########" << endl;
//    }
//    cout << "##########         NUMBER OF TWEETS SELECTED: " << twitterKeywordContainedUsernameMentionedCache.size() << "         #########" << endl;
//    cout << twitterKeywordContainedUsernameMentionedCache << endl;
//    twitterKeywordContainedUsernameMentionedCache.clear();
    
    parsedTweetData.clear();
    keywords.clear();
    screenNames.clear();
    screenNameIds.clear();
    tweetIds.clear();
    
    return 0;
}