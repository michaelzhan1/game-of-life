#ifndef HELLOWORLD_H
#define HELLOWORLD_H

#include <gtkmm/button.h>
#include <gtkmm/window.h>

class HelloWorld: public Gtk::Window {
    public:
        HelloWorld();
        virtual ~HelloWorld();
    
    protected:
        // Signal handling
        void on_button_clicked();

        // Member widgets
        Gtk::Button m_button;
};

#endif // HELLOWORLD_H