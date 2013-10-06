#ifndef tweet_h
#define tweet_h

#include "utility.h"


/* Twitter Account class */

class TwitterAccount {
public:
    TwitterAccount();
    
    ~TwitterAccount();
    
    TwitterAccount(const TwitterAccount& tc);
    
    TwitterAccount(int id, string screenName);
    
    TwitterAccount operator=(const TwitterAccount& tc);
    
    void setId(const int id);
    
    int getId() const;
    
    void setScreenName (const string screenName);
    
    string getScreenName() const;
    
    string toString() const;
    
    friend ostream& operator<<(ostream& os, const TwitterAccount& obj);
private:
    int id;
    string screenName;
};


/* Tweet class */

class Tweet {
public:
    Tweet();
    
    ~Tweet();
    
    Tweet(const Tweet& tc);
    
    Tweet(int id, TwitterAccount account, string timeStamp, string content, vector<string> mentions);
    
    Tweet operator=(const Tweet& tc);
    
    void setId(const int id);
    
    int getId() const;
    
    void setAccount(const TwitterAccount account);
    
    TwitterAccount getAccount() const;
    
    void setTimeStamp(const string timeStamp);
    
    string getTimeStamp() const;
    
    void setContent(const string content);
    
    string getContent() const;
    
    void setMentions(const vector<string> mentions);
    
    vector<string> getMentions() const;
    
    string getTimeStampFormat() const;
    
    string toString() const;
    
    bool compareTimeStamps(const Tweet& tweet2) const;
    
    bool compareScreenNames(const Tweet& tweet2) const;
    
    bool compareIds(const Tweet& tweet2) const;
    
    bool compareScreenNameIds(const Tweet& tweet2) const;
    
    bool operator<(const Tweet& tweet2) const;
    
    friend ostream& operator<<(ostream& os, const Tweet& obj);
    
private:
    int id;
    TwitterAccount account;
    string timeStamp;
    string content;
    vector<string> mentions;
    string timeStampFormat;
};


/* TweetScreenNameOccurenceCacheItem class */

class TweetScreenNameOccurenceCacheItem {
public:
    TweetScreenNameOccurenceCacheItem();
    
    ~TweetScreenNameOccurenceCacheItem();
    
    TweetScreenNameOccurenceCacheItem operator=(const TweetScreenNameOccurenceCacheItem& tc);
    
    TweetScreenNameOccurenceCacheItem(TwitterAccount account, int noTweetsSent, vector<int> tweetIds);
    
    TwitterAccount getAccount() const;
    
    int getNoTweetsSent() const;
    
    vector<int> getTweetIds() const;
    
    void setAccount(const TwitterAccount& account);
    
    void setNoTweetsSent(const int noTweetsSent);
    
    void setTweetIds(const vector<int> tweetIds);
    
    void incrementNoTweetsSent();
    
    string toString() const;
    
    bool compareNoTweetsSent(const TweetScreenNameOccurenceCacheItem& tweetScreenNameOccurenceCacheItem2) const;
    
    bool compareScreenNames(const TweetScreenNameOccurenceCacheItem& tweetScreenNameOccurenceCacheItem2) const;
    
    bool compareScreenNameIds(const TweetScreenNameOccurenceCacheItem& tweetScreenNameOccurenceCacheItem2) const;
    
    bool operator<(const TweetScreenNameOccurenceCacheItem& tweetScreenNameOccurenceCacheItem2) const;
    
    friend ostream& operator<<(ostream& os, const TweetScreenNameOccurenceCacheItem& obj);
    
private:
    TwitterAccount account;
    int noTweetsSent;
    vector<int> tweetIds;
};


/* TweetKeywordOccurenceCacheItem class */

class TweetKeywordOccurenceCacheItem {
public:
    TweetKeywordOccurenceCacheItem();
    
    ~TweetKeywordOccurenceCacheItem();
    
    TweetKeywordOccurenceCacheItem operator=(const TweetKeywordOccurenceCacheItem& tc);
    
    TweetKeywordOccurenceCacheItem(string keyword, int noTweetsSent, vector<int> tweetIds);
    
    string getKeyword() const;
    
    int getNoTweetsSent() const;
    
    vector<int> getTweetIds() const;
    
    void setKeyword(const string keyword);
    
    void setNoTweetsSent(const int noTweetsSent);
    
    void setTweetIds(const vector<int> tweetIds);
    
    void incrementNoTweetsSent();
    
    string toString() const;
    
    bool compareNoTweetsSent(const TweetKeywordOccurenceCacheItem& tweetKeywordOccurenceCacheItem2) const;
    
    bool compareKeywords(const TweetKeywordOccurenceCacheItem& tweetKeywordOccurenceCacheItem2) const;
    
    bool operator<(const TweetKeywordOccurenceCacheItem& tweetKeywordOccurenceCacheItem2) const;
    
    friend ostream& operator<<(ostream& os, const TweetKeywordOccurenceCacheItem& obj);
    
private:
    string keyword;
    int noTweetsSent;
    vector<int> tweetIds;
};


/* TweetKeywordUsernameMentionedCacheItem class */

class TweetKeywordUsernameMentionedCacheItem {
public:
    TweetKeywordUsernameMentionedCacheItem();
    
    ~TweetKeywordUsernameMentionedCacheItem();
    
    TweetKeywordUsernameMentionedCacheItem operator=(const TweetKeywordUsernameMentionedCacheItem& tc);
    
    TweetKeywordUsernameMentionedCacheItem(Tweet initialTweet, Tweet influencedTweet);
    
    Tweet getInitialTweet() const;
    
    Tweet getInfluencedTweet() const;
    
    void setInitialTweet(const Tweet& initialTweet);
    
    void setInfluencedTweet(const Tweet& influencedTweet);
    
    string toString() const;
    
    friend ostream& operator<<(ostream& os, const TweetKeywordUsernameMentionedCacheItem& obj);
    
private:
    Tweet initialTweet;
    Tweet influencedTweet;
};


#endif