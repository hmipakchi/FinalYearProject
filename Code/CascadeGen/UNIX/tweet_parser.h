#ifndef tweet_parser_h
#define tweet_parser_h

#include "tweet.h"
#include "utility.h"

/* TweetParser class */

class TweetParser {
public:
    TweetParser();
    
    ~TweetParser();
    
    TweetParser operator=(const TweetParser& tc);
    
    vector<Tweet> parseTweetData(const string& inputTweetDataFilename) const ;
    
    void writeParsedTweetDataToFile(const string& parsedTweetDataFilename, const vector<Tweet>& parsedTweetData) const;
    
private:
    string getScreenNameForSingleTweet(const string& tweet) const;
    
    string getTimeStampForSingleTweet(const string& tweet) const;
    
    string getContentForSingleTweet(const string& tweet) const;
    
    vector<string> getMentionsForSingleTweet(const string& tweet) const;
    
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