#include "grid.h"
#include <gtkmm/application.h>
#include <gtkmm/cssprovider.h>

int main(int argc, char* argv[]) {
    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

    Glib::ustring cssFile = "src/styles.css";
    Glib::RefPtr<Gtk::CssProvider> cssProvider = Gtk::CssProvider::create();
    cssProvider->load_from_path(cssFile);
    Gtk::StyleContext::add_provider_for_screen(
        Gdk::Screen::get_default(),
        cssProvider,
        GTK_STYLE_PROVIDER_PRIORITY_USER
    );
    
    GridWindow grid(5, 5);

    // Shows the window and returns when it is closed.
    return app->run(grid);
}