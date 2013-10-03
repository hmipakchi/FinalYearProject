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

vector<Tweet> TweetParser::getParsedTweetData() const {
    return parsedTweetData;
}

void TweetParser::parseTweetData(string inputTweetDataFilename) {
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

void TweetParser::writeParsedTweetDataToFile(string parsedTweetDataFilename) {
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
            cout << "error with stringStringEndPosition, position = " << stringStringEndPosition << endl;
            return "";
        }
    }
    else {
        cout << "error with stringStringStartPosition, position = " << stringStringStartPosition << endl;
        return "";
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
            cout << "error with tweetContentStringEndPosition, position = " << tweetContentStringEndPosition << endl;
            return "";
        }
    }
    else {
        cout << "error with tweetContentStringStartPosition, position = " << tweetContentStringStartPosition << endl;
        return "";
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
