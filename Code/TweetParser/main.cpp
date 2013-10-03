//
//  main.cpp
//  Tweet_Parser_Test
//
//  Created by Hesam Ipakchi on 28/09/2013.
//  Copyright (c) 2013 Hesam Ipakchi. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <time.h>
#include <cctype>
#include <algorithm>

using namespace std;

typedef enum {OR = 0, AND = 1} tweetAnalyserKeywordSearchOption;

class Tweet {
public:
    Tweet() {}
    
    ~Tweet() {
        mentions.clear();
    }
    
    Tweet(const Tweet& tc):id(tc.id), screenName(tc.screenName), timeStamp(tc.timeStamp), content(tc.content), mentions(tc.mentions) {}
    
    Tweet(int id, string screenName, string timeStamp, string content, vector<string> mentions) {
        this->id = id;
        this->screenName = screenName;
        this->timeStamp = timeStamp;
        this->content = content;
        this->mentions = mentions;
    }

    Tweet operator=(const Tweet& tc) {
        if (this == &tc) {
            return *this;
        }
        else {
            id = tc.id;
            screenName = tc.screenName;
            timeStamp = tc.timeStamp;
            content = tc.content;
            mentions = tc.mentions;
            return *this;
        }
    }
    
    void setId(const int id) {
        this->id = id;
    }
    
    int getId() const {
        return id;
    }
    
    void setScreenName(const string screenName) {
        this->screenName = screenName;
    }
    
    string getScreenName() const {
        return screenName;
    }
    
    void setTimeStamp(const string timeStamp) {
        this->timeStamp = timeStamp;
    }
    
    string getTimeStamp() const {
        return timeStamp;
    }
    
    void setContent(const string content) {
        this->content = content;
    }
    
    string getContent() const {
        return content;
    }
    
    void setMentions(const vector<string> mentions) {
        this->mentions = mentions;
    }
    
    vector<string> getMentions() const {
        return mentions;
    }
    
    string getTimeStampFormat() const {
        return timeStampFormat;
    }
    
    string toString() const {
        string allMentions = "";
        for (int i = 0; i < mentions.size(); i++) {
            allMentions += mentions.at(i) + ";";
        }
        stringstream id_ss;
        id_ss << id;
        return id_ss.str() + "\n" + screenName + "\n" + timeStamp + "\n" + content + "\n" + allMentions;
    }
    
    bool compareTimeStamps(const Tweet& tweet2) const {
        string timeStamp1 = timeStamp;
        string timeStamp2 = tweet2.timeStamp;
        time_t t1, t2;
        struct tm tm1, tm2;
        strptime(timeStamp1.c_str(), timeStampFormat.c_str(), &tm1);
        strptime(timeStamp2.c_str(), timeStampFormat.c_str(), &tm2);
        t1 = mktime(&tm1);
        t2 = mktime(&tm2);
        return (difftime(t1, t2) > 0.0) ? false : true;
    }
    
    bool compareScreenNames(const Tweet& tweet2) const {
        string screenName1 = screenName;
        string screenName2 = tweet2.screenName;
        transform(screenName1.begin(), screenName1.end(), screenName1.begin(), ::tolower);
        transform(screenName2.begin(), screenName2.end(), screenName2.begin(), ::tolower);
        return (screenName1 < screenName2) ? true : false;
    }
    
    bool compareIds(const Tweet& tweet2) const {
        int id1 = id;
        int id2 = tweet2.id;
        return (id1 < id2) ? true : false;
    }
private:
    int id;
    string screenName;
    string timeStamp;
    string content;
    vector<string> mentions;
    const string timeStampFormat = "%a %b %d %T %z %Y";
};

bool operator<(const Tweet& tweet1, const Tweet& tweet2) {
    // compare using timestamps or screennames or ids
//    return tweet1.compareTimeStamps(tweet2);
    return tweet1.compareScreenNames(tweet2);
//    return tweet1.compareIds(tweet2);
}

class TweetParser {
public:
    TweetParser() {}
    
    ~TweetParser() {
        parsedTweetData.clear();
    }
    
    TweetParser operator=(const TweetParser& tc) {
        if (this == &tc) {
            return *this;
        }
        else {
            parsedTweetData = tc.parsedTweetData;
            return *this;
        }
    }
    
    vector<Tweet> getParsedTweetData() const {
        return parsedTweetData;
    }
        
    void parseTweetData(string inputTweetDataFilename) {
        ifstream fInp;
        string tweet;
        fInp.open(inputTweetDataFilename.c_str());
        if (fInp.is_open()) {
            int id = 1;
            while (!fInp.eof()) {
                getline(fInp, tweet);
                if (tweet != "") {
                    string tweetScreenName = getScreenNameForSingleTweet(tweet);
                    string tweetTimeStamp = getTimeStampForSingleTweet(tweet);
                    string tweetContent = getContentForSingleTweet(tweet);
                    vector<string> tweetMentions = getMentionsForSingleTweet(tweet);
                    parsedTweetData.push_back(Tweet(id, tweetScreenName, tweetTimeStamp, tweetContent, tweetMentions));
                }
                id++;
            }
            fInp.close();
            
            cout << "######   *****   NUMBER OF TWEETS = " << parsedTweetData.size() << "   *****   ######" << endl;
        }
        else {
            cout << "error opening file: " << inputTweetDataFilename << endl;
        }
    }
    
    void writeParsedTweetDataToFile(string parsedTweetDataFilename) {
        ofstream fOut;
        fOut.open(parsedTweetDataFilename.c_str(), fstream::out);
        if (fOut.is_open()) {
            int noTweets = (int) parsedTweetData.size();
            for (int i = 0; i < noTweets; i++) {
                if (i != (noTweets-1)) {
                    fOut << parsedTweetData.at(i).toString() << endl;
                }
                else {
                    fOut << parsedTweetData.at(i).toString();
                }
            }
            fOut.close();
        }
        else {
            cout << "error opening file: " << parsedTweetDataFilename << endl;
        }
    }
    
private:
    string getScreenNameForSingleTweet(string tweet) const {
        size_t tweetProfileSidebarFillColorStringPosition = tweet.find(tweetProfileSidebarFillColorAsString);
        if (tweetProfileSidebarFillColorStringPosition != string::npos) {
            size_t tweetScreenNameStringStartPosition = tweet.find(tweetPreScreenNameAsString,tweetProfileSidebarFillColorStringPosition+1);
            if (tweetScreenNameStringStartPosition != string::npos) {
                size_t tweetScreenNameStringEndPosition = tweet.find(tweetPostScreenNameAsString,tweetScreenNameStringStartPosition+1);
                if (tweetScreenNameStringEndPosition != string::npos) {
                    int tweetScreenNameStringLength = (int) tweetScreenNameStringEndPosition - (int) tweetScreenNameStringStartPosition - (int) tweetPreScreenNameAsString.length();
                    string screenName = tweet.substr(tweetScreenNameStringStartPosition+tweetPreScreenNameAsString.length(), tweetScreenNameStringLength);
                    return screenName;
                }
                else {
                    cout << "error with tweetScreenNameStringEndPosition, position = " << tweetScreenNameStringEndPosition << endl;
                    return "";
                }
            }
            else {
                cout << "error with tweetScreenNameStringStartPosition, position = " << tweetScreenNameStringStartPosition << endl;
                return "";
            }
        }
        else {
            cout << "error with tweetProfileSidebarFillColorStringPosition, position = " << tweetProfileSidebarFillColorStringPosition << endl;
            return "";
        }
    }
    
    string getTimeStampForSingleTweet(string tweet) const {
        size_t stringStringStartPosition = tweet.find(tweetPreTimeStampAsString);
        
        if (stringStringStartPosition != string::npos) {
            size_t stringStringEndPosition = tweet.find(tweetPostTimeStampAsString,stringStringStartPosition+1);
            if (stringStringEndPosition != string::npos) {
                int stringStringLength = (int) stringStringEndPosition - (int) stringStringStartPosition - (int) tweetPreTimeStampAsString.length();
                string timeStamp = tweet.substr(stringStringStartPosition+tweetPreTimeStampAsString.length(), stringStringLength);
                return timeStamp;
            }
            else {
                cout << "error with stringStringEndPosition, position = " << stringStringEndPosition << endl;
                return "";
            }
        }
        else {
            cout << "error with stringStringStartPosition, position = " << stringStringStartPosition << endl;
            return "";
        }
    }
    
    string getContentForSingleTweet(string tweet) const {
        size_t tweetContentStringStartPosition = tweet.find(tweetPreContentAsString);
        
        if (tweetContentStringStartPosition != string::npos) {
            size_t tweetContentStringEndPosition = tweet.find(tweetPostContentAsString,tweetContentStringStartPosition+1);
            if (tweetContentStringEndPosition != string::npos) {
                int tweetContentStringLength = (int) tweetContentStringEndPosition - (int) tweetContentStringStartPosition - (int) tweetPreContentAsString.length();
                string content = tweet.substr(tweetContentStringStartPosition+tweetPreContentAsString.length(), tweetContentStringLength);
                return content;
            }
            else {
                cout << "error with tweetContentStringEndPosition, position = " << tweetContentStringEndPosition << endl;
                return "";
            }
        }
        else {
            cout << "error with tweetContentStringStartPosition, position = " << tweetContentStringStartPosition << endl;
            return "";
        }
    }
    
    vector<string> getMentionsForSingleTweet(string tweet) const {
        vector<string> mentions_v;
        size_t tweetMentionsStringStartPosition = tweet.find(tweetPreMentionsAsString);
        
        if (tweetMentionsStringStartPosition != string::npos) {
            size_t tweetMentionsStringEndPosition = tweet.find(tweetPostMentionsAsString, tweetMentionsStringStartPosition+1);
            if (tweetMentionsStringEndPosition != string::npos) {
                int tweetMentionsStringLength = (int) tweetMentionsStringEndPosition - (int) tweetMentionsStringStartPosition - (int) tweetPreMentionsAsString.length();
                string mentions = tweet.substr(tweetMentionsStringStartPosition+tweetPreMentionsAsString.length(), tweetMentionsStringLength);
                // no mentions in the tweet
                if (mentions == "") {
                    return mentions_v;
                }
                // at least one mention in tweet
                else {
                    while (mentions != "") {
                        string firstMention = eraseAndReturnFirstMention(mentions);
                        mentions_v.push_back(firstMention);
                    }
                    return mentions_v;
                }
            }
            else {
                cout << "error with tweetMentionsStringEndPosition, position = " << tweetMentionsStringEndPosition << endl;
                return mentions_v;
            }
        }
        else {
            cout << "error with tweetMentionsStringStartPosition, position = " << tweetMentionsStringStartPosition << endl;
            return mentions_v;
        }
    }
    
    // takes as input a string of mentions, and returns the FIRST mention in the string, also DELETING the mention from input
    string eraseAndReturnFirstMention(string& mentions) const {
        size_t firstMentionStartPosition = mentions.find(mentionPreMentionAsString);
        if (firstMentionStartPosition != string::npos) {
            size_t firstMentionEndPosition = mentions.find(mentionPostMentionAsString, firstMentionStartPosition+1);
            if (firstMentionEndPosition != string::npos) {
                // length of string containing first mention
                int firstMentionStringLength = (int) firstMentionEndPosition - (int) firstMentionStartPosition;
                // extract first mention screen name and delete first mention from input mentions string
                size_t firstMentionScreenNameStartPosition = mentions.find(mentionPreScreenNameAsString, firstMentionStartPosition+1);
                if (firstMentionScreenNameStartPosition != string::npos) {
                    size_t firstMentionScreenNameEndPosition = mentions.find(mentionPostScreenNameAsString, firstMentionScreenNameStartPosition+1);
                    if (firstMentionScreenNameEndPosition != string::npos) {
                        int firstMentionScreenNameStringLength = (int) firstMentionScreenNameEndPosition - (int) firstMentionScreenNameStartPosition - (int) mentionPreScreenNameAsString.length();
                        string screenName = mentions.substr(firstMentionScreenNameStartPosition+mentionPreScreenNameAsString.length(), firstMentionScreenNameStringLength);
                        // first mention is only mention
                        if (mentions.length() == firstMentionStringLength) {
                            mentions = "";
                        }
                        // erase first mention as well as separator after
                        else {
                            mentions.erase(mentions.begin(), mentions.begin()+firstMentionEndPosition+mentionPostMentionAsString.length());
                        }
                        return screenName;
                    }
                    else
                        cout << "error with firstMentionScreenNameEndPosition, position = " << firstMentionScreenNameEndPosition << endl;
                        return "";
                }
                else
                    cout << "error with firstMentionScreenNameStartPosition, position = " << firstMentionScreenNameStartPosition << endl;
                    return "";
            }
            else
                cout << "error with firstMentionEndPosition, position = " << firstMentionEndPosition << endl;
                return "";
        }
        // assume no mentions left
        else
            cout << "error with firstMentionStartPosition, position = " << firstMentionStartPosition << endl;
            return "";
    }
    
    vector<Tweet> parsedTweetData;
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

class TweetScreenNameOccurenceCacheItem {
public:
    TweetScreenNameOccurenceCacheItem() {}
    
    ~TweetScreenNameOccurenceCacheItem() {
        tweetIds.clear();
    }
    
    TweetScreenNameOccurenceCacheItem operator=(const TweetScreenNameOccurenceCacheItem& tc) {
        if (this == &tc) {
            return *this;
        }
        else {
            screenName = tc.screenName;
            noTweetsSent= tc.noTweetsSent;
            tweetIds = tc.tweetIds;
            return *this;
        }
    }
    
    TweetScreenNameOccurenceCacheItem(string screenName, int noTweetsSent, vector<int> tweetIds) {
        this->screenName = screenName;
        this->noTweetsSent = noTweetsSent;
        this->tweetIds = tweetIds;
    }
    
    string getScreenName() const {
        return screenName;
    }
    
    int getNoTweetsSent() const {
        return noTweetsSent;
    }
    
    vector<int> getTweetIds() const {
        return tweetIds;
    }
    
    void setScreenName(const string screenName) {
        this->screenName = screenName;
    }
    
    void setNoTweetsSent(const int noTweetsSent) {
        this->noTweetsSent = noTweetsSent;
    }
    
    void setTweetIds(const vector<int> tweetIds) {
        this->tweetIds = tweetIds;
    }
    
    void incrementNoTweetsSent() {
        noTweetsSent++;
    }

    string toString() const {
        stringstream allTweetsIds_ss;
        for (int i = 0; i < tweetIds.size(); i++) {
            allTweetsIds_ss << tweetIds.at(i);
            allTweetsIds_ss << ";";
        }
        stringstream noTweetsSent_ss;
        noTweetsSent_ss << noTweetsSent;
        return screenName + ", " + noTweetsSent_ss.str() + ", " + allTweetsIds_ss.str();
    }

    bool compareNoTweetsSent(const TweetScreenNameOccurenceCacheItem& tweetScreenNameOccurenceCacheItem2) const {
        return (noTweetsSent < tweetScreenNameOccurenceCacheItem2.noTweetsSent) ? true : false;
    }
    
    bool compareScreenNames(const TweetScreenNameOccurenceCacheItem& tweetScreenNameOccurenceCacheItem2) const {
        string screenName1 = screenName;
        string screenName2 = tweetScreenNameOccurenceCacheItem2.screenName;
        transform(screenName1.begin(), screenName1.end(), screenName1.begin(), ::tolower);
        transform(screenName2.begin(), screenName2.end(), screenName2.begin(), ::tolower);
        return (screenName1 < screenName2) ? true : false;
    }
private:
    string screenName;
    int noTweetsSent;
    vector<int> tweetIds;
};

bool operator<(const TweetScreenNameOccurenceCacheItem& tweetScreenNameOccurenceCacheItem1, const TweetScreenNameOccurenceCacheItem& tweetScreenNameOccurenceCacheItem2) {
    // compare using noTweetsSent or keywords
    return tweetScreenNameOccurenceCacheItem1.compareNoTweetsSent(tweetScreenNameOccurenceCacheItem2);
    //    return tweetScreenNameOccurenceCacheItem1.compareScreenNames(tweetScreenNameOccurenceCacheItem2);
}

class TweetKeywordOccurenceCacheItem {
public:
    TweetKeywordOccurenceCacheItem() {}
    
    ~TweetKeywordOccurenceCacheItem() {
        tweetIds.clear();
    }
    
    TweetKeywordOccurenceCacheItem operator=(const TweetKeywordOccurenceCacheItem& tc) {
        if (this == &tc) {
            return *this;
        }
        else {
            keyword = tc.keyword;
            noTweetsSent= tc.noTweetsSent;
            tweetIds = tc.tweetIds;
            return *this;
        }
    }
    
    TweetKeywordOccurenceCacheItem(string keyword, int noTweetsSent, vector<int> tweetIds) {
        this->keyword = keyword;
        this->noTweetsSent = noTweetsSent;
        this->tweetIds = tweetIds;
    }
    
    string getKeyword() const {
        return keyword;
    }
    
    int getNoTweetsSent() const {
        return noTweetsSent;
    }
    
    vector<int> getTweetIds() const {
        return tweetIds;
    }
    
    void setKeyword(const string keyword) {
        this->keyword = keyword;
    }
    
    void setNoTweetsSent(const int noTweetsSent) {
        this->noTweetsSent = noTweetsSent;
    }
    
    void setTweetIds(const vector<int> tweetIds) {
        this->tweetIds = tweetIds;
    }
    
    void incrementNoTweetsSent() {
        noTweetsSent++;
    }
    
    string toString() const {
        stringstream allTweetsIds_ss;
        for (int i = 0; i < tweetIds.size(); i++) {
            allTweetsIds_ss << tweetIds.at(i);
            allTweetsIds_ss << ";";
        }
        stringstream noTweetsSent_ss;
        noTweetsSent_ss << noTweetsSent;
        return keyword + ", " + noTweetsSent_ss.str() + ", " + allTweetsIds_ss.str();
    }
    
    bool compareNoTweetsSent(const TweetKeywordOccurenceCacheItem& tweetKeywordOccurenceCacheItem2) const {
        return (noTweetsSent < tweetKeywordOccurenceCacheItem2.noTweetsSent) ? true : false;
    }
    
    bool compareKeywords(const TweetKeywordOccurenceCacheItem& tweetKeywordOccurenceCacheItem2) const {
        string keyword1 = keyword;
        string keyword2 = tweetKeywordOccurenceCacheItem2.keyword;
        transform(keyword1.begin(), keyword1.end(), keyword1.begin(), ::tolower);
        transform(keyword2.begin(), keyword2.end(), keyword2.begin(), ::tolower);
        return (keyword1 < keyword2) ? true : false;
    }
private:
    string keyword;
    int noTweetsSent;
    vector<int> tweetIds;
};

bool operator<(const TweetKeywordOccurenceCacheItem& tweetKeywordOccurenceCacheItem1, const TweetKeywordOccurenceCacheItem& tweetKeywordOccurenceCacheItem2) {
    // compare using noTweetsSent or keywords
    return tweetKeywordOccurenceCacheItem1.compareNoTweetsSent(tweetKeywordOccurenceCacheItem2);
//    return tweetKeywordOccurenceCacheItem1.compareKeywords(tweetKeywordOccurenceCacheItem2);
}

class TweetKeywordUsernameMentionedCacheItem {
public:
    TweetKeywordUsernameMentionedCacheItem() {}
    
    ~TweetKeywordUsernameMentionedCacheItem() {}
    
    TweetKeywordUsernameMentionedCacheItem operator=(const TweetKeywordUsernameMentionedCacheItem& tc) {
        if (this == &tc) {
            return *this;
        }
        else {
            initialTweet = tc.initialTweet;
            influencedTweet= tc.influencedTweet;
            return *this;
        }
    }
    
    TweetKeywordUsernameMentionedCacheItem(Tweet initialTweet, Tweet influencedTweet) {
        this->initialTweet = initialTweet;
        this->influencedTweet = influencedTweet;
    }
    
    Tweet getInitialTweet() const {
        return initialTweet;
    }
    
    Tweet getInfluencedTweet() const {
        return influencedTweet;
    }
    
    void setInitialTweet(const Tweet& initialTweet) {
        this->initialTweet = initialTweet;
    }
    
    void setInfluencedTweet(const Tweet& influencedTweet) {
        this->influencedTweet = influencedTweet;
    }
    
    string toString() const {
        return initialTweet.toString() + "\n" + influencedTweet.toString();
    }
    
private:
    Tweet initialTweet;
    Tweet influencedTweet;
};

class TweetAnalyser {
public:
    TweetAnalyser() {}
    
    ~TweetAnalyser() {}
    
    TweetAnalyser operator=(const TweetAnalyser& tc) {
        if (this == &tc) {
            return *this;
        }
        else {
            return *this;
        }
    }
    
    // read in a parsed tweet data file
    vector<Tweet> readParsedTweetDataFromFile(string parsedTweetDataFilename) {
        vector<Tweet> parsedTweetData;
        ifstream fInp;
        string screenName, timeStamp, content, mentions;
        string idString;
        vector<string> mentions_v;
        fInp.open(parsedTweetDataFilename.c_str());
        if (fInp.is_open()) {
            while (!fInp.eof()) {
                getline(fInp, idString);
                getline(fInp, screenName);
                getline(fInp, timeStamp);
                getline(fInp, content);
                getline(fInp, mentions);
                mentions_v = extractMentionsAndStoreFromString(mentions);
                int id = atoi(idString.c_str());
                parsedTweetData.push_back(Tweet(id, screenName, timeStamp, content, mentions_v));
            }
            fInp.close();
            mentions_v.clear();
        }
        else {
            cout << "error opening file: " << parsedTweetDataFilename << endl;
        }
        return parsedTweetData;
    }
    
    // print to console output data for the tweets specified in input
    void printTweetData(vector<Tweet> tweetData) {
        for (int i = 0; i < tweetData.size(); i++) {
            cout << "[" << i << "] -> " << tweetData.at(i).toString() << endl;
        }
    }
    
    // note: NEED to sort Tweet objects by screenName !!!
    // return all distinct screen names with number of tweets sent from each screen name, as well as id's of all tweets sent
    vector<TweetScreenNameOccurenceCacheItem> extractAmountTweetDataForAllScreenNames(const vector<Tweet> tweetData) const {
        string screenName;
        string screenNamePreviousTweet;
        int noDistinctScreenNames = 0;
        vector<TweetScreenNameOccurenceCacheItem> tweetScreenNameOccurenceCache;
        vector<Tweet> sortedTweetData = tweetData;
        vector<int> tweetIds;
        sort(sortedTweetData.begin(), sortedTweetData.end());
        for (int i = 0; i < sortedTweetData.size(); i++) {
            screenNamePreviousTweet = screenName;
            screenName = sortedTweetData.at(i).getScreenName();
            // if screen name has not occurred previously
            if (screenName.compare(screenNamePreviousTweet) != 0) {
                tweetIds.clear();
                tweetIds.push_back(sortedTweetData.at(i).getId());
                tweetScreenNameOccurenceCache.push_back(TweetScreenNameOccurenceCacheItem(screenName, 1, tweetIds));
                noDistinctScreenNames++;
            }
            // if screen name has not occurred previously
            else {
                tweetIds = tweetScreenNameOccurenceCache.at(noDistinctScreenNames-1).getTweetIds();
                tweetIds.push_back(sortedTweetData.at(i).getId());
                tweetScreenNameOccurenceCache.at(noDistinctScreenNames-1).incrementNoTweetsSent();
                tweetScreenNameOccurenceCache.at(noDistinctScreenNames-1).setTweetIds(tweetIds);
            }
        }
        return tweetScreenNameOccurenceCache;
    }
    
    // return, for each keyword, the number of tweets that were sent that contained that keyword, as well as id's of all tweets sent
    vector<TweetKeywordOccurenceCacheItem> extractAmountTweetDataForKeywords(const vector<Tweet> tweetData, vector<string> keywords) {
        vector<TweetKeywordOccurenceCacheItem> tweetKeywordOccurenceCache;
        vector<Tweet> keywordsContainedTweetData;
        for (int i = 0; i < keywords.size(); i++) {
            vector<int> tweetIds;
            string keyword = keywords.at(i);
            vector<string> keyword_v;
            keyword_v.push_back(keyword);
            keywordsContainedTweetData = extractKeywordsContainedTweetData(tweetData, keyword_v, OR);
            for (int j = 0; j < keywordsContainedTweetData.size(); j++) {
                int id = keywordsContainedTweetData.at(j).getId();
                tweetIds.push_back(id);
            }
            tweetKeywordOccurenceCache.push_back(TweetKeywordOccurenceCacheItem(keyword, (int) keywordsContainedTweetData.size(), tweetIds));
            keywordsContainedTweetData.clear();
        }
        return tweetKeywordOccurenceCache;
    }
    
    // return, for each screen name, all tweets sent by the account
    vector<Tweet> extractSpecificScreenNameTweetData(const vector<Tweet> tweetData, vector<string> screenNames) {
        vector<Tweet> specificScreenNameTweetData;
        // false => case insensitive, true => case sensitive
        bool caseSensitivity = true;
        bool tweetSelectionOR = false;
        string tweetScreenName;
        for (int i = 0; i < tweetData.size(); i++) {
            tweetScreenName = tweetData.at(i).getScreenName();
            if (!caseSensitivity) {
                transform(tweetScreenName.begin(), tweetScreenName.end(), tweetScreenName.begin(), ::tolower);
            }
            for (int j = 0; j < screenNames.size(); j++) {
                if (j != screenNames.size()-1) {
                    if (tweetScreenName.compare(screenNames.at(j)) == 0) {
                        tweetSelectionOR = true;
                    }
                }
                else {
                    if ((tweetScreenName.compare(screenNames.at(j)) == 0) || tweetSelectionOR) {
                        specificScreenNameTweetData.push_back(tweetData.at(i));
                    }
                }
            }
            tweetSelectionOR = false;
        }
        return specificScreenNameTweetData;
    }
    
    // return, for a combination of the keywords (depending on the option), all tweets containing the combination
    vector<Tweet> extractKeywordsContainedTweetData(const vector<Tweet> tweetData, vector<string> keywords, tweetAnalyserKeywordSearchOption option) {
        vector<Tweet> keywordsContainedTweetData;
        // false => case insensitive, true => case sensitive
        bool caseSensitivity = false;
        bool tweetSelectionOR = false;
        bool tweetSelectionAND = false;
        string tweet;
        switch (option) {
        case OR:
            for (int i = 0; i < tweetData.size(); i++) {
                tweet = tweetData.at(i).getContent();
                if (!caseSensitivity) {
                    transform(tweet.begin(), tweet.end(), tweet.begin(), ::tolower);
                }
                for (int j = 0; j < keywords.size(); j++) {
                    if (j != keywords.size()-1) {
                        if (tweet.find(keywords.at(j)) != string::npos) {
                            tweetSelectionOR = true;
                        }
                    }
                    else {
                        if ((tweet.find(keywords.at(j)) != string::npos) || tweetSelectionOR) {
                            keywordsContainedTweetData.push_back(tweetData.at(i));
                        }
                    }
                }
                tweetSelectionOR = false;
            }
            break;
        case AND:
            tweetSelectionAND = true;
            for (int i = 0; i < tweetData.size(); i++) {
                tweet = tweetData.at(i).getContent();
                if (!caseSensitivity) {
                    transform(tweet.begin(), tweet.end(), tweet.begin(), ::tolower);
                }
                for (int j = 0; j < keywords.size(); j++) {
                    if (j != keywords.size()-1) {
                        if (tweet.find(keywords.at(j)) != string::npos) {
                            tweetSelectionAND = tweetSelectionAND && true;
                        }
                        else {
                            tweetSelectionAND = false;
                        }
                    }
                    else {
                        if ((tweet.find(keywords.at(j)) != string::npos) && tweetSelectionAND) {
                            keywordsContainedTweetData.push_back(tweetData.at(i));
                        }
                    }
                }
                tweetSelectionAND = true;
            }
            break;
        // search option is OR by default
        default:
            for (int i = 0; i < tweetData.size(); i++) {
                tweet = tweetData.at(i).getContent();
                if (!caseSensitivity) {
                    transform(tweet.begin(), tweet.end(), tweet.begin(), ::tolower);
                }
                for (int j = 0; j < keywords.size(); j++) {
                    if (j != keywords.size()-1) {
                        if (tweet.find(keywords.at(j)) != string::npos) {
                            tweetSelectionOR = true;
                        }
                    }
                    else {
                        if ((tweet.find(keywords.at(j)) != string::npos) || tweetSelectionOR) {
                            keywordsContainedTweetData.push_back(tweetData.at(i));
                        }
                    }
                }
                tweetSelectionOR = false;
            }
            break;
        }
        return keywordsContainedTweetData;
    }
    
    // return all tweets corresponding to the ids specified.
    vector<Tweet> extractSpecificIdTweetData(const vector<Tweet> tweetData, vector<int> tweetIds) {
        vector<Tweet> specificIdTweetData;
        int id;
        for (int i = 0; i < tweetData.size(); i++) {
            id = tweetData.at(i).getId();
            for (int j = 0; j < tweetIds.size(); j++) {
                if (tweetIds.at(j) == id) {
                    specificIdTweetData.push_back(tweetData.at(i));
                }
            }
        }
        return specificIdTweetData;
    }
    
    // return tweets that have been @username mentioned by and to users, that have actively tweeted, and messages contains combination of the keywords (depending on the option),
    vector<TweetKeywordUsernameMentionedCacheItem> extractKeywordsContainedUsernameMentionedTweetData(const vector<Tweet> tweetData, vector<string> keywords, tweetAnalyserKeywordSearchOption option) {
        vector<TweetKeywordUsernameMentionedCacheItem> keywordsContainedUsernameMentionedTweetData;
        vector<Tweet> keywordsContainedTweetData = extractKeywordsContainedTweetData(tweetData, keywords, option);
        for (int i = 0; i < keywordsContainedTweetData.size(); i++) {
            vector<string> mentions = keywordsContainedTweetData.at(i).getMentions();
            for (int j = 0; j < mentions.size(); j++) {
                string mention = mentions.at(j);
                if (mention.compare(keywordsContainedTweetData.at(i).getScreenName()) != 0) {
                    vector<string> screenNames;
                    screenNames.push_back(mention);
                    vector<Tweet> tweetDataSentByMentionedScreenName = extractSpecificScreenNameTweetData(keywordsContainedTweetData, screenNames);
                    int numberTweetsSentByMentionedScreenName = (int) tweetDataSentByMentionedScreenName.size();
                    Tweet influencedTweet = keywordsContainedTweetData.at(i);
                    Tweet initialTweet;
                    if (numberTweetsSentByMentionedScreenName != 0) {
                        initialTweet = tweetDataSentByMentionedScreenName.at(0);
                        // initial tweet should be sent before the influenced tweet
                        if (initialTweet.compareTimeStamps(influencedTweet))
                            keywordsContainedUsernameMentionedTweetData.push_back(TweetKeywordUsernameMentionedCacheItem(initialTweet, influencedTweet));
                    }
                    screenNames.clear();
                }
            }
        }
        keywordsContainedTweetData.clear();
        return keywordsContainedUsernameMentionedTweetData;
    }
    
private:
    vector<string> extractMentionsAndStoreFromString(string mentions) {
        vector<string> mentions_v;
        if (mentions == "") {
            return mentions_v;
        }
        else {
            while (mentions != "") {
                size_t firstMentionScreenNameEndPosition = mentions.find(mentionPostMentionAsString);
                if (firstMentionScreenNameEndPosition != string::npos) {
                    // extract and store first mention
                    int firstMentionScreenNameStringLength = (int) firstMentionScreenNameEndPosition;
                    string screenName = mentions.substr(0, firstMentionScreenNameStringLength);
                    mentions_v.push_back(screenName);
                    // remove first mention and separator from input string
                    mentions.erase(mentions.begin(), mentions.begin()+firstMentionScreenNameStringLength+mentionPostMentionAsString.length());
                }
            }
            return mentions_v;
        }
    }
    
    const string mentionPostMentionAsString = ";";
};

int main(int argc, const char * argv[]){
//    string inputTweetDataFilename = "3_tweets.txt";
//    string inputTweetDataFilename = "100_tweets.txt";
    string inputTweetDataFilename = "100k_tweets.txt";
    
//    string parsedTweetDataFilename = "3_tweets_parsed.txt";
//    string parsedTweetDataFilename = "100_tweets_parsed.txt";
    string parsedTweetDataFilename = "100k_tweets_parsed.txt";
    
    vector<string> keywords;
    keywords.push_back("london");
    tweetAnalyserKeywordSearchOption option = OR;
    
    vector <string> screenNames;
    screenNames.push_back("dN0t");
    
    vector<int> tweetIds;
    tweetIds.push_back(1);
    tweetIds.push_back(57005);
    tweetIds.push_back(99999);

    /* parse tweet data to a cleaner format */
//    TweetParser tweetParser;
//    tweetParser.parseTweetData(inputTweetDataFilename);
//    tweetParser.writeParsedTweetDataToFile(parsedTweetDataFilename);
    
    /* analysed parsed tweet data */
    TweetAnalyser tweetAnalyser;
    vector<Tweet> parsedTweetData = tweetAnalyser.readParsedTweetDataFromFile(parsedTweetDataFilename);
    
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
//    tweetAnalyser.printTweetData(twitterKeywordSelectionData);
//    twitterKeywordSelectionData.clear();
    
     /* test functionality: return tweet data of tweets sent by certain accounts */
//    vector<Tweet> twitterSpecificScreenNameData = tweetAnalyser.extractSpecificScreenNameTweetData(parsedTweetData, screenNames);
//    cout << "##########       TESTING SCREENAME SECTION         #########" << endl;
//    for (int i = 0; i < screenNames.size(); i++) {
//        if (i == 0 && i != screenNames.size()-1)
//            cout << "##########      TESTING SCREENAMES: " << screenNames.at(i);
//        else if (i == 0 && i == screenNames.size()-1)
//            cout << "##########      TESTING SCREENAMES: " << screenNames.at(i) << "      #########" << endl;
//        else if (i != 0 && i != screenNames.size()-1)
//            cout << ", " << screenNames.at(i);
//        else
//            cout << ", " << screenNames.at(i) << "      #########" << endl;
//    }
//    cout << "##########         NUMBER OF TWEETS SELECTED: " << twitterSpecificScreenNameData.size() << "         #########" << endl;
//    sort(twitterSpecificScreenNameData.begin(), twitterSpecificScreenNameData.end());
//    tweetAnalyser.printTweetData(twitterSpecificScreenNameData);
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
//    tweetAnalyser.printTweetData(twitterSpecificIdData);
//    twitterSpecificIdData.clear();
    
    /* test functionality: return statistics (number of tweets) sent that contain certain keywords */
//    vector<TweetKeywordOccurenceCacheItem> tweetKeywordOccurenceCache = tweetAnalyser.extractAmountTweetDataForKeywords(parsedTweetData, keywords);
//    cout << "##########       TESTING KEYWORD OCURENCE CACHE SECTION         #########" << endl;
//    cout << "##########         NUMBER OF KEYWORDS TESTING: " << keywords.size() << "         #########" << endl;
//    sort(tweetKeywordOccurenceCache.begin(), tweetKeywordOccurenceCache.end());
//    for (int i = 0; i < tweetKeywordOccurenceCache.size(); i++) {
//        cout << "[" << i << "] -> " << tweetKeywordOccurenceCache.at(i).toString() << endl;
//    }
//    tweetKeywordOccurenceCache.clear();
    
     /* test functionality: return statistics (number of tweets) sent by all distinct accounts */
//    vector<TweetScreenNameOccurenceCacheItem> tweetScreenNameOccurenceCache = tweetAnalyser.extractAmountTweetDataForAllScreenNames(parsedTweetData);
//    cout << "##########       TESTING SCREENAME OCURENCE CACHE SECTION         #########" << endl;
//    cout << "##########         NUMBER OF DISTINCT SCERENNAMES: " << tweetScreenNameOccurenceCache.size() << "         #########" << endl;
//    sort(tweetScreenNameOccurenceCache.begin(), tweetScreenNameOccurenceCache.end());
//    for (int i = 0; i < tweetScreenNameOccurenceCache.size(); i++) {
//        cout << "[" << i << "] -> " << tweetScreenNameOccurenceCache.at(i).toString() << endl;
//    }
//    tweetScreenNameOccurenceCache.clear();
    
    /* test functionality: return tweets that have been @username mentioned by and to users, that have actively tweeted, and messages contains combination of the keywords (depending on the option) */
    vector<TweetKeywordUsernameMentionedCacheItem> twitterKeywordContainedUsernameMentionedCache = tweetAnalyser.extractKeywordsContainedUsernameMentionedTweetData(parsedTweetData, keywords, option);
    cout << "##########       TESTING KEYWORD USERNAME MENTIONED SECTION         #########" << endl;
    for (int i = 0; i < keywords.size(); i++) {
        if (i == 0 && i != keywords.size()-1)
            cout << "##########      TESTING KEYWORDS with option '" << option << "': " << keywords.at(i);
        else if (i == 0 && i == keywords.size()-1)
            cout << "##########      TESTING KEYWORDS with option '" << option << "': " << keywords.at(i) << "      #########" << endl;
        else if (i != 0 && i != keywords.size()-1)
            cout << ", " << keywords.at(i);
        else
            cout << ", " << keywords.at(i) << "      #########" << endl;
    }
    cout << "##########         NUMBER OF TWEETS SELECTED: " << twitterKeywordContainedUsernameMentionedCache.size() << "         #########" << endl;
    for (int i = 0; i < twitterKeywordContainedUsernameMentionedCache.size(); i++) {
        cout << "[" << i << "] -> " << twitterKeywordContainedUsernameMentionedCache.at(i).toString() << endl;
    }
    twitterKeywordContainedUsernameMentionedCache.clear();
    
    parsedTweetData.clear();
    keywords.clear();
    screenNames.clear();
    tweetIds.clear();
    
    return 0;
}