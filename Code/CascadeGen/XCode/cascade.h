#ifndef cascade_h
#define cascade_h

#include "tweet.h"
#include "utility.h"


/* CascadeItem class */

class CascadeItem {
public:
    CascadeItem();
    
    ~CascadeItem();
    
    CascadeItem operator=(const CascadeItem& tc);
    
    CascadeItem(TwitterAccount account, string timeStamp);
    
    void setAccount(const TwitterAccount& account);
    
    TwitterAccount getAccount() const;
    
    void setTimeStamp(const string& timeStamp);
    
    string getTimeStamp() const;
    
    void setTimeStampFormat(const string& timeStampFormat);
    
    string getTimeStampFormat() const;
    
    string toString() const;
    
    friend ostream& operator<<(ostream& os, const CascadeItem& obj);
    
private:
    double convertTimeStampToSeconds(const string& timeStamp) const;
    
    TwitterAccount account;
    string timeStamp;
    string timeStampFormat;
};


/* Cascade class */

class Cascade {
public:
    Cascade();
    
    ~Cascade();
    
    Cascade operator=(const Cascade& tc);
    
    Cascade(vector<CascadeItem> cascade);
    
    void setCascade(const vector<CascadeItem>& account);
    
    vector<CascadeItem> getCascade() const;
    
    void addCascadeItem(const CascadeItem& cascadeItem);
    
    string toString() const;
    
    friend ostream& operator<<(ostream& os, const Cascade& obj);
    
private:
    vector<CascadeItem> cascade;
};

#endif