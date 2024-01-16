#ifndef GRIDWINDOW_H
#define GRIDWINDOW_H

#include <gtkmm/window.h>
#include <gtkmm/grid.h>
#include <gtkmm/togglebutton.h>
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

    // Member widgets:
    Gtk::Grid m_grid;
    std::vector<std::vector<Gtk::ToggleButton>> m_buttons;
    std::vector<std::vector<bool>> next_state;
    int rows;
    int cols;
    bool loopRunning = false;
    sigc::connection loopConnection;
};

#endif // GRIDWINDOW_H