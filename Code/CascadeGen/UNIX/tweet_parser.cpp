#include "tweet_parser.h"

/* TweetParser class methods */

TweetParser::TweetParser() {
    tweetPreScreenNameAsString = "\"screen_name\":\"";
    tweetPostScreenNameAsString = "\",\"";
    tweetProfileSidebarFillColorAsString = "profile_sidebar_fill_color";
    tweetPreTimeStampAsString = "\"created_at\":\"";
    tweetPostTimeStampAsString = "\",\"";
    tweetPreContentAsString = "\"text\":\"";
    tweetPostContentAsString = "\",\"";
    tweetPreMentionsAsString = "\"user_mentions\":[";
    tweetPostMentionsAsString = "]}";
    mentionPreMentionAsString = "{";
    mentionPostMentionAsString = "\"}";
    mentionPreScreenNameAsString = "\"screen_name\":\"";
    mentionPostScreenNameAsString = "\",\"";
}

TweetParser::~TweetParser() {
    parsedTweetData.clear();
}

TweetParser TweetParser::operator=(const TweetParser& tc) {
    if (this == &tc) {
        return *this;
    }
    else {
        parsedTweetData = tc.parsedTweetData;
        tweetPreScreenNameAsString = "\"screen_name\":\"";
        tweetPostScreenNameAsString = "\",\"";
        tweetProfileSidebarFillColorAsString = "profile_sidebar_fill_color";
        tweetPreTimeStampAsString = "\"created_at\":\"";
        tweetPostTimeStampAsString = "\",\"";
        tweetPreContentAsString = "\"text\":\"";
        tweetPostContentAsString = "\",\"";
        tweetPreMentionsAsString = "\"user_mentions\":[";
        tweetPostMentionsAsString = "]}";
        mentionPreMentionAsString = "{";
        mentionPostMentionAsString = "\"}";
        mentionPreScreenNameAsString = "\"screen_name\":\"";
        mentionPostScreenNameAsString = "\",\"";
        return *this;
    }
}

vector<Tweet> TweetParser::parseTweetData(string inputTweetDataFilename) {
    ifstream fInp;
    vector<Tweet> parsedTweetData;
    string tweet;
    fInp.open(inputTweetDataFilename.c_str());
    if (fInp.is_open()) {
        int id = 0;
        int screenNameId = 0;
        int resetScreenNameId = 0;
        string tweetScreenName, tweetTimeStamp, tweetContent;
        vector<string> tweetMentions;
        vector<string> screenNames;
        bool incrementScreenNameId = true;
        while (!fInp.eof()) {
            getline(fInp, tweet);
            if (tweet != "") {
                try {
                    tweetScreenName = getScreenNameForSingleTweet(tweet);
                    tweetTimeStamp = getTimeStampForSingleTweet(tweet);
                    tweetContent = getContentForSingleTweet(tweet);
                    tweetMentions = getMentionsForSingleTweet(tweet);
                } catch (const char* msg) {
                    cerr << msg << endl;
                }
                vector<string>::iterator it = find(screenNames.begin(), screenNames.end(), tweetScreenName);
                // new screen name found
                if (it == screenNames.end()) {
                    screenNames.push_back(tweetScreenName);
                    incrementScreenNameId = true;
                }
                // screen name already found
                else {
                    resetScreenNameId = screenNameId;
                    screenNameId = (int) (it - screenNames.begin());
                    incrementScreenNameId = false;
                }
                parsedTweetData.push_back(Tweet(id, TwitterAccount(screenNameId, tweetScreenName), tweetTimeStamp, tweetContent, tweetMentions));
                if (incrementScreenNameId) {
                    screenNameId++;
                }
                else {
                    screenNameId = resetScreenNameId;
                }
                id++;
            }
        }
        fInp.close();
        tweetMentions.clear();
        screenNames.clear();
        cout << "######   *****   NUMBER OF TWEETS = " << parsedTweetData.size() << "   *****   ######" << endl;
        cout << "######   *****   NUMBER OF TWITTER ACCOUNTS = " << screenNameId << "   *****   ######" << endl;
        return parsedTweetData;
    }
    else {
        string message = "TweetParser::parseTweetData: error opening file: " + inputTweetDataFilename;
        throw message.c_str();
    }
}

void TweetParser::writeParsedTweetDataToFile(string parsedTweetDataFilename, vector<Tweet> parsedTweetData) {
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
        string message = "TweetParser::writeParsedTweetDataToFile: error opening file: " + parsedTweetDataFilename;
        throw message.c_str();
    }
}

string TweetParser::getScreenNameForSingleTweet(string tweet) const {
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
                string message = "TweetParser::getScreenNameForSingleTweet: error with tweetScreenNameStringEndPosition, position = " + convertIntToString((int) tweetScreenNameStringEndPosition);
                throw message.c_str();
            }
        }
        else {
            string message = "TweetParser::getScreenNameForSingleTweet: error with tweetScreenNameStringStartPosition, position = " + convertIntToString((int) tweetScreenNameStringStartPosition);
            throw message.c_str();
        }
    }
    else {
        string message = "TweetParser::getScreenNameForSingleTweet: error with tweetProfileSidebarFillColorStringPosition, position = " + convertIntToString((int) tweetProfileSidebarFillColorStringPosition);
        throw message.c_str();
    }
}

string TweetParser::getTimeStampForSingleTweet(string tweet) const {
    size_t stringStringStartPosition = tweet.find(tweetPreTimeStampAsString);
    
    if (stringStringStartPosition != string::npos) {
        size_t stringStringEndPosition = tweet.find(tweetPostTimeStampAsString,stringStringStartPosition+1);
        if (stringStringEndPosition != string::npos) {
            int stringStringLength = (int) stringStringEndPosition - (int) stringStringStartPosition - (int) tweetPreTimeStampAsString.length();
            string timeStamp = tweet.substr(stringStringStartPosition+tweetPreTimeStampAsString.length(), stringStringLength);
            return timeStamp;
        }
        else {
            string message = "TweetParser::getTimeStampForSingleTweet: error with stringStringEndPosition, position = " + convertIntToString((int) stringStringEndPosition);
            throw message.c_str();
        }
    }
    else {
        string message = "TweetParser::getTimeStampForSingleTweet: error with stringStringStartPosition, position = " + convertIntToString((int) stringStringStartPosition);
        throw message.c_str();
    }
}

string TweetParser::getContentForSingleTweet(string tweet) const {
    size_t tweetContentStringStartPosition = tweet.find(tweetPreContentAsString);
    
    if (tweetContentStringStartPosition != string::npos) {
        size_t tweetContentStringEndPosition = tweet.find(tweetPostContentAsString,tweetContentStringStartPosition+1);
        if (tweetContentStringEndPosition != string::npos) {
            int tweetContentStringLength = (int) tweetContentStringEndPosition - (int) tweetContentStringStartPosition - (int) tweetPreContentAsString.length();
            string content = tweet.substr(tweetContentStringStartPosition+tweetPreContentAsString.length(), tweetContentStringLength);
            return content;
        }
        else {
            string message = "TweetParser::getContentForSingleTweet: error with tweetContentStringEndPosition, position = " + convertIntToString((int) tweetContentStringEndPosition);
            throw message.c_str();
        }
    }
    else {
        string message = "TweetParser::getContentForSingleTweet: error with tweetContentStringStartPosition, position = " + convertIntToString((int) tweetContentStringStartPosition);
        throw message.c_str();
    }
}

vector<string> TweetParser::getMentionsForSingleTweet(string tweet) const {
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
                    string firstMention;
                    try {
                        firstMention = eraseAndReturnFirstMention(mentions);
                    } catch (const char* msg) {
                        cerr << msg << endl;
                    }
                    mentions_v.push_back(firstMention);
                }
                return mentions_v;
            }
        }
        else {
            string message = "TweetParser::getMentionsForSingleTweet: error with tweetMentionsStringEndPosition, position = " + convertIntToString((int) tweetMentionsStringEndPosition);
            throw message.c_str();
        }
    }
    else {
        string message = "TweetParser::getMentionsForSingleTweet: error with tweetMentionsStringStartPosition, position = " + convertIntToString((int) tweetMentionsStringStartPosition);
        throw message.c_str();
    }
}

// takes as input a string of mentions, and returns the FIRST mention in the string, also DELETING the mention from input
string TweetParser::eraseAndReturnFirstMention(string& mentions) const {
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
                else {
                    string message = "TweetParser::eraseAndReturnFirstMention: error with firstMentionScreenNameEndPosition, position = " + convertIntToString((int) firstMentionScreenNameEndPosition);
                    throw message.c_str();
                }
            }
            else {
                string message = "TweetParser::eraseAndReturnFirstMention: error with firstMentionScreenNameStartPosition, position = " + convertIntToString((int) firstMentionScreenNameStartPosition);
                throw message.c_str();
            }
        }
        else {
            string message = "TweetParser::eraseAndReturnFirstMention: error with firstMentionEndPosition, position = " + convertIntToString((int) firstMentionEndPosition);
            throw message.c_str();
        }
    }
    // assume no mentions left
    else {
        string message = "TweetParser::eraseAndReturnFirstMention: error with firstMentionStartPosition, position = " + convertIntToString((int) firstMentionStartPosition);
        throw message.c_str();
    }
}