// File "grid.cc"
#include "grid.h"
#include <iostream>
#include <gtkmm/togglebutton.h>
#include <gtkmm/button.h>
#include <glibmm/main.h>
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
        next_state.push_back(std::vector<bool>());
        for (int j = 0; j < cols; j++)
        {
            next_state[i].push_back(false);
            m_buttons[i].push_back(Gtk::ToggleButton());
            m_buttons[i][j].set_size_request(40, 40);
            m_buttons[i][j].set_can_focus(false);
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
            m_grid.attach(m_buttons[i][j], i, j, 1, 1);
        }
    }

    this->signal_key_press_event().connect(
        sigc::mem_fun(*this, &GridWindow::on_key_press_event),
        false
    );

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


bool GridWindow::on_key_press_event(GdkEventKey* eventkey)
{
    std::cout << "Key pressed: " << eventkey->keyval << std::endl;
    std::cout << "" << std::endl;
    if (eventkey->keyval == 32)
    {
        if (loopRunning)
        {
            stop_update_grid();
        }
        else
        {
            loopRunning = true;
            loopConnection = Glib::signal_timeout().connect(
                sigc::mem_fun(*this, &GridWindow::update_grid),
                100
            );
            std::cout << "Loop started" << std::endl;
        }
    }
    return true;
}


bool GridWindow::update_grid()
{
    std::cout << "Updating grid" << std::endl;
    int neighborCount;
    bool changed = false;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            neighborCount = 0;
            for (int di = -1; di < 2; di++)
            {
                for (int dj = -1; dj < 2; dj++)
                {
                    if (di == 0 && dj == 0)
                    {
                        continue;
                    }
                    int ni = i + di;
                    int nj = j + dj;
                    if (ni >= 0 && ni < rows && nj >= 0 && nj < cols)
                    {
                        if (m_buttons[ni][nj].get_active())
                        {
                            neighborCount++;
                        }
                    }
                }
            }

            if (m_buttons[i][j].get_active())
            {
                if (neighborCount < 2 || neighborCount > 3)
                {
                    next_state[i][j] = false;
                    changed = true;
                }
                else
                {
                    next_state[i][j] = true;
                }
            }
            else
            {
                if (neighborCount == 3)
                {
                    next_state[i][j] = true;
                    changed = true;
                }
                else
                {
                    next_state[i][j] = false;
                }
            }
        }
    }

    if (!changed)
    {
        stop_update_grid();
        std::cout << "Loop stopped" << std::endl;
    }

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            m_buttons[i][j].set_active(next_state[i][j]);
        }
    }
    return loopRunning;
}

void GridWindow::stop_update_grid()
{
    loopRunning = false;
    loopConnection.disconnect();
}