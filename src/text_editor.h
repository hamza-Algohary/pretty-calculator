#ifndef TEXTEDITOR_H_
#define TEXTEDITOR_H_

#include <string>
#include <sstream>
#include <cmath>
#include <vector>

using namespace std;

/*
template <typename T>
string toString(T var);

string toString(long double var);
string toString(int var);*/


/*double convertToNumber(char c);
double convertToNumber(string str);*/
//int search(char c, string str);

//------------------------------------------------

void clear(char c, string &str);
void clear_by_index(int i, string &str);

bool compare(string input , string keyword , int index);
int search_first(string input , string keyword , int from=0 , int to=-1);
vector<int> search_all(string input , string keyword , int from=0 , int to=-1);

bool exist_in(string input , string keyword);
bool exist_in(string input , char c);

string copy_part_of_string(string str, int down, int up);
void cut(string &input , int down , int up , string replacement="");
void replace_each(string &input , string to_be_replaced , string replacement);

void insert_at(string &input , int index , string to_be_inserted);
void insert_at(string &input , vector<int> index , string to_be_inserted);
void insert_after_each(string &input , string to_be_inserted , string after);

string char_to_string(char c);
int convert_to_number(char c);
long double convert_to_number(string str);

bool isNumber(char c);
bool isArithmeticSign(char c);
bool is_letter(char c);
bool is_capital(char c);
bool is_small(char c);

string capitalize(string &input , int index);
string capitalize_all(string &input);
char capitalize(char c);

string uncapitalize(string &input , int index);
string uncapitalize_all(string &input);
char uncapitalize(char c);

#endif