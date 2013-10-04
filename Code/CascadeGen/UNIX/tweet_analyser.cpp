#include "tweet_analyser.h"

/* TweetAnalyser class methods */

TweetAnalyser::TweetAnalyser() {
    mentionPostMentionAsString = ";";
}

TweetAnalyser::~TweetAnalyser() {}

TweetAnalyser TweetAnalyser::operator=(const TweetAnalyser& tc) {
    if (this == &tc) {
        return *this;
    }
    else {
        mentionPostMentionAsString = ";";
        return *this;
    }
}


// read in a parsed tweet data file
vector<Tweet> TweetAnalyser::readParsedTweetDataFromFile(string parsedTweetDataFilename) {
    vector<Tweet> parsedTweetData;
    ifstream fInp;
    string idString, screenName, screenNameIdString, timeStamp, content, mentions;
    vector<string> mentions_v;
    fInp.open(parsedTweetDataFilename.c_str());
    if (fInp.is_open()) {
        while (!fInp.eof()) {
            getline(fInp, idString);
            getline(fInp, screenName);
            getline(fInp, screenNameIdString);
            getline(fInp, timeStamp);
            getline(fInp, content);
            getline(fInp, mentions);
            mentions_v = extractMentionsAndStoreFromString(mentions);
            int id = atoi(idString.c_str());
            int screenNameId = atoi(screenNameIdString.c_str());
            parsedTweetData.push_back(Tweet(id, TwitterAccount(screenNameId, screenName), timeStamp, content, mentions_v));
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
void TweetAnalyser::printTweetData(vector<Tweet> tweetData) {
    for (int i = 0; i < tweetData.size(); i++) {
        cout << "[" << i << "] -> " << tweetData.at(i).toString() << endl;
    }
}

// note: NEED to sort Tweet objects by screenName !!!
// return all distinct screen names with number of tweets sent from each screen name, as well as id's of all tweets sent
vector<TweetScreenNameOccurenceCacheItem> TweetAnalyser::extractAmountTweetDataForAllScreenNames(const vector<Tweet> tweetData) const {
    string screenName;
    string screenNamePreviousTweet;
    int noDistinctScreenNames = 0;
    vector<TweetScreenNameOccurenceCacheItem> tweetScreenNameOccurenceCache;
    vector<Tweet> sortedTweetData = tweetData;
    vector<int> tweetIds;
    sort(sortedTweetData.begin(), sortedTweetData.end());
    for (int i = 0; i < sortedTweetData.size(); i++) {
        screenNamePreviousTweet = screenName;
        screenName = sortedTweetData.at(i).getAccount().getScreenName();
        // if screen name has not occurred previously
        if (screenName.compare(screenNamePreviousTweet) != 0) {
            tweetIds.clear();
            tweetIds.push_back(sortedTweetData.at(i).getId());
            tweetScreenNameOccurenceCache.push_back(TweetScreenNameOccurenceCacheItem(screenName, noDistinctScreenNames, 1, tweetIds));
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
vector<TweetKeywordOccurenceCacheItem> TweetAnalyser::extractAmountTweetDataForKeywords(const vector<Tweet> tweetData, vector<string> keywords) {
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
vector<Tweet> TweetAnalyser::extractSpecificScreenNameTweetData(const vector<Tweet> tweetData, vector<string> screenNames) {
    vector<Tweet> specificScreenNameTweetData;
    // false => case insensitive, true => case sensitive
    bool caseSensitivity = true;
    bool tweetSelectionOR = false;
    string tweetScreenName;
    for (int i = 0; i < tweetData.size(); i++) {
        tweetScreenName = tweetData.at(i).getAccount().getScreenName();
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
vector<Tweet> TweetAnalyser::extractKeywordsContainedTweetData(const vector<Tweet> tweetData, vector<string> keywords, tweetAnalyserKeywordSearchOption option) {
    vector<Tweet> keywordsContainedTweetData;
    // false => case insensitive, true => case sensitive
    bool caseSensitivity = false;
    bool tweetSelectionOR = false;
    bool tweetSelectionAND = false;
    bool tweetSelectionNOT = false;
    string tweet;
    switch (option) {
            // return tweets that contain AT LEAST ONE keywords
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
            // return tweets that contain ALL keywords
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
            // return tweets that contain NONE of the keywords
        case NOT:
            tweetSelectionNOT = true;
            for (int i = 0; i < tweetData.size(); i++) {
                tweet = tweetData.at(i).getContent();
                if (!caseSensitivity) {
                    transform(tweet.begin(), tweet.end(), tweet.begin(), ::tolower);
                }
                for (int j = 0; j < keywords.size(); j++) {
                    if (j != keywords.size()-1) {
                        if (tweet.find(keywords.at(j)) == string::npos) {
                            tweetSelectionNOT = tweetSelectionNOT && true;
                        }
                        else {
                            tweetSelectionNOT = false;
                        }
                    }
                    else {
                        if ((tweet.find(keywords.at(j)) == string::npos) && tweetSelectionNOT) {
                            keywordsContainedTweetData.push_back(tweetData.at(i));
                        }
                    }
                }
                tweetSelectionNOT = true;
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
vector<Tweet> TweetAnalyser::extractSpecificIdTweetData(const vector<Tweet> tweetData, vector<int> tweetIds) {
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
vector<TweetKeywordUsernameMentionedCacheItem> TweetAnalyser::extractKeywordsContainedUsernameMentionedTweetData(const vector<Tweet> tweetData, vector<string> keywords, tweetAnalyserKeywordSearchOption option) {
    vector<TweetKeywordUsernameMentionedCacheItem> keywordsContainedUsernameMentionedTweetData;
    vector<Tweet> keywordsContainedTweetData = extractKeywordsContainedTweetData(tweetData, keywords, option);
    for (int i = 0; i < keywordsContainedTweetData.size(); i++) {
        vector<string> mentions = keywordsContainedTweetData.at(i).getMentions();
        string screenName = keywordsContainedTweetData.at(i).getAccount().getScreenName();
        Tweet influencedTweet = keywordsContainedTweetData.at(i);
        for (int j = 0; j < mentions.size(); j++) {
            string mention = mentions.at(j);
            if (mention.compare(screenName) != 0) {
                vector<string> screenNames;
                screenNames.push_back(mention);
                vector<Tweet> tweetDataSentByMentionedScreenName = extractSpecificScreenNameTweetData(keywordsContainedTweetData, screenNames);
                int numberTweetsSentByMentionedScreenName = (int) tweetDataSentByMentionedScreenName.size();
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

vector<string> TweetAnalyser::extractMentionsAndStoreFromString(string mentions) {
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

vector<Tweet> TweetAnalyser::extractOnlyFirstInfectionsTweetData(const vector<Tweet> tweetData) {
    // sort tweets by screenName
    vector<Tweet> sortedTweetData = tweetData;
    sort(sortedTweetData.begin(), sortedTweetData.end());
    vector<Tweet> onlyFirstInfectionsTweetData;
    string screenName, previousScreenName;
    for (int i = 0; i < tweetData.size(); i++) {
        previousScreenName = screenName;
        screenName = tweetData.at(i).getAccount().getScreenName();
        if (previousScreenName.compare(screenName) != 0) {
            onlyFirstInfectionsTweetData.push_back(tweetData.at(i));
        }
    }
    sortedTweetData.clear();
    return onlyFirstInfectionsTweetData;
}

vector<Cascade> TweetAnalyser::generateCascades(const vector<Tweet> tweetData, vector<string> keywords) {
    vector<Cascade> cascades;
    for (int i = 0; i < keywords.size(); i++) {
        cascades.push_back(generateCascade(tweetData, keywords.at(i)));
    }
    return cascades;
}

Cascade TweetAnalyser::generateCascade(const vector<Tweet> tweetData, string keyword) {
    Cascade cascade;
    vector<string> keywords;
    keywords.push_back(keyword);
    vector<Tweet> onlyFirstInfectionsKeywordsContainedTweetData = extractOnlyFirstInfectionsTweetData(extractKeywordsContainedTweetData(tweetData, keywords, OR));
    for (int i = 0; i < onlyFirstInfectionsKeywordsContainedTweetData.size(); i++) {
        Tweet tweet = onlyFirstInfectionsKeywordsContainedTweetData.at(i);
        TwitterAccount account = TwitterAccount(tweet.getAccount());
        cascade.addCascadeItem(CascadeItem(account, tweet.getTimeStamp()));
    }
    keywords.clear();
    return cascade;
}

vector<TwitterAccount> TweetAnalyser::generateTwitterAccounts(const vector<Tweet> tweetData) {
    vector<TwitterAccount> accounts;
    vector<TweetScreenNameOccurenceCacheItem> screenNameOccurenceCache = extractAmountTweetDataForAllScreenNames(tweetData);
    for (int i = 0; i < screenNameOccurenceCache.size(); i++) {
        accounts.push_back(TwitterAccount(screenNameOccurenceCache.at(i).getScreenNameId(), screenNameOccurenceCache.at(i).getScreenName()));
    }
    screenNameOccurenceCache.clear();
    return accounts;
}

void TweetAnalyser::writeCascadesDataToFile(string casacdesDataFilename, const vector<Tweet> tweetData, vector<string> keywords) {
    ofstream fOut;
    fOut.open(casacdesDataFilename.c_str(), fstream::out);
    if (fOut.is_open()) {
        vector<TwitterAccount> accounts = generateTwitterAccounts(tweetData);
        for (int i = 0; i < accounts.size(); i++) {
            fOut << accounts.at(i).toString() << "\n";
        }
        fOut << "\n";
        vector<Cascade> cascades = generateCascades(tweetData, keywords);
        for (int j = 0; j < cascades.size(); j++) {
            if (j != cascades.size()-1) {
                fOut << cascades.at(j).toString() << "\n";
            }
            else {
                fOut << cascades.at(j).toString();
            }
        }
        fOut.close();
        accounts.clear();
        cascades.clear();
    }
    else {
        cout << "error opening file: " << casacdesDataFilename << endl;
    }
}
