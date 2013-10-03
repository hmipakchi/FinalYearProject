#include "tweet.h"

/* Tweet class methods */

Tweet::Tweet() {
    timeStampFormat = "%a %b %d %T %z %Y";
}
    
Tweet::~Tweet() {
    mentions.clear();
}

Tweet::Tweet(const Tweet& tc):id(tc.id), screenName(tc.screenName), timeStamp(tc.timeStamp), content(tc.content), mentions(tc.mentions), timeStampFormat(tc.timeStampFormat) {}

Tweet::Tweet(int id, string screenName, string timeStamp, string content, vector<string> mentions) {
    this->id = id;
    this->screenName = screenName;
    this->timeStamp = timeStamp;
    this->content = content;
    this->mentions = mentions;
    this->timeStampFormat = "%a %b %d %T %z %Y";
}

Tweet Tweet::operator=(const Tweet& tc) {
    if (this == &tc) {
        return *this;
    }
    else {
        id = tc.id;
        screenName = tc.screenName;
        timeStamp = tc.timeStamp;
        content = tc.content;
        mentions = tc.mentions;
        timeStampFormat = "%a %b %d %T %z %Y";
        return *this;
    }
}

void Tweet::setId(const int id) {
    this->id = id;
}

int Tweet::getId() const {
    return id;
}

void Tweet::setScreenName(const string screenName) {
    this->screenName = screenName;
}

string Tweet::getScreenName() const {
    return screenName;
}

void Tweet::setTimeStamp(const string timeStamp) {
    this->timeStamp = timeStamp;
}

string Tweet::getTimeStamp() const {
    return timeStamp;
}

void Tweet::setContent(const string content) {
    this->content = content;
}

string Tweet::getContent() const {
    return content;
}

void Tweet::setMentions(const vector<string> mentions) {
    this->mentions = mentions;
}

vector<string> Tweet::getMentions() const {
    return mentions;
}

string Tweet::getTimeStampFormat() const {
    return timeStampFormat;
}

string Tweet::toString() const {
    string allMentions = "";
    for (int i = 0; i < mentions.size(); i++) {
        allMentions += mentions.at(i) + ";";
    }
    stringstream id_ss;
    id_ss << id;
    return id_ss.str() + "\n" + screenName + "\n" + timeStamp + "\n" + content + "\n" + allMentions;
}

bool Tweet::compareTimeStamps(const Tweet& tweet2) const {
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

bool Tweet::compareScreenNames(const Tweet& tweet2) const {
    string screenName1 = screenName;
    string screenName2 = tweet2.screenName;
    transform(screenName1.begin(), screenName1.end(), screenName1.begin(), ::tolower);
    transform(screenName2.begin(), screenName2.end(), screenName2.begin(), ::tolower);
    return (screenName1 < screenName2) ? true : false;
}

bool Tweet::compareIds(const Tweet& tweet2) const {
    int id1 = id;
    int id2 = tweet2.id;
    return (id1 < id2) ? true : false;
}

bool operator<(const Tweet& tweet1, const Tweet& tweet2) {
    // compare using timestamps or screennames or ids
    //    return tweet1.compareTimeStamps(tweet2);
    return tweet1.compareScreenNames(tweet2);
    //    return tweet1.compareIds(tweet2);
}


/* TweetScreenNameOccurenceCacheItem class methods */

TweetScreenNameOccurenceCacheItem::TweetScreenNameOccurenceCacheItem() {}

TweetScreenNameOccurenceCacheItem::~TweetScreenNameOccurenceCacheItem() {
    tweetIds.clear();
}

TweetScreenNameOccurenceCacheItem TweetScreenNameOccurenceCacheItem::operator=(const TweetScreenNameOccurenceCacheItem& tc) {
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

TweetScreenNameOccurenceCacheItem::TweetScreenNameOccurenceCacheItem(string screenName, int noTweetsSent, vector<int> tweetIds) {
    this->screenName = screenName;
    this->noTweetsSent = noTweetsSent;
    this->tweetIds = tweetIds;
}

string TweetScreenNameOccurenceCacheItem::getScreenName() const {
    return screenName;
}

int TweetScreenNameOccurenceCacheItem::getNoTweetsSent() const {
    return noTweetsSent;
}

vector<int> TweetScreenNameOccurenceCacheItem::getTweetIds() const {
    return tweetIds;
}

void TweetScreenNameOccurenceCacheItem::setScreenName(const string screenName) {
    this->screenName = screenName;
}

void TweetScreenNameOccurenceCacheItem::setNoTweetsSent(const int noTweetsSent) {
    this->noTweetsSent = noTweetsSent;
}

void TweetScreenNameOccurenceCacheItem::setTweetIds(const vector<int> tweetIds) {
    this->tweetIds = tweetIds;
}

void TweetScreenNameOccurenceCacheItem::incrementNoTweetsSent() {
    noTweetsSent++;
}

string TweetScreenNameOccurenceCacheItem::toString() const {
    stringstream allTweetsIds_ss;
    for (int i = 0; i < tweetIds.size(); i++) {
        allTweetsIds_ss << tweetIds.at(i);
        allTweetsIds_ss << ";";
    }
    stringstream noTweetsSent_ss;
    noTweetsSent_ss << noTweetsSent;
    return screenName + ", " + noTweetsSent_ss.str() + ", " + allTweetsIds_ss.str();
}

bool TweetScreenNameOccurenceCacheItem::compareNoTweetsSent(const TweetScreenNameOccurenceCacheItem& tweetScreenNameOccurenceCacheItem2) const {
    return (noTweetsSent < tweetScreenNameOccurenceCacheItem2.noTweetsSent) ? true : false;
}

bool TweetScreenNameOccurenceCacheItem::compareScreenNames(const TweetScreenNameOccurenceCacheItem& tweetScreenNameOccurenceCacheItem2) const {
    string screenName1 = screenName;
    string screenName2 = tweetScreenNameOccurenceCacheItem2.screenName;
    transform(screenName1.begin(), screenName1.end(), screenName1.begin(), ::tolower);
    transform(screenName2.begin(), screenName2.end(), screenName2.begin(), ::tolower);
    return (screenName1 < screenName2) ? true : false;
}

bool operator<(const TweetScreenNameOccurenceCacheItem& tweetScreenNameOccurenceCacheItem1, const TweetScreenNameOccurenceCacheItem& tweetScreenNameOccurenceCacheItem2) {
    // compare using noTweetsSent or keywords
    return tweetScreenNameOccurenceCacheItem1.compareNoTweetsSent(tweetScreenNameOccurenceCacheItem2);
    //    return tweetScreenNameOccurenceCacheItem1.compareScreenNames(tweetScreenNameOccurenceCacheItem2);
}


/* TweetKeywordOccurenceCacheItem class methods */

TweetKeywordOccurenceCacheItem::TweetKeywordOccurenceCacheItem() {}

TweetKeywordOccurenceCacheItem::~TweetKeywordOccurenceCacheItem() {
    tweetIds.clear();
}

TweetKeywordOccurenceCacheItem TweetKeywordOccurenceCacheItem::operator=(const TweetKeywordOccurenceCacheItem& tc) {
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

TweetKeywordOccurenceCacheItem::TweetKeywordOccurenceCacheItem(string keyword, int noTweetsSent, vector<int> tweetIds) {
    this->keyword = keyword;
    this->noTweetsSent = noTweetsSent;
    this->tweetIds = tweetIds;
}

string TweetKeywordOccurenceCacheItem::getKeyword() const {
    return keyword;
}

int TweetKeywordOccurenceCacheItem::getNoTweetsSent() const {
    return noTweetsSent;
}

vector<int> TweetKeywordOccurenceCacheItem::getTweetIds() const {
    return tweetIds;
}

void TweetKeywordOccurenceCacheItem::setKeyword(const string keyword) {
    this->keyword = keyword;
}

void TweetKeywordOccurenceCacheItem::setNoTweetsSent(const int noTweetsSent) {
    this->noTweetsSent = noTweetsSent;
}

void TweetKeywordOccurenceCacheItem::setTweetIds(const vector<int> tweetIds) {
    this->tweetIds = tweetIds;
}

void TweetKeywordOccurenceCacheItem::incrementNoTweetsSent() {
    noTweetsSent++;
}

string TweetKeywordOccurenceCacheItem::toString() const {
    stringstream allTweetsIds_ss;
    for (int i = 0; i < tweetIds.size(); i++) {
        allTweetsIds_ss << tweetIds.at(i);
        allTweetsIds_ss << ";";
    }
    stringstream noTweetsSent_ss;
    noTweetsSent_ss << noTweetsSent;
    return keyword + ", " + noTweetsSent_ss.str() + ", " + allTweetsIds_ss.str();
}

bool TweetKeywordOccurenceCacheItem::compareNoTweetsSent(const TweetKeywordOccurenceCacheItem& tweetKeywordOccurenceCacheItem2) const {
    return (noTweetsSent < tweetKeywordOccurenceCacheItem2.noTweetsSent) ? true : false;
}

bool TweetKeywordOccurenceCacheItem::compareKeywords(const TweetKeywordOccurenceCacheItem& tweetKeywordOccurenceCacheItem2) const {
    string keyword1 = keyword;
    string keyword2 = tweetKeywordOccurenceCacheItem2.keyword;
    transform(keyword1.begin(), keyword1.end(), keyword1.begin(), ::tolower);
    transform(keyword2.begin(), keyword2.end(), keyword2.begin(), ::tolower);
    return (keyword1 < keyword2) ? true : false;
}

bool operator<(const TweetKeywordOccurenceCacheItem& tweetKeywordOccurenceCacheItem1, const TweetKeywordOccurenceCacheItem& tweetKeywordOccurenceCacheItem2) {
    // compare using noTweetsSent or keywords
    return tweetKeywordOccurenceCacheItem1.compareNoTweetsSent(tweetKeywordOccurenceCacheItem2);
    //    return tweetKeywordOccurenceCacheItem1.compareKeywords(tweetKeywordOccurenceCacheItem2);
}


/* TweetKeywordUsernameMentionedCacheItem class methods */

TweetKeywordUsernameMentionedCacheItem::TweetKeywordUsernameMentionedCacheItem() {}

TweetKeywordUsernameMentionedCacheItem::~TweetKeywordUsernameMentionedCacheItem() {}

TweetKeywordUsernameMentionedCacheItem TweetKeywordUsernameMentionedCacheItem::operator=(const TweetKeywordUsernameMentionedCacheItem& tc) {
    if (this == &tc) {
        return *this;
    }
    else {
        initialTweet = tc.initialTweet;
        influencedTweet= tc.influencedTweet;
        return *this;
    }
}

TweetKeywordUsernameMentionedCacheItem::TweetKeywordUsernameMentionedCacheItem(Tweet initialTweet, Tweet influencedTweet) {
    this->initialTweet = initialTweet;
    this->influencedTweet = influencedTweet;
}

Tweet TweetKeywordUsernameMentionedCacheItem::getInitialTweet() const {
    return initialTweet;
}

Tweet TweetKeywordUsernameMentionedCacheItem::getInfluencedTweet() const {
    return influencedTweet;
}

void TweetKeywordUsernameMentionedCacheItem::setInitialTweet(const Tweet& initialTweet) {
    this->initialTweet = initialTweet;
}

void TweetKeywordUsernameMentionedCacheItem::setInfluencedTweet(const Tweet& influencedTweet) {
    this->influencedTweet = influencedTweet;
}

string TweetKeywordUsernameMentionedCacheItem::toString() const {
    return initialTweet.toString() + "\n" + influencedTweet.toString();
}