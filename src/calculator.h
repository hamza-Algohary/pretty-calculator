#pragma once

#include <map>
#include <functional>
#include <stack>
#include <math.h>
#include <iostream>
#include <memory>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <glibmm.h>
#include "text_editor.h"
namespace num = boost::multiprecision;

//typedef numbers::cpp_dec_float<50> number;
typedef num::cpp_dec_float_100 number;
// # ! () &


namespace my{
    number log(number base , number operand){
        return (num::log(operand)/num::log(base));
    }
    number root(number operand , number nth){
        return num::pow(operand , 1.0/nth);
    }

    auto round_to_nearest = [](number n , int precision){
        return num::round(n*std::pow(10 , precision))/std::pow(10 , precision);
    };

    typedef enum{
        ERROR_UNEXPECTED_CHARACHTER,
        ERROR_BAD_BRACKETS,
        ERROR_DIVISION_BY_ZERO,
        ERROR_DIVISION_ZERO_BY_ZERO,
        ERROR_EMPTY_STRING,
        ERROR_UNKNOWN_VARIABLE,
        ERROR_OUT_OF_RANGE,
        ERROR_INVALID_ARGC
    } CalculatorError;

    const number DEGREE_SYSTEM = M_PI/180 , RADIAN_SYSTEM = 1;
    number ANGLE_SYSTEM = DEGREE_SYSTEM;

struct Interval{
    vector<std::function<bool(number)>> conditions;
    std::string error_message = "";
    bool contains(number n){
        bool condition = true;
        for(auto cond : conditions){
            condition = condition && cond(n); 
        }
        return condition;
    }
};
Interval cotan_interval , tan_interval , real_interval , logarithmic_interval , arcsin_interval , arccos_interval;
struct math_function{
    std::string name;
    int argc;
    Interval *interval;
    math_function(std::string name , int argc , function<number(vector<number>)> call , Interval &interval = real_interval);
    bool operator==(const math_function& other);
    math_function();
    number apply_function(string args); 
    number call(vector<number> args){
        if(args.size() != argc){
            throw std::make_pair(ERROR_INVALID_ARGC , name);
        }
        for (auto arg : args){
            if (!interval->contains(arg)){
                throw std::make_pair(ERROR_OUT_OF_RANGE , name);
            }  
        }
        return m_call(args);
    }
private:
    function<number(vector<number>)> m_call;
};

auto EMPTY_FUNCTION = math_function();
/*auto convert_to_angle = [](number angle){
    auto consume = [](){};
    if (ANGLE_SYSTEM == DEGREE_SYSTEM){
        angle %= 360;
    }else if (ANGLE_SYSTEM == RADIAN_SYSTEM){
        angle = angle*DEGREE_SYSTEM;
        angle %= 360;
        angle = angle / DEGREE_SYSTEM;
    }
    return angle;
};*/

/*auto check_domain = [](number n , number begin , number end , std::string ){
    if(!(n >= begin && n <=end)){
        throw make_pair(OUT_OF_DOMAIN , );
    }
};*/

class{    
private:
    //std::map<std::pair<std::string , int> , math_function> functions;
    vector <math_function> functions;
    int constructor = [&](){

        add_function(math_function("abs" , 1 , [](std::vector<number> numbers){return num::abs(numbers.begin()[0]);}));

        add_function( math_function("log" , 1 , [](std::vector<number> numbers){return num::log10(numbers.begin()[0]);} , logarithmic_interval));
        add_function( math_function("log" , 2 , [](std::vector<number> numbers){return my::log(numbers.begin()[0] , numbers.begin()[1]);} , logarithmic_interval));
        add_function( math_function("ln" , 1 , [](std::vector<number> numbers){return num::log(numbers.begin()[0]);} , logarithmic_interval));
        add_function( math_function("pow" , 2 , [](std::vector<number> numbers){return num::pow(numbers.begin()[0] , numbers.begin()[1]);}));
        add_function( math_function("root" , 2 , [](std::vector<number> numbers){return my::root(numbers.begin()[0] , numbers.begin()[1]);}));
        add_function(math_function("sqrt" , 1 , [](std::vector<number> numbers){return num::sqrt(numbers.begin()[0]);} , logarithmic_interval));

        add_function( math_function("sin" , 1 , [](std::vector<number> numbers){return num::sin(numbers.begin()[0]*ANGLE_SYSTEM);}));
        add_function( math_function("cos" , 1 , [](std::vector<number> numbers){return num::cos(numbers.begin()[0]*ANGLE_SYSTEM);}));
        add_function( math_function("tan" , 1 , [](std::vector<number> numbers){return num::tan(numbers.begin()[0]*ANGLE_SYSTEM);} , tan_interval));

        add_function(math_function("sec" , 1 , [](std::vector<number> numbers){return 1.0/num::cos(numbers.begin()[0]*ANGLE_SYSTEM);}));      
        add_function( math_function("csc" , 1 , [](std::vector<number> numbers){return 1.0/num::sin(numbers.begin()[0]*ANGLE_SYSTEM);}));
        add_function( math_function("cot" , 1 , [](std::vector<number> numbers){return 1.0/num::tan(numbers.begin()[0]*ANGLE_SYSTEM);}));

        add_function( math_function("arcsin" , 1 , [](std::vector<number> numbers){return num::asin(numbers.begin()[0])*1/ANGLE_SYSTEM;} , arcsin_interval));
        add_function( math_function("arccos" , 1 , [](std::vector<number> numbers){return num::acos(numbers.begin()[0])*1/ANGLE_SYSTEM;} , arccos_interval));
        add_function( math_function("arctan" , 1 , [](std::vector<number> numbers){return num::atan(numbers.begin()[0])*1/ANGLE_SYSTEM;}));
        add_function( math_function("asin" , 1 , [](std::vector<number> numbers){return num::asin(numbers.begin()[0])*1/ANGLE_SYSTEM;} , arcsin_interval));
        add_function( math_function("acos" , 1 , [](std::vector<number> numbers){return num::acos(numbers.begin()[0])*1/ANGLE_SYSTEM;} , arccos_interval));
        add_function( math_function("atan" , 1 , [](std::vector<number> numbers){return num::atan(numbers.begin()[0])*1/ANGLE_SYSTEM;}));
        
        return 0;}();
public:
    math_function get_function(std::string name , int argc){
        for(auto func:functions){
            if ((func.name == name)&&(func.argc == argc)){
                return func;
            }
            
        }
        return EMPTY_FUNCTION;

    }
    void add_function(math_function mf){
        if(exists(mf.name , mf.argc)){
            return;
        }
        functions.push_back(mf);
    }

    math_function extract_function(std::string &formula){
        std::string name;
        for (int i=0; i < formula.length(); i++){
            if (is_letter(formula[i])){
                name.append(char_to_string(formula[i]));
            }else{
                cut(formula , 0 , i-1);
                //std::cout << name << std::endl;
                break;
            }
        }
        if(!exists(name , 1)){
            throw std::make_pair(ERROR_UNKNOWN_VARIABLE , name);
        }
        return get_function(name , 1);  
    }

    bool exists(std::string name , int argc){
        return !(get_function(name , argc) == EMPTY_FUNCTION);
    }
    const vector<math_function> &get_functions(){
        return functions;
    }
}functions;


class SimpleCalculator{

    typedef enum{
        PRIORITY_PLUS_MINUS = 0, 
        PRIORITY_TIMES_DIVIDE = 1, 
        PRIORITY_POWER = 2
    } Priority;

    struct Operation{
        char symbol;
        int priority; //0 is least priority 3 is bigger, and so on..
        function<number(number, number)> operation;
        Operation(){operation = [](number , number){throw std::make_pair(ERROR_UNKNOWN_VARIABLE , ""); return -1;};}
        Operation(char symbol, int priority, function<number(number, number)> operation) : symbol(symbol), priority(priority), operation(operation) {}
        number Operator(number operand_1, number operand_2){
            return operation(operand_1 , operand_2);
        }
        Operation operator=(const Operation& op){
            symbol = op.symbol;
            priority = op.priority;
            operation = op.operation;
            return (*this);
        }
        bool operator==(const Operation& op){
            return (op.symbol == symbol) && (op.priority == priority);
        }
    };
    const Operation EMPTY_OPERATION = Operation();
    std::map<char, Operation> operations;
    std::string operations_symbols;
    std::vector<Operation> operators;
    std::vector<number> operands;

    number extract_number(std::string &formula){
            math_function func;
            bool function_exists = false;
            if(is_letter(formula[0])){
                func = functions.extract_function(formula);
                function_exists = true;
            }
            std::stringstream num_stream;
            bool dot_exists = false;
            bool negative_exists = false;
            int i=0;
            if (formula == ""){
                throw ERROR_EMPTY_STRING;
            }
            if(formula[i] == '-'){
                num_stream << formula[i];
                i++;
            }
            if(!isNumber(formula[i])){
                throw std::make_pair(ERROR_UNEXPECTED_CHARACHTER , char_to_string(formula[i]));
            }

            for (i; i <=formula.length(); i++){
                if (isNumber(formula[i])){
                    num_stream << formula[i];
                }else if(formula[i] == '.'){
                    if(dot_exists){
                        throw std::make_pair(ERROR_UNEXPECTED_CHARACHTER , ".");
                    }else{
                        num_stream << formula[i];
                        dot_exists = true;
                    }
                }else{
                    cut(formula , 0 , i-1 , ""); // Clears the extracted number
                    
                    //std::cout << "\tnumber is:    " << num.str() << std::endl;
                    break;
                }
            }
            number result(num_stream.str());//std::stold(num.str());
            //std::cout << "got first number\n";
            if (function_exists){
                vector<number> args;
                args.push_back(result);
                return func.call(args);
            }
            
            return result; //stold = string to long double
        }

        Operation extract_operation(std::string &formula)
        {
            if(formula == ""){
                throw ERROR_EMPTY_STRING;
            }
            //std::cout << "getting first operation\n";
            //if (exist_in(operations_symbols, formula[0]) )
            //if (operations[formula[0]] == EMPTY_OPERATION){
            //    throw std::make_pair(ERROR_UNEXPECTED_CHARACHTER , char_to_string(formula[0]));                
            //}
            
            if(exist_in(operations_symbols , formula[0])){   
                //std::cout << "s1\n";
                char symbol = formula[0];
                clear_by_index(0, formula);
                //std::cout << "s2\n";
                std::cout << "\tOperation is: " << operations[symbol].symbol << std::endl;
                //std::cout << formula << std::endl;
                return operations[symbol];
            }
            else
            {
                std::cout << "operator_error " << formula << endl;
                throw std::make_pair(ERROR_UNEXPECTED_CHARACHTER , char_to_string(formula[0]));
            }

        }
        number solve(int &operand_index , int min_precedence=0){
            //std::cout << "Calculating " << formula << std::endl;
            
            number result = operands[operand_index];
            operand_index++;
            number operand;

            for(;operand_index<operands.size();operand_index++){
                //std::cout << operand_index << std::endl;
                Operation current_operation = operators[operand_index];
                if(operators[operand_index+1].priority > operators[operand_index].priority){
                    operand = solve(operand_index , operators[operand_index+1].priority);
                }else if(operators[operand_index+1].priority < min_precedence){//operators[operand_index].priority){
                    operand = operands[operand_index];
                    result = current_operation.operation(result , operand);
                    break;
                }else{
                    operand = operands[operand_index];
                } 
                result = current_operation.operation(result , operand);
            }
            return result;
        }
    public:
        void add_operation(Operation operation){
            if (!exist_in(operations_symbols , operation.symbol)){
                operations_symbols.append(char_to_string(operation.symbol));
                operations[operation.symbol] = operation;
                //std::cout << "Operation " << operation.symbol << " added\n";
            }
        }

        number calculate(std::string formula){
            operators.clear();
            operands.clear();
            operators.push_back(Operation());
            while(formula != ""){
                if(formula != ""){
                    operands.push_back(extract_number(formula));
                }
                if(formula != ""){
                    operators.push_back(extract_operation(formula));
                }
            }
            int i=0;
            return solve(i);

        }

        SimpleCalculator() {
            add_operation(Operation('+', PRIORITY_PLUS_MINUS, [](number o1, number o2){ return o1 + o2; }));
            add_operation(Operation('-', PRIORITY_PLUS_MINUS, [](number o1, number o2){ return o1 - o2; }));
            add_operation(Operation('*', PRIORITY_TIMES_DIVIDE, [](number o1, number o2){ return o1 * o2; }));
            add_operation(Operation('/', PRIORITY_TIMES_DIVIDE, [](number o1, number o2){
                if((o1 == 0) && (o2 == 0)){
                    throw ERROR_DIVISION_ZERO_BY_ZERO;
                } 
                if (o2 == 0){
                    throw ERROR_DIVISION_BY_ZERO;
                } 
                return o1 / o2; }));
            add_operation(Operation('^', PRIORITY_POWER, [](number o1, number o2){ return pow(o1, o2); }));
        }
};
auto is_integer_number = [](number n){
    return num::ceil(n) == n;
};
class Calculator{
    

    //std::map<std::string , math_function> functions;
    SimpleCalculator calc;
    std::map<std::string , number> vars;

    void add_variable(std::string var_name , number var_value){
        if (vars.find(var_name) == vars.end()){
           vars[var_name] = var_value;
        }
    }

    const std::map<std::string , number> &get_vars(){
        return vars;
    }

    bool check_brackets_syntax(string str)
    {
        stack<char> braces;
        for (int i = 0; i < str.length(); i++)
        {
            if (str[i] == '(')
            {
                braces.push(str[i]);
            }
            else if (str[i] == ')')
            {
                if (!braces.empty())
                {
                    braces.pop();
                }
                else
                {
                    //return false;
                    throw ERROR_BAD_BRACKETS;
                }
            }   
        }

        if (braces.empty()){
            return true;
        }
        else{
            //return false;
            throw ERROR_BAD_BRACKETS;
        }
}

    //A function that put '*' operator between any number and '(' or any ')' and '(' in order to give
    //the user the option to just enter 6(6) instead of 6*(6)
    //--------------------------------------------
    //@Param string &input to do this operation on  
    //return It returns nothing
    void utilize_brackets(string &input)
    {
        stringstream ss;

        /*for (int i = 0; i < input.length(); i++)
        {
            if ((input[i] == ')' || isNumber(input[i - 1])) && input[i] == '(')
            {
                ss << '*' << input[i] << "1*";
            }
            else
            {
                ss << input[i];
            }
        }*/

        for (int i = 0; i < input.length(); i++)
        {
            if (((input[i+1] == '(') || is_letter(input[i+1])) && (isNumber(input[i]) || (input[i]==')') ))
            {
                ss << input[i] << "*";
            }else{
                ss << input[i];
            }
            
        }
        

        input = ss.str();
    }

public:

    number calculate(std::string formula){
        clear(' ' , formula);
        std::cout << "\t\t\t\tformula: " << formula << "\n"; 
        auto my_to_string = [](number n){std::stringstream ss; ss << n; return ss.str();};
        for(auto var: vars){
            replace_each(formula , var.first , "("+var.second.str()+")");
        }

        if (formula == "")
        {
            throw ERROR_EMPTY_STRING;
        }
        

        check_brackets_syntax(formula);
        utilize_brackets(formula);

        //std::cout << "\t\t\t\tformula: " << formula << "\n"; 
        
        while (search_first(formula , char_to_string('('))  != -1){
            int down = search_first(formula , char_to_string('(')), up;
            stack<char> braces;
            for (int i = 0; i < formula.length(); i++){
                if (formula[i] == '('){
                    braces.push(formula[i]);
                }
                else if (formula[i] == ')'){   
                    braces.pop();
                }
                if (braces.empty() && (i > down)){   
                    up = i;
                    //std::string formula2 = copy_part_of_string(formula, down + 1, up - 1);
                    //cut(formula, down, up, to_string(calculate(formula2)));
                    //break;

                    //----------Functions support-----------
                    auto positions = search_all(formula , "," , down , up);
                    if (positions.size() != 0){
                        //std::cout << "I'm here\n";
                        int n=0;
                        std::string f_name = "";
                        for (int i=down-1 ; i>=0 ; i--){
                            if (is_letter(formula[i])){
                                n++;
                                insert_at(f_name , 0 , char_to_string(formula[i]));
                            }else{
                                break;
                            }
                        }
                        if (n==0 || !functions.exists(f_name , positions.size()+1)){
                            throw std::make_pair(ERROR_UNEXPECTED_CHARACHTER , ",");
                        }else{
                            //std::cout << "argc = " << positions.size()+1 << std::endl;
                            number num = functions.get_function(f_name , positions.size()+1).apply_function(copy_part_of_string(formula , down+1 , up-1));
                            //std::cout << "endl\n";
                            cut(formula , down-n , up , num.str());
                        }
                        
                    }else{
                        std::string formula2 = copy_part_of_string(formula, down + 1, up - 1);
                        //std::cout << "formula2" << formula2 << std::endl;
                        cut(formula, down, up, calculate(formula2).str());
                        break;
                    }
                    
                }
            }
        }
        return calc.calculate(formula);
    }

    /*number calculate_utf8(Glib::ustring formula){
        replace_each_ustring(formula , "\xC3\x97" , '*');
        replace_each_ustring(formula , "\xC3\xB7" , '/');
        //std::stringstream ss;
        //ss << formula;
        std::string f="";
        convert_ustring_to_string(formula , f);
        return calculate(f);//(ss.str());
    }*/

    Calculator(){
        vars["pi"] = M_PI;
        vars["e"] = M_E;

        arcsin_interval.conditions.push_back([](number n){return (n >= -1) && (n <=1);});
        arccos_interval.conditions.push_back([](number n){return (n >= -1) && (n <=1);});
        tan_interval.conditions.push_back([](number n){
            if(ANGLE_SYSTEM == RADIAN_SYSTEM){
                n /= DEGREE_SYSTEM;
            }
            return !((is_integer_number(n)) && ((int)n % 90 == 0));
            /*if(is_integer_number(round_to_nearest(n , 20))){
                return (int)n % 90 == 0; 
            }else{
                return false;
            }*/
            //return std::is_integral(n*pow(10 , 9) , 9)/std::pow(10 , 9) % 90;
            /*if (ANGLE_SYSTEM == DEGREE_SYSTEM){
                return (int)n/90 < round_to_nearest(n/90 , 9);
            }else if (ANGLE_SYSTEM == RADIAN_SYSTEM){
                n /= DEGREE_SYSTEM;
                return (int)n/90 < round_to_nearest(n/90 , 9);
            }*/
            throw "unexpected_move";
            return false;
        });
        real_interval.conditions.push_back([](number n){return true;});
        logarithmic_interval.conditions.push_back([](number n){return n >= 0;});

        arcsin_interval.error_message = "arcsin() domain is [-1 , 1]";
        arccos_interval.error_message = "arccos() domain is [-1 , 1]";
        logarithmic_interval.error_message = "Logarithms and sqrt don't accept negatives.";
        tan_interval.error_message = "tan() doesn't accept 90 or its multipeles.";
        /*functions.add_function(math_function("abs" , 1 , [](std::vector<number> numbers){return std::abs(numbers.begin()[0]);}));

        functions.add_function( math_function("log" , 1 , [](std::vector<number> numbers){return std::log10(numbers.begin()[0]);}));
        functions.add_function( math_function("log" , 2 , [](std::vector<number> numbers){return log(numbers.begin()[0] , numbers.begin()[1]);}));
        functions.add_function( math_function("ln" , 1 , [](std::vector<number> numbers){return std::log(numbers.begin()[0]);}));

        functions.add_function( math_function("pow" , 2 , [](std::vector<number> numbers){return std::pow(numbers.begin()[0] , numbers.begin()[1]);}));
        functions.add_function( math_function("root" , 2 , [](std::vector<number> numbers){return root(numbers.begin()[0] , numbers.begin()[1]);}));
        functions.add_function(math_function("sqrt" , 1 , [](std::vector<number> numbers){return std::sqrt(numbers.begin()[0]);}));

        functions.add_function( math_function("sin" , 1 , [](std::vector<number> numbers){return std::sin(numbers.begin()[0]*ANGLE_SYSTEM);}));
        functions.add_function( math_function("cos" , 1 , [](std::vector<number> numbers){return std::cos(numbers.begin()[0]*ANGLE_SYSTEM);}));
        functions.add_function( math_function("tan" , 1 , [](std::vector<number> numbers){return std::tan(numbers.begin()[0]*ANGLE_SYSTEM);}));

        functions.add_function(math_function("sec" , 1 , [](std::vector<number> numbers){return 1.0/std::cos(numbers.begin()[0]*ANGLE_SYSTEM);}));      
        functions.add_function( math_function("csc" , 1 , [](std::vector<number> numbers){return 1.0/std::sin(numbers.begin()[0]*ANGLE_SYSTEM);}));
        functions.add_function( math_function("cot" , 1 , [](std::vector<number> numbers){return 1.0/std::tan(numbers.begin()[0]*ANGLE_SYSTEM);}));

        functions.add_function( math_function("arcsin" , 1 , [](std::vector<number> numbers){return std::asin(numbers.begin()[0])*ANGLE_SYSTEM;}));
        functions.add_function( math_function("arccos" , 1 , [](std::vector<number> numbers){return std::acos(numbers.begin()[0])*ANGLE_SYSTEM;}));
        functions.add_function( math_function("arctan" , 1 , [](std::vector<number> numbers){return std::atan(numbers.begin()[0])*ANGLE_SYSTEM;}));
        */
    } 
};

    //std::map<std::pair<std::string , int> , math_function> math_function::functions = std::map<std::pair<std::string , int> , math_function>();
    math_function::math_function(std::string name , int argc , function<number(vector<number>)> call , Interval &interval){
        this->name = name;
        this->argc = argc;
        this->m_call = call;
        this->interval = &interval;
    }
    bool math_function::operator==(const math_function& other){
        return name == other.name && argc == other.argc;
    }
    math_function::math_function() {
        name = "";
        argc = -1;
        m_call = [](vector<number>){throw std::make_pair(ERROR_UNKNOWN_VARIABLE , ""); return number();};
    }
    number math_function::apply_function(string args){
        /*vector<int> args_pos = search_all(args , ",");
        args_pos.insert(args_pos.begin(), 0);
        args_pos.push_back(args.length()-1);
        std::cout << args_pos.size() << std::endl;
        vector<number> arguments;
        Calculator *calc = new Calculator;
        std::cout << "Calculator Constructed\n";
        for (int i = 0; i < (args_pos.size()-1); i++){
            std::cout << "Arg: " << copy_part_of_string(args , args_pos[i]+1 , args_pos[i+1]-1);
            arguments.push_back(calc->calculate(copy_part_of_string(args , args_pos[i]+1 , args_pos[i+1]-1)));
        }
        std::cout << "returning\n";
        delete calc;
        return call(arguments);*/

        std::stringstream ss;
        ss << args;
        std::string tmp;
        std::vector<number> arguments;
        std::unique_ptr<Calculator> calc(new Calculator);
        while (std::getline(ss , tmp , ',')){
            arguments.push_back(calc->calculate(tmp));
        }
        return call(arguments);
        
    }



}
