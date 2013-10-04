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
    
    const string tweetPreScreenNameAsString = "\"screen_name\":\"";
    const string tweetPostScreenNameAsString = "\",\"";
    const string tweetProfileSidebarFillColorAsString = "profile_sidebar_fill_color";
    const string tweetPreTimeStampAsString = "\"created_at\":\"";
    const string tweetPostTimeStampAsString = "\",\"";
    const string tweetPreContentAsString = "\"text\":\"";
    const string tweetPostContentAsString = "\",\"";
    const string tweetPreMentionsAsString = "\"user_mentions\":[";
    const string tweetPostMentionsAsString = "]}";
    const string mentionPreMentionAsString = "{";
    const string mentionPostMentionAsString = "\"}";
    const string mentionPreScreenNameAsString = "\"screen_name\":\"";
    const string mentionPostScreenNameAsString = "\",\"";
};

#endif