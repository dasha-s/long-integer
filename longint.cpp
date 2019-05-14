//
// Created by Daria  on 12/2/18.
//
#include "longint.h"
#include <algorithm>

using namespace std;

//Constructors
LongInt::LongInt(const string str) {
    int pos = 0;
    negative = false;
    while (str.at(pos) == ' ' && pos < str.length()) {           //to skip spaces
        pos++;
    }
    if (pos > str.length() - 1) {                                //if the string is empty
        digits.addBack('0');                                     //because the string consists of the spaces only
        negative = false;                                        //initialize a LongInt with 0
        return;
    }
    if (str.at(pos) == '-') {
        negative = true;                                         //make it negative
        pos++;
    }
    //iterate through the rest of the string
    //indicator of whether we found at least one non-zero digit
    bool non_zero_digit_found = false;
    for (int i = pos; i < str.length(); i++) {
        //ignore any non-numeric chars
        if (str.at(i) < '0' || str.at(i) > '9') {
            continue;
        }
        //ignore all leading zeros
        if (!non_zero_digit_found && str.at(i) == '0') {
            continue;
        }
        //found some non zero digits, and after that the program accepts zeros as well
        non_zero_digit_found = true;
        digits.addBack(str.at(i));
    }
    //if there's still an indicator in false, that means that nothing is read from a string, initiate with 0
    if (!non_zero_digit_found) {
        digits.addBack('0');                                     //because the string consists of the spaces only
        negative = false;                                        //initialize a LongInt with 0
    }
}

//copy constructor
LongInt::LongInt(const LongInt &rhs) {
    digits = rhs.digits;
    negative = rhs.negative;                        //get the sign
}

//default constructor
LongInt::LongInt( ) {
    digits.addBack('0');
    negative = false;
}

//destructor
LongInt::~LongInt( ) {
    digits.clear();
}

//reads characters '0' through '9' from the keyboard as well as accept '-'
// if it is the first character. Skips all the other characters.
istream &operator>>(istream &in, LongInt &rhs) {
    string line;
    getline(in, line);
    rhs = LongInt(line);
    return in;
}

//prints out a given LongInt object's digits as an integer.
//If the LongInt object's digits is empty or 0,
//it prints out as 0 without a negative sign.
ostream &operator<<( ostream &out, const LongInt &rhs ) {
    LongInt temp(rhs);         //make a copy  of "rhs" to not modify it
    if (rhs.negative) {
        out << '-';
    }
    //print out an object by removing the nodes from the front
    while (!temp.digits.isEmpty()) {
        out << temp.digits.removeFront();
    }
    return out;
}

LongInt LongInt::operator+(const LongInt &rhs) const {
    //positive lhs, positive rhs
    if (!negative && !rhs.negative) {
        return plus(rhs);
    }
    //positive lhs and negative rhs
    if (!negative && rhs.negative) {
        LongInt positive(rhs);
        positive.negative = false;
        //it is more convenient to subtract greater number from smaller number
        if (*this >= positive) {
            return minus(positive);
        } else {
            LongInt result = positive.minus(*this);
            result.negative = true;
            return result;
        }
    }
    //negative lhs and positive rhs
    if (negative && !rhs.negative) {
        LongInt positive(*this);
        positive.negative = false;
        if (positive > rhs) {
            LongInt result = positive.minus(rhs);
            result.negative = true;
            return result;
        } else {
            return rhs.minus(positive);
        }
    }
    //negative lhs and negative rhs
    if (negative && rhs.negative) {
        LongInt result = plus(rhs);
        result.negative = true;
        return result;
    }
}

LongInt LongInt::operator-(const LongInt &rhs) const {
    //positive lhs, positive rhs
    if (!negative && !rhs.negative) {
        if (*this >= rhs) {
            return minus(rhs);
        } else {
            LongInt result = rhs.minus(*this);
            result.negative = true;
            return result;
        }
    }
    //positive lhs and negative rhs
    if (!negative && rhs.negative) {
        return plus(rhs);
    }
    //negative lhs and positive rhs
    if (negative && !rhs.negative) {
        LongInt result = plus(rhs);
        result.negative = true;
        return result;
    }
    //negative lhs and negative rhs
    if (negative && rhs.negative) {
        //copy the variables because we cannot change *this and rhs
        LongInt positive1(*this);
        positive1.negative = false;
        LongInt positive2(rhs);
        positive2.negative = false;
        if (positive1 > positive2) {
            LongInt result = positive1.minus(positive2);
            result.negative = true;
            return result;
        } else {
            return positive2.minus(positive1);
        }
    }
}

//addition operator
LongInt LongInt::plus(const LongInt &rhs) const {
    int carry = 0;       //to store digits that exceed 10 when adding
    //create copies of "rhs" and "lhs" to not modify them
    LongInt lhs_temp(*this);
    LongInt rhs_temp(rhs);
    string s = string("");
    while (!lhs_temp.digits.isEmpty() || !rhs_temp.digits.isEmpty()) {
        int lhs_digit = 0;
        if (!lhs_temp.digits.isEmpty()) {
            lhs_digit = lhs_temp.digits.removeBack() - '0';
        }
        int rhs_digit = 0;
        if (!rhs_temp.digits.isEmpty()) {
            rhs_digit = rhs_temp.digits.removeBack() - '0';
        }
        int ans = (lhs_digit + rhs_digit + carry) % 10;
        carry = (lhs_digit + rhs_digit + carry) / 10;
        s += '0' + ans;
    }
    if (carry != 0) {
        s += '0' + carry;
    }
    reverse(s.begin(), s.end());   //reverse the string because digits were removed starting from the back
    return LongInt(s);
}

LongInt LongInt::minus(const LongInt &rhs) const {
    int borrow = 0;       //to borrow a 10 when subtracting
    //create a copy of "lhs" and "rhs" to not modify the originals
    LongInt lhs_temp(*this);
    LongInt rhs_temp(rhs);
    string s = string("");
    while (!lhs_temp.digits.isEmpty() || !rhs_temp.digits.isEmpty()) {
        int lhs_digit = 0;
        if (!lhs_temp.digits.isEmpty()) {
            lhs_digit = lhs_temp.digits.removeBack() - '0';
        }
        int rhs_digit = 0;
        if (!rhs_temp.digits.isEmpty()) {
            rhs_digit = rhs_temp.digits.removeBack() - '0';
        }
        int ans = 0;
        if (lhs_digit < rhs_digit + borrow) {
            ans = 10 + lhs_digit - rhs_digit - borrow;
            borrow = 1;
        } else {
            ans = lhs_digit - rhs_digit - borrow;
            borrow = 0;
        }
        s += '0' + ans;
    }
    reverse(s.begin(), s.end());
    return LongInt(s);
}

const LongInt &LongInt::operator=( const LongInt &rhs ) {
    if (this != &rhs) {
        digits = rhs.digits;
        negative = rhs.negative;            //get the sign
    }
    return *this;
}

bool LongInt::operator>( const LongInt & rhs ) const {
    //compare sign
    //negative and positive -> return false
    if (negative && !rhs.negative) {
        return false;
    }
    //positive and negative -> return true
    if(!negative && rhs.negative) {
        return true;
    }
    //both positive
    if (!negative && !rhs.negative) {
        //compare the number of nodes
        if (digits.size() > rhs.digits.size()) {
            return true;
        }
        if (digits.size() < rhs.digits.size()) {
            return false;
        }
        return greater_same_size(rhs);
    //both negative
    } else {
        if (digits.size() > rhs.digits.size()) {
            return false;
        }
        if (digits.size() < rhs.digits.size()) {
            return true;
        }
        return rhs.greater_same_size(*this);
    }
}

//checks if lhs is greater than rhs when integers are the same size
bool LongInt::greater_same_size(const LongInt &rhs) const {
    LongInt lhs_temp(*this);
    LongInt rhs_temp(rhs);
    while (!lhs_temp.digits.isEmpty() || !rhs_temp.digits.isEmpty()) {
        int lhs_digit = 0;
        if (!lhs_temp.digits.isEmpty()) {
            lhs_digit = lhs_temp.digits.removeFront() - '0';
        }
        int rhs_digit = 0;
        if (!rhs_temp.digits.isEmpty()) {
            rhs_digit = rhs_temp.digits.removeFront() - '0';
        }
        if (lhs_digit > rhs_digit) {
            return true;
        }
        if (lhs_digit < rhs_digit) {
            return false;
        }
    }
    return false;           //if rhs and lhs are equal
}

bool LongInt::operator>=( const LongInt & rhs ) const {
    //compare sign
    //negative and positive -> return false
    if (negative && !rhs.negative) {
        return false;
    }
    //positive and negative -> return true
    if(!negative && rhs.negative) {
        return true;
    }
    //both positive
    if (!negative && !rhs.negative) {
        //compare the number of nodes
        if (digits.size() > rhs.digits.size()) {
            return true;
        }
        if (digits.size() < rhs.digits.size()) {
            return false;
        }
        return greater_or_equal_same_size(rhs);
        //both negative
    } else {
        if (digits.size() > rhs.digits.size()) {
            return false;
        }
        if (digits.size() < rhs.digits.size()) {
            return true;
        }
        return rhs.greater_or_equal_same_size(*this);
    }
}

//checks if lhs is greater than or equal rhs when integers are the same size
bool LongInt::greater_or_equal_same_size(const LongInt &rhs) const {
    LongInt lhs_temp(*this);
    LongInt rhs_temp(rhs);
    while (!lhs_temp.digits.isEmpty() || !rhs_temp.digits.isEmpty()) {
        int lhs_digit = 0;
        if (!lhs_temp.digits.isEmpty()) {
            lhs_digit = lhs_temp.digits.removeFront() - '0';
        }
        int rhs_digit = 0;
        if (!rhs_temp.digits.isEmpty()) {
            rhs_digit = rhs_temp.digits.removeFront() - '0';
        }
        if (lhs_digit > rhs_digit) {
            return true;
        }
        if (lhs_digit < rhs_digit) {
            return false;
        }
    }
    return true;           //if rhs and lhs are equal
}

bool LongInt::operator<( const LongInt & rhs ) const {
    return rhs > *this;    //swap out rhs and lhs and reuse operator>
}

bool LongInt::operator<=( const LongInt & rhs ) const {
    return rhs >= *this;   //swap out rhs and lhs and reuse operator>=
}

bool LongInt::operator==( const LongInt & rhs ) const {
    //if both of these conditions are correct, then the "=" is used in both of the cases
    //thus, lhs and rhs are equal
    return *this >= rhs && *this <= rhs;
}

bool LongInt::operator!=( const LongInt & rhs ) const {
    //if at least on of these conditions returns true, then lhs and rhs are not equal 
    return *this > rhs || *this < rhs;
}