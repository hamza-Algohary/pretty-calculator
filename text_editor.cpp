#include "text_editor.h"

#include <string>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <iostream>

//#include <iostream> //For debugging , delete it after finishing

using namespace std;

void clear(char c, string &str)
{
    stringstream text;
    for (int i = 0; i < str.length(); i++)
    {
        if (str[i] == c)
        {
            continue;
        }
        else
        {
            text << str[i];
        }
    }
    str = text.str();
}

void clear_by_index(int i, string &str)
{
    stringstream text;
    for (int j = 0; j < str.size(); j++)
    {
        if (j == i)
        {
            continue;
        }
        else
        {
            text << str[j];
        }
    }
    str = text.str();
}

//This function returns true if the keyword is an empty string
bool compare(string input , string keyword , int index){

    if(input.length()-index < keyword.length()){
        return false;
    }

    for(int i=index  ,j=0; j<keyword.length() ; i++ , j++){
        if(input[i] != keyword[j]){
            return false;
        }
    }
    return true;
}//Tested succefully

bool compare_ustring(Glib::ustring input , Glib::ustring keyword , int index){

    if(input.length()-index < keyword.length()){
        return false;
    }

    for(int i=index  ,j=0; j<keyword.length() ; i++ , j++){
        if(input[i] != keyword[j]){
            return false;
        }
    }
    return true;
}//Doesnot work

int search_first(string input , string keyword , int from , int to){
    if(to==-1){
        to=input.length();
    }
    for(int i=from ; i<to ; i++){
        bool found = compare(input , keyword , i);
        if(found){
            return i;
        } 
    }
    return -1;
}//Tested successfully

vector<int> search_all(string input , string keyword , int from , int to){
    vector<int> indexes;
    if(to==-1){
        to=input.length();
    }
    for(int i=from ; i<to ; i++){
        bool found = compare(input , keyword , i);
        if(found){
            indexes.push_back(i);
        } 
    }
    return indexes;
}//Ready


bool exist_in(string input , string keyword){
    int found = search_first(input , keyword);
    if(found == -1){
        return false;
    }else{
        return true;
    }
}
bool exist_in(string input , char c){
    int found = search_first(input , char_to_string(c));
    if(found == -1){
        return false;
    }else{
        return true;
    }
}

void cut(string &input , int down , int up , string replacement){
    stringstream ss;
    if (down > up){
        //swap(down , up);
    }
    
    for(int i=0;i<input.length();i++){
        if(i==down){
            ss << replacement;
            i = up;
        }else{
            ss << input[i];
        }
    }
    input = ss.str();
}

void replace_each(string &input , string to_be_replaced , string replacement){
    /*auto indexes = search_all(input , to_be_replaced);
    std::cout << indexes.size() << std::endl;
    for(int i=0;i<indexes.size();i++){
        std::cout << indexes[i] << " " << indexes[i]+to_be_replaced.length() << "\n";
        cut(input , indexes[i] , indexes[i]+to_be_replaced.length()-1, replacement);
    }*/
    stringstream ss;
    for (int i = 0; i < input.length(); i++){
        if (compare(input , to_be_replaced , i)){
            ss << replacement;
            i += to_be_replaced.length()-1;
        }else{
            ss << input[i];
        }
    }
    input = ss.str();
}

/*void replace_each_ustring(Glib::ustring &input , Glib::ustring to_be_replaced , char replacement){
    /*auto indexes = search_all(input , to_be_replaced);
    std::cout << indexes.size() << std::endl;
    for(int i=0;i<indexes.size();i++){
        std::cout << indexes[i] << " " << indexes[i]+to_be_replaced.length() << "\n";
        cut(input , indexes[i] , indexes[i]+to_be_replaced.length()-1, replacement);
    }
    stringstream ss;
    for (auto c : input.){
        //input.erase(c);
        //std::cout << (char)c << std::endl;
        if (c == to_be_replaced[0]){
            input.erase(c);
            input.insert(c, replacement);
        }/*else{
            ss << (char)c;
        }
    }

    /*for (int i = 0; i < input.length(); i++){
        if ((int)input[i] == (int)to_be_replaced[0]){
            std::string tmp = "";
            convert_ustring_to_string(replacement , tmp);
            ss << tmp;
            i += to_be_replaced.length()-1;
        }else{
            ss << (char)input[i];
        }
    }
    input = ss.str();
}*/

string copy_part_of_string(string str, int down, int up){
    stringstream ss;
    for (int i = down; i <= up; i++){
        ss << str[i];
    }

    return ss.str();
}

void insert_at(string &input , int index , string to_be_inserted){
    stringstream ss;

    if(index == input.length()){
        ss << input;
        ss << to_be_inserted;
        input = ss.str();
        return;
    }

    for(int i=0 ; i<input.length() ; i++){
        if(i == index){
            ss << to_be_inserted;
        }
        ss << input[i];
    }
    input = ss.str();
}//Ready

void insert_at(string &input , vector<int> indexes , string to_be_inserted){
    sort(indexes.begin() , indexes.end());
    stringstream ss;

    int i=0;
    for(auto index : indexes){
        for(; i<index ; i++){
            ss << input[i];
        }
        ss << to_be_inserted;
    }
    for(; i<input.length() ; i++){
        ss << input[i];
    }
    input = ss.str();
}

void convert_ustring_to_string(Glib::ustring u , std::string &s){
    s = "";
    for (auto c : u){
        s.append(char_to_string((char)c));
    }
}

void insert_after_each(string &input , string to_be_inserted , string after){
    auto indexes = search_all(input , after);
    insert_at(input , indexes , to_be_inserted);
}

string char_to_string(char c){
    stringstream ss;
    ss << c;
    return ss.str();
}

bool isNumber(char c)
{
    if (c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9')
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool isArithmeticSign(char c)
{
    if ((c == '+') || (c == '/') || (c == '*')) //I've removed minus in order to add negative to numbers
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool is_letter(char c){
    return exist_in("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ" , c);
}

bool is_capital(char c){
    return (c <= 'Z') && (c >= 'A');
}

bool is_small(char c){
    return (c <= 'z') && (c >= 'a');
}

string capitalize(string &input , int index){
    if(is_small(input[index])){
        input[index] -= ('a' - 'A');
    }

    return input;
}

string capitalize_all(string &input){
    for(int i=0 ; i<input.length() ; i++){
        if(is_small(input[i])){
            input[i] -= ('a' - 'A');
        }
    }
    return input;
}

char capitalize(char &c){
    if(is_small(c)){
        c -= ('a' - 'A');
    }
    return c;
}//All ready till here

string uncapitalize(string &input , int index){
    if(is_capital(input[index])){
        input[index] += ('a' - 'A');
    }

    return input;
}

string uncapitalize_all(string &input){
    for(int i=0 ; i<input.length() ; i++){
        if(is_capital(input[i])){
            input[i] += ('a' - 'A');
        }
    }
    return input;
}

char uncapitalize(char &c){
    if(is_capital(c)){
        c += ('a' - 'A');
    }
    return c;
}

/*int search(char c, string str)
{
    for (int i = 0; i < str.length(); i++)
    {
        if (str[i] == c)
        {
            return i;
        }
    }
    return -1;
}*/

int convert_to_number(char c)
{
    std::cout << "CONVERTING NUMBER " << c << std::endl;
    if (c == '0')
    {
        return 0;
    }
    else if (c == '1')
    {
        return 1;
    }
    else if (c == '2')
    {
        return 2;
    }
    else if (c == '3')
    {
        return 3;
    }
    else if (c == '4')
    {
        return 4;
    }
    else if (c == '5')
    {
        return 5;
    }
    else if (c == '6')
    {
        return 6;
    }
    else if (c == '7')
    {
        return 7;
    }
    else if (c == '8')
    {
        return 8;
    }
    else if (c == '9')
    {
        return 9;
    }
    else{
        return -1;
    }
}

//Not working ... Needs some fixes
long double convert_to_number(string str)
{
    //int size = str.length(); //Remove it later
    std::cout << "Converting number: " << str << std::endl;
    bool negative = false;
    bool decimal = false;
    if (str[0] == '-')
    {
        negative = true;
        clear_by_index(0, str);
    }
    int size;

    int dot = search_first(str , to_string('.'));

    if (dot == -1)
    {
        size = str.length();
    }
    else
    {
        clear_by_index(dot, str);
        decimal = true;
        size = str.length();
    }

    long double arr[size];

    for (int i = 0; i < size; i++)
    {
        long int x = convert_to_number(str[i]);
        if (x == -1){
            std::cout << str[i];
            throw " Bad conversion";
        }
        for (int j = 1; j < (size - i); j++)
        {
            x *= 10;
        }
        //x *= pow(10 , size-i-1);
        arr[i] = x;
    }

    long double result = 0;
    for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
    {
        result += arr[i];
    }

    if (negative)
    {
        result *= -1;
    }

    if (decimal)
    {
        double decimalPoint = str.length() - dot;
        result /= pow((double)10, decimalPoint);
    }

    return result;
}

/*template <typename T>
string toString(T var)
{
    stringstream ss;
    ss << var;
    return ss.str();
}

string toString(long double var)
{
    stringstream ss;
    ss << var;
    return ss.str();
}

string toString(int var)
{
    stringstream ss;
    ss << var;
    return ss.str();
}
*/

