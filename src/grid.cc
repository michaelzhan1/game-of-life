// File "grid.cc"
#include "grid.h"
#include <gtkmm/togglebutton.h>
#include <gtkmm/button.h>
#include <glibmm/main.h>
#include <gtkmm/actiongroup.h>
#include <gtkmm/action.h>
#include <gtkmm/uimanager.h>
#include <gtkmm/box.h>
#include <gtkmm/messagedialog.h>
#include <gtkmm/dialog.h>
#include <gtkmm/entry.h>
#include <iostream>
#include <vector>

GridWindow::GridWindow(int rows, int cols)
: vbox(Gtk::ORIENTATION_VERTICAL), rows(rows), cols(cols)
{
    // Menubar
    Glib::RefPtr<Gtk::ActionGroup> actionGroup = Gtk::ActionGroup::create();
    actionGroup->add(Gtk::Action::create("Options", "Options"));
    actionGroup->add(Gtk::Action::create("Start", "Start"));
    actionGroup->add(Gtk::Action::create("Stop", "Stop"));
    actionGroup->add(Gtk::Action::create("Reset", "Reset"));
    actionGroup->add(Gtk::Action::create("Grid Size", "Grid Size"));
    actionGroup->add(Gtk::Action::create("Speed", "Speed"));
    actionGroup->add(Gtk::Action::create("Info", "Info"));
    actionGroup->add(Gtk::Action::create("Help", "Help"));
    actionGroup->add(Gtk::Action::create("About", "About"));

    actionGroup->get_action("Start")->signal_activate().connect(
        sigc::mem_fun(*this, &GridWindow::on_menu_start)
    );
    actionGroup->get_action("Stop")->signal_activate().connect(
        sigc::mem_fun(*this, &GridWindow::on_menu_stop)
    );
    actionGroup->get_action("Reset")->signal_activate().connect(
        sigc::mem_fun(*this, &GridWindow::on_menu_reset)
    );
    actionGroup->get_action("Grid Size")->signal_activate().connect(
        sigc::mem_fun(*this, &GridWindow::on_menu_grid_size)
    );
    actionGroup->get_action("Speed")->signal_activate().connect(
        sigc::mem_fun(*this, &GridWindow::on_menu_grid_speed)
    );

    actionGroup->get_action("Help")->signal_activate().connect(
        sigc::mem_fun(*this, &GridWindow::on_menu_help)
    );
    actionGroup->get_action("About")->signal_activate().connect(
        sigc::mem_fun(*this, &GridWindow::on_menu_about)
    );

    Glib::RefPtr<Gtk::UIManager> uiManager = Gtk::UIManager::create();
    uiManager->insert_action_group(actionGroup);
    Glib::ustring ui_info =
        "<ui>"
        "   <menubar name='MenuBar'>"
        "       <menu action='Options'>"
        "           <menuitem action='Start'/>"
        "           <menuitem action='Stop'/>"
        "           <menuitem action='Reset'/>"
        "           <menuitem action='Grid Size'/>"
        "           <menuitem action='Speed'/>"
        "       </menu>"
        "       <menu action='Info'>"
        "           <menuitem action='Help'/>"
        "           <menuitem action='About'/>"
        "       </menu>"
        "   </menubar>"
        "</ui>";
    uiManager->add_ui_from_string(ui_info);

    Gtk::Widget* pMenubar = uiManager->get_widget("/MenuBar");
    if (pMenubar)
    {
        menuBar.pack_start(*pMenubar, Gtk::PACK_SHRINK);
    }

    set_title("Conway's Game of Life");
    set_border_width(10);
    set_default_size(800, 600);
    
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
            m_grid.attach(m_buttons[i][j], j, i, 1, 1);
        }
    }

    this->signal_key_press_event().connect(
        sigc::mem_fun(*this, &GridWindow::on_key_press_event),
        false
    );

    vbox.pack_start(menuBar, Gtk::PACK_SHRINK);
    vbox.pack_start(m_grid);

    add(vbox);
    show_all_children();
}

// Destructor
GridWindow::~GridWindow()
{
}

// ToggleButton click handler
void GridWindow::on_button_clicked(int row, int col)
{
}

// Keypress handler
bool GridWindow::on_key_press_event(GdkEventKey* eventkey)
{
    switch (eventkey->keyval) {
        case GDK_KEY_space:
            if (loopRunning)
            {
                stop_update_grid();
            }
            else
            {
                loopRunning = true;
                loopConnection = Glib::signal_timeout().connect(
                    sigc::mem_fun(*this, &GridWindow::update_grid),
                    speed
                );
                std::cout << "Loop started" << std::endl;
            }
            break;
        
        case GDK_KEY_Escape:
            close();
            break;
    }
    return true;
}

// Main game logic
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

// Stop the loop
void GridWindow::stop_update_grid()
{
    loopRunning = false;
    loopConnection.disconnect();
}

// Menu button: Start
void GridWindow::on_menu_start()
{
    loopRunning = true;
    loopConnection = Glib::signal_timeout().connect(
        sigc::mem_fun(*this, &GridWindow::update_grid),
        speed
    );
    std::cout << "Loop started" << std::endl;
}

// Menu button: Stop
void GridWindow::on_menu_stop()
{
    stop_update_grid();
}

// Menu button: Reset
void GridWindow::on_menu_reset()
{
    reset_grid();
}

// Open grid size dialog
void GridWindow::on_menu_grid_size()
{
    Gtk::Dialog dialog("Change grid size", *this);
    Gtk::Label label1("Rows:");
    Gtk::Label label2("Columns:");
    Gtk::Entry entry1;
    Gtk::Entry entry2;
    label1.set_can_focus(true);
    label1.grab_focus();
    entry1.set_placeholder_text((Glib::ustring) std::to_string(rows));
    entry2.set_placeholder_text((Glib::ustring) std::to_string(cols));

    dialog.get_content_area()->pack_start(label1);
    dialog.get_content_area()->pack_start(entry1);
    dialog.get_content_area()->pack_start(label2);
    dialog.get_content_area()->pack_start(entry2);

    dialog.add_button("OK", Gtk::RESPONSE_OK);
    dialog.add_button("Cancel", Gtk::RESPONSE_CANCEL);

    dialog.show_all_children();
    int result = dialog.run();

    if (result == Gtk::RESPONSE_OK)
    {
        try {
            int newRows = std::stoi(entry1.get_text());
            int newCols = std::stoi(entry2.get_text());
            set_grid_size(newRows, newCols);
        }
        catch (std::invalid_argument& e) {
            Gtk::MessageDialog dialog(*this, "Error");
            dialog.set_secondary_text("Please enter valid numbers.");
            dialog.run();
        }
    }
}

// Open grid speed dialog
void GridWindow::on_menu_grid_speed()
{
    Gtk::Dialog dialog("Change grid speed", *this);
    Gtk::Label label("Refresh time (ms)");
    Gtk::Entry entry;
    label.set_can_focus(true);
    label.grab_focus();
    entry.set_placeholder_text((Glib::ustring) std::to_string(speed));

    dialog.get_content_area()->pack_start(label);
    dialog.get_content_area()->pack_start(entry);

    dialog.add_button("OK", Gtk::RESPONSE_OK);
    dialog.add_button("Cancel", Gtk::RESPONSE_CANCEL);

    dialog.show_all_children();
    int result = dialog.run();

    if (result == Gtk::RESPONSE_OK)
    {
        try {
            int newSpeed = std::stoi(entry.get_text());
            set_speed(newSpeed);
        }
        catch (std::invalid_argument& e) {
            Gtk::MessageDialog dialog(*this, "Error");
            dialog.set_secondary_text("Please enter a valid number.");
            dialog.run();
        }
    }
}

// Open help dialog
void GridWindow::on_menu_help()
{
    Gtk::MessageDialog dialog(*this, "How-to");
    dialog.set_secondary_text("Click squares to toggle them on or off.\nPress spacebar to start or stop the simulation.\nPress escape to exit the program.");
    dialog.run();
}

// Open about dialog
void GridWindow::on_menu_about()
{
    Gtk::MessageDialog dialog(*this, "About");
    dialog.set_secondary_text("Conway's Game of Life\n\nMade by: Michael Zhan\n\nFollowing the game developed by John Conway.\nThis program is licensed under the MIT license.");
    dialog.run();
}

// Reset the grid
void GridWindow::reset_grid()
{
    stop_update_grid();
    std::cout << "Loop stopped" << std::endl;
    for (size_t i = 0; i < m_buttons.size(); i++)
    {
        for (size_t j = 0; j < m_buttons[i].size(); j++)
        {
            m_buttons[i][j].set_active(false);
        }
    }
}

// Set the speed of the loop
void GridWindow::set_speed(int newSpeed)
{
    speed = newSpeed;
    if (speed < 1) speed = 1;
    if (speed > 1000) speed = 1000;
}

// Set the size of the grid
void GridWindow::set_grid_size(int newRows, int newCols)
{
    rows = newRows;
    cols = newCols;

    if (rows < 3) rows = 3;
    if (cols < 3) cols = 3;
    if (rows > 30) rows = 30;
    if (cols > 50) cols = 50;

    std::cout << "Setting grid size to " << rows << "x" << cols << std::endl;

    for (size_t i = 0; i < m_buttons.size(); i++)
    {
        for (size_t j = 0; j < m_buttons[i].size(); j++)
        {
            m_grid.remove(m_buttons[i][j]);
        }
    }

    m_buttons.clear();
    next_state.clear();

    int squareSize = 1;

    for (int i = 0; i < rows; i++)
    {
        m_buttons.push_back(std::vector<Gtk::ToggleButton>());
        next_state.push_back(std::vector<bool>());
        for (int j = 0; j < cols; j++)
        {
            next_state[i].push_back(false);
            m_buttons[i].push_back(Gtk::ToggleButton());
            m_buttons[i][j].set_size_request(squareSize, squareSize);
            m_buttons[i][j].set_can_focus(false);
        }
    }

    std::cout << "m_buttons size " << m_buttons.size() << "x" << m_buttons[0].size() << std::endl;

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

    show_all_children();
    resize(1, 1);
}