// File "grid.cc"
#include "grid.h"
#include <iostream>
#include <gtkmm/togglebutton.h>
#include <gtkmm/button.h>
#include <vector>

GridWindow::GridWindow(int rows, int cols)
: rows(rows), cols(cols)
{
    set_title("Conway's Game of Life");
    set_border_width(10);
    set_default_size(400, 400);
    
    for (int i = 0; i < rows; i++)
    {
        m_buttons.push_back(std::vector<Gtk::ToggleButton>());
        for (int j = 0; j < cols; j++)
        {
            m_buttons[i].push_back(Gtk::ToggleButton());
            m_buttons[i][j].get_style_context()->add_class("rectangular");
        }
    }

    m_grid.set_row_homogeneous(true);
    m_grid.set_column_homogeneous(true);

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            m_buttons[i][j].signal_clicked().connect(
                sigc::bind<int, int>(sigc::mem_fun(*this, &GridWindow::on_button_clicked), i, j)
            );
            m_grid.attach(m_buttons[i][j], j, i, 1, 1);
        }
    }

    add(m_grid);
    show_all_children();
}

GridWindow::~GridWindow()
{
}

void GridWindow::on_button_clicked(int row, int col)
{
    bool active = m_buttons[row][col].get_active();
    std::cout << "Button clicked: " << row << ", " << col << std::endl;
    std::cout << "Active: " << active << std::endl;
    std::cout << "" << std::endl;
}