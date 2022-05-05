#include <gtkmm.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <map>
#include <iomanip>
//#include <wchar.h>
#include "calculator.h"
#include "gtkmc.h"
#include "GPL.h"

using namespace Gtk;
using namespace my;

/*
window.csd{
border-radius:20px;
border:none;
}
box{
border-radius:15px;
}
headerbar{
border-radius: 15px 15px 0 0;
border:none;
}
*/
/*struct Keypad{
    TextView& textview;
    //std::vector<Gtk::Button> buttons;
    //std::map<std::string , int> index_of;
    std::map<std::string , Button> buttons;
public:
    Keypad(TextView& view){
        textview = view;
    }

    void add_key(std::string label){
        Gtk::Button button(label);
        button.signal_clicked().connect([&](){
            auto buffer = textview.get_buffer();
            buffer->set_text(buffer->get_text() + label);
        });
        buttons[label] = button;
    }

    void add_key(std::string label , function<void()> to_do){
        Gtk::Button button(label);
        button.signal_clicked().connect(to_do);
        buttons[label] = button;
    }
    void add_keys(std::initializer_list<std::string> list){
        for(auto button : list){
            add_key(button);
        }
    }
}*/

/*class myTextBuffer : public TextBuffer{
public:
    myTextBuffer(){
        
    }
    virtual void on_change(){

    }
    virtual void on_insert(const TextBuffer::iterator& pos, const Glib::ustring& text, int bytes){
        std::cout << "default handler\n";    
        
        if (text != "\n"){    
            set_text(get_text() + text);
        }
    }

};*/

/*class EventsController : public Box{
public:
    EventsController(){
        add_events(Gdk::EventMask::ALL_EVENTS_MASK);
        //on_event()
    }
    virtual bool on_event(GdkEvent *event){
        event->key.keyval
    }

    virtual bool on_key_release_event(GdkEventKey *key){
        std::cout << "events controller\n";
        if(key->keyval == GDK_KEY_Return){
            std::cout << "eventsctrl enter\n";
            return true;
        }
        return true;
    }
};*/

// # ! () &

namespace my{
    class QuickException : public std::exception{
    public:
        const char* error_message = "";
        QuickException(std::string error_message){
            this->error_message = error_message.c_str();
        }
        virtual const char* what(){
            return error_message;
        }
    };
    
   
    class Lambda{
        public:
        function<void(std::string , std::string)> to_do;
        Lambda(std::function<void(std::string , std::string)> to_do){
            this->to_do = to_do;
        }

        auto operator()(std::string arg1 , std::string arg2){
            return [this , arg1 , arg2](){to_do(arg1 , arg2);};
        }
    };
    /*class CustomLambda{
        public:
            CustomLambda(std::function<void()> func , ){

            }
            void operator()(){
                
            }
    };*/
}
class ErrorDisplay : public Revealer{
public:
    InfoBar infobar;
    Label label;
    ErrorDisplay(){
        add(infobar);
        infobar.set_message_type(MessageType::MESSAGE_ERROR);
        infobar.pack_start(label);
        infobar.set_size_request(-1 , 30);
    }
    void emit_error(std::string error){
        label.set_label(error);
        set_reveal_child(true);
        Glib::signal_timeout().connect([this](){
            set_reveal_child(false);
            return false;
        } , 2000);
    }
};

class ErrorLabel : public Label{
    public:
    void emit_error(std::string error){
        set_label(error);
        Glib::signal_timeout().connect([this](){
            set_label("");
            return false;
        } , 4000);
    }
};

class HistoryManager{
    Glib::RefPtr<TextView> textview;
    int index = -1; 
    std::list<std::pair<std::string , std::string>> history;
    void show_index(const bool second = false){
        auto iter = history.begin();
        std::advance(iter , index);
        if (second){
            textview->get_buffer()->set_text(iter->second);
        }else{
            textview->get_buffer()->set_text(iter->first);
        }
    }
public:
    const std::list<std::pair<std::string , std::string>>& get_vector(){
        return history;
    }
    void push_back(std::string formula , std::string result){
        auto iter = history.begin();
        iter++;
        history.insert(iter , make_pair(formula , result));
        index = 0;
    }
    void go_up(bool result = false){
        if(index+1 < history.size()){
            std::cout << "going up\n";
            index++;
            show_index(result);
        }
    }
    void go_down(bool result = false){
        if(index > 0){
            std::cout << "going down\n";
            index--;
            show_index(result);
        }
    }
    void go_at_index(int i , bool result = false){
        if(i >= 0 && i < history.size()){
            index = i;
            show_index(result);
        }
    }
    void set_current_formula(Glib::ustring formula){
        history.begin()->first = formula;
        history.begin()->second = formula;
    }
    HistoryManager(TextView& textview):textview(&textview){
        history.insert(history.begin() , make_pair("" , ""));
    }
};

/*class key : public Button{

public:

    static void set_alt_button(ToggleButton &toggle){
        toggle.signal_toggled().connect([&toggle](){
            key::alt_is_pressed = toggle.get_active();
        });
    }

    static HistoryManager history;
    static Glib::RefPtr<TextView> textview;
    static Glib::RefPtr<TextBuffer> buffer;
    static bool init_finished;
    static bool alt_is_pressed;
    std::string label = "";
    std::string alt_label = "";


    //function<void()> to_do = ;

    key(std::string label){    
        if (!init_finished){
            throw QuickException("Keyboard not initialized.");
        }
        buffer = textview->get_buffer();
        Button button(label);

        button.signal_clicked().connect([this](){ //You have to capture label by value because otherwise you will have a reference that will be called after this function scope ends, because this lambda is called after this function returns.

            
            buffer->insert_at_cursor(this->label);
            history.set_current_formula(buffer->get_text());

        });
    }

    static const bool init = [&](HistoryManager history , Glib::RefPtr<TextView> textview){
        key::history = history;
        key::textview = textview;
        key::buffer = textview->get_buffer();
        key::init_finished = true;
        key::alt_is_pressed = false;
        return true;
    }();
};*/

int main(int argc, char *argv[])
{   
    /*Glib::ustring u = "hello\xC3\x97world";
    std::cout << u.c_str() << std::endl;
    //replace_each_ustring(u , "\xC3\x97" , '*');
    std::cout << u << std::endl;
    std::string s = u;
    std::cout << s << std::endl;*/
    auto app = Gtk::Application::create();

    Glib::ustring str = "hello";
    Glib::ustring ustr = str;
    std::cout << ustr << std::endl;

    // srand(std::time(NULL));

    static Window window;
    HeaderBar titlebar;
    VBox main_box;
    Overlay main_overlay;
    ErrorLabel error_label;
    ScrolledWindow scrolled;
    TextView textview;
    HistoryManager history(textview);
    Button menu_button;
    Popover menu;

    typedef enum{
        SCIENTIFIC,
        FIXED,
    }NumberMode;

    static NumberMode number_mode = FIXED;
    //auto buffer = myTextBuffer::create();
    //textview.set_buffer(buffer);
    auto tag = TextBuffer::Tag::create();
    auto buffer = textview.get_buffer();
    std::string formula = "";

    std::array<Box, 5> boxes;
    // std::map<std::string , Button> keypad;
    Calculator calculator;

    /*-----Window-------*/
    window.set_titlebar(titlebar);


    /*-----Titlebar-----*/
    titlebar.set_show_close_button(true);
    titlebar.set_title("Calculator");

    /*-----Scrolled-----*/
    scrolled.set_policy(PolicyType::POLICY_AUTOMATIC , PolicyType::POLICY_NEVER);
    scrolled.set_overlay_scrolling(true);

    /*----TextBuffer------*/
    /*tag->property_size() = 35 * 1000;
    buffer->get_tag_table()->add(tag);
    buffer->signal_changed().connect([&]()
                                     { buffer->apply_tag(tag, buffer->begin(), buffer->end()); });
    */
    /*----Popover Menu---*/
    menu.set_relative_to(menu_button);
    VBox menu_box;
    /*----MenuButton-----*/
    menu_button.set_image_from_icon_name("view-more-symbolic");
    bool popover_is_shown = false;
    menu_button.signal_clicked().connect([&]{
            menu.popup();
            popover_is_shown = true;
    });
    Button degree , mode;
    ModelButton about;
    degree.set_label("Degree");
    auto active_system = make_pair("Degree" , DEGREE_SYSTEM);
    auto inactive_system = make_pair("Radian" , RADIAN_SYSTEM);
    degree.signal_clicked().connect([&](){
        std::swap(active_system , inactive_system);
        degree.set_label(active_system.first);
        ANGLE_SYSTEM = active_system.second;
    });
    mode.set_label("Fixed");
    auto active_mode = make_pair("Fixed" , FIXED);
    auto inactive_mode = make_pair("Scientific" , SCIENTIFIC);
    mode.signal_clicked().connect([&](){
        std::swap(active_mode , inactive_mode);
        mode.set_label(active_mode.first);
        number_mode = active_mode.second;
    });
    
    auto about_dialog = my::make_about_dialog("Pretty Calculator" , "org.gnome.Calculator" , {"Hamza Algohary"} , License::LICENSE_GPL_2_0);
    //std::stringstream ss;
    //std::ifstream fin("GPL_V2.txt");
    //ss << fin.rdbuf();
    about_dialog.set_license(GPL_LICENSE);
    about_dialog.set_version("Version 1.1");
    about_dialog.set_comments("A beautiful and simple yet functional Calculator.");
    about.set_label("About");
    about.signal_clicked().connect([&](){
        about_dialog.run();
        about_dialog.hide();
    });
    Gtk::HeaderBar about_headerbar;
    about_headerbar.set_show_close_button(true);
    about_headerbar.set_title("About Pretty Calculator");
    about_dialog.set_titlebar(about_headerbar);
    about_dialog.show_all_children();

    /*----TextView-------*/
    textview.set_editable(true);
    // textview.property_margin_left() = 0;
    // textview.property_margin_right() = 0;
    // textview.property_halign() = Align::ALIGN_FILL;
    textview.set_size_request(-1, 30);

    /*----Keypad--------*/
    static ToggleButton functions_key("f(x)");
    /*auto print = [&](std::string label , std::string alt_label){
        std::string target = label;
        if (functions_key.get_active()){
            target = alt_label;
        }
        buffer->insert_at_cursor(label);
        history.set_current_formula(buffer->get_text());
    };*/

    //auto key = [&](Glib::ustring label , std::string alt_label = ""){
    static Lambda print([&](std::string label , std::string alt_label){
        std::string target = label;
        if (functions_key.get_active()){
            target = alt_label;
        }
        buffer->insert_at_cursor(target);
        history.set_current_formula(buffer->get_text());
    });

    class key : public Button{
        public:
        Glib::ustring label = "";
        Glib::ustring alt_label = "";
        Glib::ustring style_class , style_class_alt;
        //const HistoryManager& m_history = history;
        key(std::string label , std::string alt_label="" , std::string style_class = "large-text" , std::string style_class_alt = "small-text"):label(label) , alt_label(alt_label){
            set_label(label);
            this->style_class = style_class;
            this->style_class_alt = style_class_alt;
            signal_clicked().connect(print(label , alt_label));
            Glib::signal_timeout().connect([this](){
                if (functions_key.get_active()){
                    set_label(this->alt_label);
                    get_style_context()->remove_class(this->style_class);
                    get_style_context()->add_class(this->style_class_alt);
                }else{
                    set_label(this->label);
                    get_style_context()->remove_class(this->style_class_alt);
                    get_style_context()->add_class(this->style_class);
                }
                if (!window.is_visible()){
                    return false;
                }
                return true;
            } , 50);
            //std::cout << label << std::endl;
            //buffer->insert_at_cursor(label);
        }
            /*button.signal_clicked().connect([&history , &buffer , &functions_key , alt_label ,label](){ //You have to capture label by value because otherwise you will have a reference that will be called after this function scope ends, because this lambda is called after this function returns.
                //std::wstring l = L"1";
                //const Glib::ustring* l = new Glib::ustring(label);
                //std::cout << *l << std::endl;
                std::string txt;
                if (functions_key.get_active()){
                    txt = alt_label;
                }else{
                    txt = label;
                }
                
                buffer->insert_at_cursor(label);
                history.set_current_formula(buffer->get_text());
                //std::cout << "entered\n";
                //std::string tmp = buffer->get_text();
                //std::cout << "Copied" << tmp.size() << " " << tmp.capacity() << std::endl;
                //tmp.append(label);
                //std::cout << "Concantenated\n";
                //buffer->set_text(label);
                //std::cout << "Done\n";
            });*/
            //return button;
    };
    
    //}
    auto power = key("^" , "e");
    auto plus = key("+" , "log");
    auto minus = key("-" , "ln");
    auto times = key("\xC3\x97" , "root");
    auto divide = key("\xC3\xB7" , "pi");
    auto dot = key("." , "sqrt");
    auto one = key("1" , "sin") , two = key("2" , "cos") , three = key("3" , "tan") , four = key("4" , "sec") , five = key("5" , "csc") , six = key("6" , "cot") ,
         seven = key("7" , "asin") , eight = key("8" , "acos") , nine = key("9" , "atan") , zero = key("0" , "abs");

    //auto sin_f = key("sin") , cos_f = key("cos") , tan_f = key("tan") , arcsin_f = key("arcsin") , arccos_f = key("arccos") , arctan_f = key("arctan");
    //auto sec_f = key("sec") , csc_f = key("csc") , cot_f = key("cot");
    //auto abs_f = key("abs") , sqrt_f = key("sqrt") , log_f = key("log") , ln_f = key("ln") , root_f = key("root");

    //Button numbers_key("123");
    //times.set_label()
    
    bool erase = false;
    static int precision = 15;
    auto adapt_utf8 = [](Glib::ustring u){
        std::string str = "";
        for (auto c : u){
            //input.erase(c);
            //std::cout << (char)c << std::endl;
            Glib::ustring m("\xC3\x97") , d("\xC3\xB7");
            if (c == m[0]){
                str.append("*");
            }else if (c == d[0]){
                str.append("/");
            }else{
                str.append(char_to_string((char)c));
            }
        }
        return str;
    };
    auto equal = TButton("=" , [&](){
        try{
            Glib::ustring formula = buffer->get_text();

            if (formula != ""){
                std::string result_string;
                std::stringstream result_stream;
                number result = calculator.calculate(adapt_utf8(formula));
                if(number_mode == SCIENTIFIC){
                    result_stream << std::scientific << result;
                    result_string = result_stream.str();

                }else if(number_mode == FIXED){
                    result = my::round_to_nearest(result , precision);
                    result_string = result.str();
                }
                replace_each(result_string , "e+" , "*10^");
                replace_each(result_string , "e-" , "*10^-");
                //result << /*std::fixed <<*/ calculator.calculate(formula);
                //std::cout << result.str();
                buffer->set_text(result_string);
                history.push_back(formula , result_string);
                history.set_current_formula(result_string); 
                erase = true;         
            }
            
        }catch(CalculatorError error){
            if (error == my::ERROR_EMPTY_STRING){
                error_label.emit_error("Syntax Error");
            }else if (error == my::ERROR_BAD_BRACKETS){
                error_label.emit_error("Brackets syntax error.");
            }else if (error == my::ERROR_DIVISION_BY_ZERO){
                error_label.emit_error("Division by zero tends to INFINITY!");
            }else if(error == my::ERROR_DIVISION_ZERO_BY_ZERO){
                error_label.emit_error("Can't divide 0/0.");
            }
            
        }catch(std::pair<CalculatorError , std::string> error){
            if (error.first == my::ERROR_UNEXPECTED_CHARACHTER){
                error_label.emit_error("Unexpected Charachter " + error.second);
            }else if (error.first == my::ERROR_UNKNOWN_VARIABLE){
                error_label.emit_error("Unknown Variable " + error.second);
            }else if (error.first == my::ERROR_OUT_OF_RANGE){
                auto target_func = my::functions.get_function(error.second , 1);
                error_label.emit_error(target_func.interval->error_message);
            }else if (error.first == my::ERROR_INVALID_ARGC){
                error_label.emit_error("Unexpected Arguments");
            }
            
            
        }
    });
    Gdk::Rectangle strong(0 , 0 , 0, 0) , weak(0, 0 ,0 ,0);
    buffer->signal_insert().connect([&](const Gtk::TextBuffer::iterator &iter, const Glib::ustring &str, int){
        //auto cursor_pos = TextBuffer::iterator();
        //textview.get_cursor_locations(cursor_pos , strong , weak);
        //if (buffer->end() == cursor_pos){
        if (iter != buffer->end()){
            erase = false;
        }
        
        if(erase){
            auto input = str[0];
            if (isNumber(input) || input == '.'){
                buffer->set_text("");
            }
            erase = false;
        }
    } , false);  

    auto clear = TButton("C" , [&](){
        buffer->set_text("");
    });

    /*static bool textview_focused = false;
    textview.signal_focus_in_event().connect([](GdkEventFocus* const&){
        textview_focused = true;
        return true;
    });
    textview.signal_focus_out_event().connect([](GdkEventFocus* const&){
        textview_focused = false;
        return true;
    });*/
    //;
    window.signal_event().connect([&](GdkEvent* event){
        auto key = event->key.keyval;
        if(key == GDK_KEY_Return){
            equal.activate();
            return true;
        }else if(key == GDK_KEY_Escape){
            buffer->set_text("");
        }
            /*if(textview.property_is_focus()){
                //buffer->insert_at_cursor(event->key.string);
                return false;
            }else{
                //std::cout << "insert in buffer\n";
                buffer->insert(buffer->end() , event->key.string);
            }  */
            return false; 

        //return true;
    });

    /*buffer->signal_changed().connect([&]{
        erase = true;
    });*/

    textview.signal_key_release_event().connect([&](GdkEventKey* event){
        auto key = event->keyval;
        if(key == GDK_KEY_Up){
            //std::cout << "Handling signal\n";
            history.go_up();
        }else if(key == GDK_KEY_Down){
            //std::cout << "Handling signal\n";
            history.go_down();
        }
        return false;
    });
    
    //window.set_default(textview);
    menu.signal_closed().connect([&]{
        popover_is_shown = false;
    });
    
    textview.signal_focus_out_event().connect([&](GdkEventFocus* const&){
        if (!popover_is_shown){
            textview.grab_focus();
        }
        return true;
    });

    /*----Packing-----*/

    window.add(main_box);
    //main_overlay.add_overlay(error_label);

    VBox numbers_and_functions_box , operations , operators , numbers_box;
    HBox keyboard , row1 , row2 , row3 , row4;
    FlowBox functions_box;
    //Stack numbers_and_functions_stack;

    titlebar.pack_start(menu_button);
    menu.add(menu_box);
    menu_box.pack_start(degree);
    menu_box.pack_start(mode);
    menu_box.pack_start(about);

    menu.show_all_children();
    menu_box.show_all_children();
    operators.set_homogeneous(true);

    /*auto functions_button = TButton("f(x)" , [&](){
        static bool functions_visible = false , numbers_visible = true; 
        if (numbers_visible){
            numbers_and_functions_stack.set_visible_child(functions_box);
        }else if(functions_visible){
            numbers_and_functions_stack.set_visible_child(numbers_box);
        }
        swap(functions_visible , numbers_visible);
    });*/

    main_box.pack_start(scrolled , PACK_EXPAND_PADDING);
    main_box.pack_start(error_label , PACK_SHRINK);
    main_box.pack_start(keyboard);

    scrolled.add(textview);
    keyboard.pack_start(numbers_box);
    keyboard.pack_start(operations);
    
    operations.pack_start(operators);
    operations.pack_start(equal);

    operators.pack_start(plus);
    operators.pack_start(minus);
    operators.pack_start(times);
    operators.pack_start(divide);
    operators.pack_start(power);

    /*numbers_and_functions_box.pack_start(numbers_and_functions_stack);
    numbers_and_functions_box.pack_start(row4);
    */
    //numbers_and_functions_stack.add(numbers_box);
    //numbers_and_functions_stack.add(functions_box);

    numbers_box.pack_start(row1);
    numbers_box.pack_start(row2);
    numbers_box.pack_start(row3);
    numbers_box.pack_start(row4);



    row1.pack_start(seven);
    row1.pack_start(eight);
    row1.pack_start(nine);
    row2.pack_start(four);
    row2.pack_start(five);
    row2.pack_start(six);
    row3.pack_start(one);
    row3.pack_start(two);
    row3.pack_start(three);

    row4.pack_start(zero);
    row4.pack_start(dot);
    row4.pack_start(functions_key);

    /*----Styling----*/
    auto provider = StyleContext::create();
    auto css = CssProvider::create();
    auto screen = Gdk::Screen::get_default();
    // top-left top-right bottom-right bottom-left
    auto update_style = [&](){
        //Gdk::RGBA fg_color;
        //window.get_style_context()->lookup_color("fg_color" , fg_color);
        Gtk::Window style;
        auto bg_color = style.get_style_context()->get_background_color();
        auto fg_color = style.get_style_context()->get_color();
        
        //std::cout << color.get_red() << " " << color.get_green() << " " << color.get_blue() << std::endl;
        std::stringstream ss;
        ss << ".same-background , .same-background text{background-color: rgb(" << bg_color.get_red()*255 << "," << bg_color.get_green()*255 << "," << bg_color.get_blue()*255 << "); border:none }";
        ss << ".same-background , .same-background text{color: rgb(" << fg_color.get_red()*255 << "," << fg_color.get_green()*255 << "," << fg_color.get_blue()*255 << "); }";
        ss << ".very-large-text{font-size:40px;}";
        ss << "button {border:none;}";
        ss << ".large-text{font-size:20px;} .small-text{font-size:15px;}";
        ss << ".italic{font-size:20px; font-style:italic;}";
        ss << ".square{border-radius:0 0 0 0;}";
        ss << ".top-left{border-radius:10px 0 0 0;}";
        ss << ".top-right{border-radius:0 10px 0 0;}";
        ss << ".bottom-right{border-radius:0 0 10px 0;}";
        ss << ".bottom-left{border-radius:0 0 0 10px}";
        ss << ".top{border-radius:10px 10px 0 0;}";
        ss << ".bottom{border-radius:0 0 10px 10px;}";
        ss << ".lonely{border-radius:10px;}";
        ss << "window , window > *{border-radius:0 0 15px 15px;}";
        ss << "window headerbar , window headerbar > box{border-radius:15px 15px 0px 0px;}";
        ss << "box.vertical , box.horizontal{border-radius:30px 30px 30px 30px;}";
        ss << "window > *:not(headerbar) {border-radius: 15px 15px 15px 15px;}";
        ss << ".simple-roundness{border-radius:10px 10px 10px 10px;}";
        ss << "window.maximized , window.maximized headerbar{border-radius: 0 0 0 0;}";
        //ss << "window , box{border-radius:10px;}"; 
        std::cout << ss.str() << "\n";
        try{
            css->load_from_data(ss.str());
        }catch(CssProviderError err){
            std::cout << err.what() << std::endl;
        }
    };

    menu_box.property_margin() = 5;
    row4.set_homogeneous(true);
    error_label.property_halign() = Gtk::Align::ALIGN_START;
    error_label.property_valign() = Gtk::Align::ALIGN_START;
    error_label.property_margin_left() = 5;
    //window.get_style_context()->signal_changed().connect(update_style);
    window.signal_style_updated().connect(update_style);
    window.get_style_context()->add_provider_for_screen(screen , css , GTK_STYLE_PROVIDER_PRIORITY_USER);
    textview.get_style_context()->add_class("very-large-text");
    menu.get_style_context()->add_class("simple-roundness");
    textview.set_margin_left(3);
    textview.set_margin_right(1);
    main_box.property_margin() = 5;
    numbers_box.property_margin() = 5;

    operators.get_style_context()->add_class("linked");
    row1.get_style_context()->add_class("linked");
    row2.get_style_context()->add_class("linked");
    row3.get_style_context()->add_class("linked");
    row4.get_style_context()->add_class("linked");

    seven.get_style_context()->add_class("top-left");
    nine.get_style_context()->add_class("top-right");
    zero.get_style_context()->add_class("bottom-left");
    functions_key.get_style_context()->add_class("bottom-right");
    functions_key.get_style_context()->add_class("large-text");

    plus.get_style_context()->add_class("top");
    power.get_style_context()->add_class("bottom");
    equal.get_style_context()->add_class("lonely");
    equal.get_style_context()->add_class("suggested-action");
    equal.get_style_context()->add_class("large-text");
        
    window.get_style_context()->add_class("same-background");
    textview.get_style_context()->add_class("same-background");
    titlebar.get_style_context()->add_class("same-background");

    eight.get_style_context()->add_class("square");
    four.get_style_context()->add_class("square");
    five.get_style_context()->add_class("square");
    six.get_style_context()->add_class("square");
    two.get_style_context()->add_class("square");
    dot.get_style_context()->add_class("square");
    one.get_style_context()->add_class("square");
    two.get_style_context()->add_class("square");
    three.get_style_context()->add_class("square");

    minus.get_style_context()->add_class("square");
    times.get_style_context()->add_class("square");
    divide.get_style_context()->add_class("square");
    row1.set_homogeneous(true);
    row2.set_homogeneous(true);
    row3.set_homogeneous(true);
    row4.set_homogeneous(true);
    /*for(auto c : row1.get_children()){
        c->get_style_context()->add_class("same-background");
    }*/

    //window.set_property("overflow" , true);

    operators.property_margin() = 5;
    //numbers_and_functions_stack.property_margin() = 5;
    equal.property_margin() = 5;


    window.show_all_children();
    app->run(window);
    return 0;
}



    /*main_box.pack_start(textview, PACK_EXPAND_WIDGET);
    for (auto &box : boxes)
    {
        main_box.pack_start(box, PACK_EXPAND_WIDGET);
        box.get_style_context()->add_class("linked");
        box.property_margin() = 0;
        box.set_homogeneous(true);
    }

    HBox brackets;
    // multi_pack_start(brackets ,  {keypad["("] , keypad[")"]} );
    brackets.get_style_context()->add_class("linked");*/

    /*multi_pack_start(boxes[0] , {keypad1["f"] , keypad1["^"] , keypad1["C"] , keypad1["+"]});
    multi_pack_start(boxes[1] , {keypad1["7"] , keypad1["8"] , keypad1["9"] , keypad1["-"]});
    multi_pack_start(boxes[2] , {keypad1["4"] , keypad1["5"] , keypad1["6"] , keypad1["*"]});
    multi_pack_start(boxes[3] , {keypad1["1"] , keypad1["2"] , keypad1["3"] , keypad1["/"]});
    multi_pack_start(boxes[4] , {keypad1["0"] , keypad1["()"] , keypad1["."] , keypad1["="]});*/

    // on_change e1(window , formula , [&](){buffer->set_text(formula);});

    /*-----Styling---*/

