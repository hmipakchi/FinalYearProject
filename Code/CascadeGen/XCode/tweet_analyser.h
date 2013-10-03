#ifndef tweet_analyser_h
#define tweet_analyser_h

#include "tweet.h"

typedef enum {OR = 0, AND = 1, NOT = 2} tweetAnalyserKeywordSearchOption;

/* TweetAnalyser class */

class TweetAnalyser {
public:
    TweetAnalyser();
    
    ~TweetAnalyser();
    
    TweetAnalyser operator=(const TweetAnalyser& tc);
    
    // read in a parsed tweet data file
    vector<Tweet> readParsedTweetDataFromFile(string parsedTweetDataFilename);

    // print to console output data for the tweets specified in input
    void printTweetData(vector<Tweet> tweetData);
    
    // note: NEED to sort Tweet objects by screenName !!!
    // return all distinct screen names with number of tweets sent from each screen name, as well as id's of all tweets sent
    vector<TweetScreenNameOccurenceCacheItem> extractAmountTweetDataForAllScreenNames(const vector<Tweet> tweetData) const;
    
    // return, for each keyword, the number of tweets that were sent that contained that keyword, as well as id's of all tweets sent
    vector<TweetKeywordOccurenceCacheItem> extractAmountTweetDataForKeywords(const vector<Tweet> tweetData, vector<string> keywords);
    
    // return, for each screen name, all tweets sent by the account
    vector<Tweet> extractSpecificScreenNameTweetData(const vector<Tweet> tweetData, vector<string> screenNames);
    
    // return, for a combination of the keywords (depending on the option), all tweets containing the combination
    vector<Tweet> extractKeywordsContainedTweetData(const vector<Tweet> tweetData, vector<string> keywords, tweetAnalyserKeywordSearchOption option);
    
    // return all tweets corresponding to the ids specified.
    vector<Tweet> extractSpecificIdTweetData(const vector<Tweet> tweetData, vector<int> tweetIds);
    
    // return tweets that have been @username mentioned by and to users, that have actively tweeted, and messages contains combination of the keywords (depending on the option),
    vector<TweetKeywordUsernameMentionedCacheItem> extractKeywordsContainedUsernameMentionedTweetData(const vector<Tweet> tweetData, vector<string> keywords, tweetAnalyserKeywordSearchOption option);
    
private:
    vector<string> extractMentionsAndStoreFromString(string mentions);
    
    const string mentionPostMentionAsString = ";";
};

#endif