#ifndef tweet_parser_h
#define tweet_parser_h

#include "tweet.h"

/* TweetParser class */

class TweetParser {
public:
    TweetParser();
    
    ~TweetParser();
    
    TweetParser operator=(const TweetParser& tc);
    
    vector<Tweet> parseTweetData(string inputTweetDataFilename);
    
    void writeParsedTweetDataToFile(string parsedTweetDataFilename, vector<Tweet> parsedTweetData);
    
private:
    string getScreenNameForSingleTweet(string tweet) const;
    
    string getTimeStampForSingleTweet(string tweet) const;
    
    string getContentForSingleTweet(string tweet) const;
    
    vector<string> getMentionsForSingleTweet(string tweet) const;
    
    // takes as input a string of mentions, and returns the FIRST mention in the string, also DELETING the mention from input
    string eraseAndReturnFirstMention(string& mentions) const;
    
    vector<Tweet> parsedTweetData;
    string tweetPreScreenNameAsString;
    string tweetPostScreenNameAsString;
    string tweetProfileSidebarFillColorAsString;
    string tweetPreTimeStampAsString;
    string tweetPostTimeStampAsString;
    string tweetPreContentAsString;
    string tweetPostContentAsString;
    string tweetPreMentionsAsString;
    string tweetPostMentionsAsString;
    string mentionPreMentionAsString;
    string mentionPostMentionAsString;
    string mentionPreScreenNameAsString;
    string mentionPostScreenNameAsString;
};

#endif