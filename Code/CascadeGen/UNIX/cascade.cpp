#include "cascade.h"

/* CascadeItem class methods */

CascadeItem::CascadeItem() {
    timeStampFormat = Tweet().getTimeStampFormat();
}

CascadeItem::~CascadeItem() {}

CascadeItem CascadeItem::operator=(const CascadeItem& tc) {
    if (this == &tc) {
        return *this;
    }
    else {
        account = tc.account;
        timeStamp = tc.timeStamp;
        return *this;
    }
}

CascadeItem::CascadeItem(TwitterAccount account, string timeStamp) {
    timeStampFormat = Tweet().getTimeStampFormat();
    this->account = account;
    this->timeStamp = timeStamp;
}

void CascadeItem::setAccount(const TwitterAccount& account) {
    this->account = account;
}

TwitterAccount CascadeItem::getAccount() const {
    return account;
}

void CascadeItem::setTimeStamp(const string& timeStamp) {
    this->timeStamp = timeStamp;
}

string CascadeItem::getTimeStamp() const {
    return timeStamp;
}

void CascadeItem::setTimeStampFormat(const string& timeStampFormat) {
    this->timeStampFormat = timeStampFormat;
}

string CascadeItem::getTimeStampFormat() const {
    return timeStampFormat;
}

string CascadeItem::toString() const {
    return convertIntToString(account.getId()) + "," + convertDoubleToString(convertTimeStampToSeconds(timeStamp));
}

// note: keep reference time of September 21, 2012 00:00:00
double CascadeItem::convertTimeStampToSeconds(const string &timeStamp) const {
    time_t t1, refT;
    struct tm tm1, refTm;
    strptime(timeStamp.c_str(), timeStampFormat.c_str(), &tm1);
    t1 = mktime(&tm1);
    refTm.tm_year = 112;
    refTm.tm_mon = 8;
    refTm.tm_mday = 21;
    refTm.tm_hour = 0;
    refTm.tm_min = 0;
    refTm.tm_sec = 0;
    refT = mktime(&refTm);
    double diff = difftime(t1, refT);
    return diff;
}

ostream& operator<<(ostream& os, const CascadeItem& obj) {
    os << obj.toString();
    return os;
}


/* Cascade class methods */

Cascade::Cascade() {}

Cascade::~Cascade() {
    cascade.clear();
}

Cascade Cascade::operator=(const Cascade& tc){
    if (this == &tc) {
        return *this;
    }
    else {
        cascade = tc.cascade;
        return *this;
    }
}

Cascade::Cascade(vector<CascadeItem> cascade) {
    this->cascade = cascade;
}

void Cascade::setCascade(const vector<CascadeItem>& account) {
    this->cascade = cascade;
}

vector<CascadeItem> Cascade::getCascade() const {
    return cascade;
}

void Cascade::addCascadeItem(const CascadeItem& cascadeItem) {
    cascade.push_back(cascadeItem);
}

string Cascade::toString() const {
    string cascade_s;
    for (int i = 0; i < cascade.size(); i++) {
        cascade_s += cascade.at(i).toString() + ";";
    }
    return cascade_s;
}

ostream& operator<<(ostream& os, const Cascade& obj) {
    os << obj.toString();
    return os;
}