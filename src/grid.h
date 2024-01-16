#ifndef GRIDWINDOW_H
#define GRIDWINDOW_H

#include <gtkmm/window.h>
#include <gtkmm/grid.h>
#include <gtkmm/togglebutton.h>
#include <gtkmm/box.h>
#include <vector>

class GridWindow : public Gtk::Window
{
public:
    GridWindow(int rows, int cols);
    virtual ~GridWindow();

protected:
    // Signal handlers:
    void on_button_clicked(int row, int col);
    bool on_key_press_event(GdkEventKey* eventkey);

    // Game logic:
    bool update_grid();
    void stop_update_grid();

    // Menubar functions:
    void on_menu_help();
    void on_menu_about();
    void on_menu_start();
    void on_menu_stop();
    void on_menu_reset();
    void on_menu_grid_size();
    void on_menu_grid_speed();

    // Game option functions:
    void reset_grid();
    void set_grid_size(int newRows, int newCols);
    void set_speed(int newSpeed);

    // Member widgets:
    Gtk::Grid m_grid;
    Gtk::Box menuBar;
    Gtk::Box vbox;

    // Member variables:
    std::vector<std::vector<Gtk::ToggleButton>> m_buttons;
    std::vector<std::vector<bool>> next_state;
    int rows;
    int cols;
    bool loopRunning = false;
    sigc::connection loopConnection;
    int speed = 500;
};

#endif // GRIDWINDOW_H