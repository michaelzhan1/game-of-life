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

    // Member widgets:
    Gtk::Grid m_grid;
    std::vector<std::vector<Gtk::ToggleButton>> m_buttons;
    int rows;
    int cols;
};

#endif // GRIDWINDOW_H