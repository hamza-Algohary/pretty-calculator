//GTKMC = GTK Missing Convenience

//A header file giving some convenient functions and constructors
//that the worst GUI library on earth misses.

#ifndef GTKMC_H_
#define GTKMC_H_

#include <functional>
#include <initializer_list>
#include <vector>
#include <memory>

#include <gtkmm.h>
#include <any>
#include "graph.h"

namespace my{
    //Dialogs
    //Mouse postion
    //Input control (For example for TextView)
    //Drag and drop
    //Keyboard buttons

    pos get_cursor_pos(){
        auto seat =  Gdk::Display::get_default()->list_seats()[0];
        pos coord;
        seat->get_pointer()->get_position(coord.x , coord.y);
        return coord;
    }

    pos get_widget_position(Gtk::Widget& widget){
        pos widget_coord(widget.get_allocation().get_x() , widget.get_allocation().get_y());
        int x , y;
        widget.get_window()->get_position(x , y);
        pos window_coord(x , y);
        
        return window_coord + widget_coord;
    }
    
    pos get_cursor_pos_relative_to(Gtk::Widget &widget){
        return get_cursor_pos().relative_to(get_widget_position(widget));
    }

    Gtk::Button IButton(std::string icon_name , std::function<void()> on_click){
        Gtk::Button button;
        button.set_image_from_icon_name(icon_name);
        button.signal_clicked().connect(on_click);
        return button;
    }
    Gtk::Button TButton(std::string label , std::function<void()> on_click){
        Gtk::Button button;
        button.set_label(label);
        button.signal_clicked().connect(on_click);
        return button;
    }
    
    void multi_pack_start(Gtk::Bin& bin , std::initializer_list<std::reference_wrapper<Gtk::Widget>> widgets){
        bin.add(*widgets.begin());
    }
    void multi_pack_start(Gtk::Box &box , std::initializer_list<std::reference_wrapper<Gtk::Widget>> widgets , Gtk::PackOptions pack_option = Gtk::PACK_EXPAND_WIDGET){
        for(auto widget : widgets){
            box.pack_start(widget , pack_option);
        }
    }
    void multi_pack_start(Gtk::HeaderBar &box, std::initializer_list<std::reference_wrapper<Gtk::Widget>> widgets){
        for(auto widget : widgets){
            box.pack_start(widget);
        }
    }
    template <typename T>
    T w(T w , std::function<void(T&)> to_do , std::initializer_list<std::reference_wrapper<Gtk::Widget>> children){
        to_do(w);
        multi_pack_start(w , children);
        return w;
    }

    auto embed_in_main_loop(Gtk::Window& window, std::function<void()> to_do){
        sigc::connection c = Glib::signal_idle().connect([&](){
            to_do();
            if (window.is_visible()){
            }else{
                c.disconnect();
            }
            return true;
        } , 0);
    }

    static Gtk::AboutDialog make_about_dialog(std::string app_name , std::string icon_name , std::vector<Glib::ustring> authors , Gtk::License license , std::string website_url="" , std::string website_label=""){
        Gtk::AboutDialog about;
        //Gtk::HeaderBar headerbar;
        //headerbar.set_show_close_button(true);
        //headerbar.set_title("About" + app_name);
        about.set_resizable(false);
        //about.set_titlebar(headerbar);
        about.set_program_name(app_name);
        about.set_logo_icon_name(icon_name);
        about.set_license_type(license);
        about.set_authors(authors);
        about.show_all_children();
        about.set_website(website_url);
        about.set_website_label(website_label);
        //headerbar.show_all_children();
        about.show_all_children();
        return about;
    }
    /*void on_change(Gtk::Window& window , int& x , std::function<void()> to_do){
        embed_in_main_loop(window , [&](){
            static int x_last = x;
            if (x != x_last){
                x_last = x;
                to_do();
            }
            return true;
        });
    }*/

    class on_change{
        std::string* x_last = new string;
        sigc::connection c;
        //std::shared_ptr<std::string> x_last(std::string());
        public:
        on_change(Gtk::Window& window , std::string& x , std::function<void()> to_do){
                c = Glib::signal_idle().connect([&](){
                    if (x != *x_last){
                        *x_last = x;
                        to_do();
                    }
                    if (!window.is_visible()){
                        c.disconnect();
                    }
                    return true;
                } , 0);
        }
        ~on_change(){
            c.disconnect();
            delete x_last;
        }
    };
    /*struct on_change{
        std::string x_last = "";
        //sigc::connection c;
        on_change(Gtk::Window& window , std::string& x){ //, std::function<void()> to_do){
        //static std::string x_last;
        //embed_in_main_loop(window , [&](){
            x_last = x;
            if (x != x_last){
                x_last = x;
                to_do();
            }
            return true;
        });*/
        /*c = Glib::signal_idle().connect([&](){

            if (x != x_last){
                x_last = x;
                to_do();
            }
            
            if (!window.is_visible()){
                c.disconnect();
            }

            return true;
        } , 0);
        }
        ~on_change(){
            c.disconnect();
        }
    };*/

    //struct{
    //  std::vector<on_change> events;
    //}events;
    /*void on_change(Gtk::Window& window , double& x , std::function<void()> to_do){
        embed_in_main_loop(window , [&](){
            static double x_last = x;
            if (x != x_last){
                x_last = x;
                to_do();
            }
            return true;
        });
    }*/
};

#endif