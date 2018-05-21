#include<iostream>
#include<string>
#include<list>
#include<vector>

enum class default_special_symbols
{
    Left            =   '<'   ,
    Right           =   '>'   ,
    Up              =   '^'   ,
    Down            =   '|'   ,
    Delete          =   '#'   ,
    Backspace       =   '~'   ,
    Home            =   '{'   ,
    End             =   '}'   ,
    Undo            =   '@'   ,
    Redo            =   '$'   ,
    Line_Folding    =   '+'   ,
};

//template<class special_symbols = default_special_symbols>
class text_editor
{
private:
    enum line_status
    {
        not_active,
        active
    };
    typedef std::list<char> line;
    typedef std::list<std::pair<line, line_status>> text_t;
    typedef std::list<char>::iterator iterator_line;
    typedef std::list<std::pair<line, line_status>>::iterator vertical_iterator;
    typedef std::pair<vertical_iterator, iterator_line> cursor_t;
    typedef default_special_symbols special_symbols;

    class Action
    {
    protected:
        cursor_t cursor;
    public:
        Action(const cursor_t & cursor)
            : cursor(cursor)
        {
        }
        virtual void undo(text_editor & text) = 0;
        virtual void redo(text_editor & text) = 0;
    };

    class Insert : public Action
    {
        char symbol;
    public:
        Insert(const char & symbol, const cursor_t & cursor)
            : symbol(symbol), Action(cursor)
        {
        }

        void undo(text_editor & text) override
        {
            text.cursor = this->cursor;
            text.execute_command_without_of_saving_action((char)special_symbols::Backspace);
            this->cursor = text.cursor;
        }
        void redo(text_editor & text) override
        {
            text.cursor = this->cursor;
            text.execute_command_without_of_saving_action(symbol);
            this->cursor = text.cursor;
        }
    };

    class Delete : public Action
    {
        char symbol;
    public:
        Delete(const char symbol, const cursor_t & cursor)
            : symbol(symbol), Action(cursor)
        {
        }

        void undo(text_editor & text) override
        {
            text.cursor = this->cursor;
            if (symbol != (char)special_symbols::Line_Folding) 
            {
                text.execute_command_without_of_saving_action(symbol);
            }
            else
            {
                Line_Folding line_folding(this->cursor);
                line_folding.redo(text);
            }
            text.execute_command_without_of_saving_action((char)special_symbols::Left);
            this->cursor = text.cursor;
        }

        void redo(text_editor & text) override
        {
            text.cursor = this->cursor;
            text.execute_command_without_of_saving_action((char)special_symbols::Delete);
            this->cursor = text.cursor;
        }
    };

    class Backspace : public Action
    {
        char symbol;
    public:
        Backspace(const char symbol, const cursor_t & cursor)
            : symbol(symbol), Action(cursor)
        {
        }

        void undo(text_editor & text) override
        {
            text.cursor = this->cursor;
            if (symbol != (char)special_symbols::Line_Folding)
            {
                text.execute_command_without_of_saving_action(symbol);
            }
            else
            {
                Line_Folding line_folding(this->cursor);
                line_folding.redo(text);
            }
            this->cursor = text.cursor;
        }

        void redo(text_editor & text) override
        {
            text.cursor = this->cursor;
            text.execute_command_without_of_saving_action((char)special_symbols::Backspace);
            this->cursor = text.cursor;
        }
    };

    class Line_Folding : public Action
    {
    public:
        Line_Folding(const cursor_t & cursor)
            : Action(cursor)
        {
        }

        void undo(text_editor & text) override
        {
            text.cursor = this->cursor;
            text.execute_command_without_of_saving_action((char)special_symbols::Backspace);
            this->cursor = text.cursor;
        }

        void redo(text_editor & text) override
        {
            text.cursor = this->cursor;
            vertical_iterator recovery_line = (++text.cursor.first)--;
            recovery_line->second = line_status::active;
            while (text.cursor.second != text.cursor.first->first.end())
            {
                recovery_line->first.push_back(*(text.cursor.second));
                text.execute_command_without_of_saving_action((char)special_symbols::Delete);
            }
            text.cursor.first = recovery_line;
            text.cursor.second = text.cursor.first->first.begin();
            this->cursor = text.cursor;
        }
    };

    typedef std::vector<Action*> action_container_t;
    typedef std::vector<Action*>::iterator action_iterator;

    text_t text;
    cursor_t cursor;
    action_container_t actions;
    action_iterator last_action_it;

    bool execute_command_without_of_saving_action(const char & symbol);
    text_editor & memory_release_after(action_iterator & last_action_it);

public:
    text_editor();

    ~text_editor();

    vertical_iterator active_begin()
    {
        auto active_line_begin = this->text.begin();
        for (; active_line_begin->second != line_status::active && active_line_begin != this->text.end();
            ++active_line_begin)
        {
        }
        return active_line_begin;
    }

    vertical_iterator active_end()
    {
        if (this->text.empty())
        {
            return this->text.end();
        }
        auto active_line_end = --this->text.end();
        for (; active_line_end->second != line_status::active && active_line_end != this->text.begin();
            --active_line_end)
        {
        }
        return ++active_line_end;
    }

    vertical_iterator get_next_active(const vertical_iterator & it)
    {
        vertical_iterator new_it;
        for (++new_it; new_it->second != line_status::active; ++new_it);
        return new_it;
    }

    vertical_iterator get_prev_active(const vertical_iterator & it)
    {
        vertical_iterator new_it;
        for (--new_it; new_it->second != line_status::active; --new_it);
        return new_it;
    }

    vertical_iterator & next_active(vertical_iterator & it)
    {
        for (++it; it->second != line_status::active; ++it);
        return it;
    }

    vertical_iterator & prev_active(vertical_iterator & it)
    {
        for (--it; it->second != line_status::active; --it);
        return it;
    }

    bool execute_command(const char & symbol);

    bool execute_command(const std::string & text)
    {
        bool changes_was = false;
        for (auto symbol_it = text.begin(); symbol_it != text.end(); ++symbol_it)
        {
            changes_was |= this->execute_command(*symbol_it);
        }
        return changes_was;
    }

    std::string get_text() const
    {
        std::string text_string;
        for (auto line : this->text)
        {
            if (line.second == line_status::active)
            {
                for (auto ch : line.first)
                {
                    text_string.push_back(ch);
                }
                text_string.push_back('\n');
            }
            /*else
            {
                text_string.push_back('~');
                text_string.push_back('\n');
            }*/
        }
        text_string.pop_back();
        return text_string;
    }

    std::string get_cursor() const
    {
        return (std::string)"line started with " + 
            (this->cursor.first->first.empty() ? '0' : *this->cursor.first->first.begin()) + "; cursor before " +
            (this->cursor.second == this->cursor.first->first.end() ? '0' : *(this->cursor.second));
    }
};

text_editor & text_editor::memory_release_after(action_iterator & last_action_it)
{
    for (auto it = last_action_it; it != this->actions.end(); ++it)
    {
        delete *(it);
    }
    this->actions.erase(last_action_it, this->actions.end());
    return *this;
}

text_editor::text_editor()
    : last_action_it(this->actions.begin())
{
    text.push_back(make_pair(line(), line_status::active));
    cursor = make_pair(text.begin(), text.begin()->first.begin());
}

text_editor::~text_editor()
{
    for (auto & action : actions)
    {
        delete action;
    }
}

bool text_editor::execute_command_without_of_saving_action(const char & symbol)
{
    switch (symbol)
    {
    case (char)special_symbols::Left:
        if (this->cursor.second != this->cursor.first->first.begin())
        {
            --this->cursor.second;
            return true;
        }
        else if (this->cursor.first != this->active_begin())
        {
            this->prev_active(this->cursor.first);
            this->cursor.second = this->cursor.first->first.end();
            return true;
        }
        return false;
    case (char)special_symbols::Right:
        if (this->cursor.second != this->cursor.first->first.end())
        {
            ++this->cursor.second;
            return true;
        }
        else if (this->cursor.first != --this->active_end())
        {
            this->next_active(this->cursor.first);
            this->cursor.second = this->cursor.first->first.begin();
            return true;
        }
        return false;
    case (char)special_symbols::Down:
        if (this->cursor.first != --this->active_end())
        {
            vertical_iterator prev_line = this->cursor.first;
            iterator_line old_pozition = this->cursor.second;
            this->next_active(this->cursor.first);
            this->cursor.second = this->cursor.first->first.begin();
            for (; old_pozition != prev_line->first.begin() && this->cursor.second != this->cursor.first->first.end(); --old_pozition)
            {
                this->execute_command((char)special_symbols::Right);
            }
            return true;
        }
        return false;
    case (char)special_symbols::Up:
        if (this->cursor.first != this->text.begin())
        {
            vertical_iterator prev_line = this->cursor.first;
            iterator_line old_pozition = this->cursor.second;
            this->prev_active(this->cursor.first);
            this->cursor.second = this->cursor.first->first.begin();
            for (; old_pozition != prev_line->first.begin() && this->cursor.second != this->cursor.first->first.end(); --old_pozition)
            {
                this->execute_command((char)special_symbols::Right);
            }
            return true;
        }
        return false;
    case (char)special_symbols::Delete:
        if (this->cursor.second != this->cursor.first->first.end())
        {
            cursor_t char_for_romove = this->cursor;
            this->execute_command((char)special_symbols::Right);
            char_for_romove.first->first.erase(char_for_romove.second);
            return true;
        }
        else if (this->cursor.first != --this->active_end())
        {
            vertical_iterator line_for_lifting = (++this->cursor.first)--;
            iterator_line old_pozition = this->cursor.first->first.empty() ? this->cursor.first->first.begin() : --this->cursor.first->first.end();
            bool another_line_was_empty = this->cursor.first->first.empty();
            for (auto ch : line_for_lifting->first)
            {
                this->cursor.first->first.push_back(ch);
            }
            line_for_lifting->first.clear();
            line_for_lifting->second = line_status::not_active;
            this->cursor.second = another_line_was_empty ? this->cursor.first->first.begin() : ++old_pozition;
            return true;
        }
        return false;
    case (char)special_symbols::Backspace:
        if (this->cursor.second != this->cursor.first->first.begin())
        {
            this->execute_command((char)special_symbols::Left);
            auto char_for_romove = this->cursor;
            this->execute_command((char)special_symbols::Right);
            char_for_romove.first->first.erase(char_for_romove.second);
            return true;
        }
        else if (this->cursor.first != this->active_begin())
        {
            vertical_iterator line_for_lifting = this->cursor.first;
            this->prev_active(this->cursor.first);
            iterator_line old_pozition = this->cursor.first->first.empty() ? this->cursor.first->first.begin() : --this->cursor.first->first.end();
            bool another_line_was_empty = this->cursor.first->first.empty();
            for (auto ch : line_for_lifting->first)
            {
                this->cursor.first->first.push_back(ch);
            }
            line_for_lifting->first.clear();
            line_for_lifting->second = line_status::not_active;
            this->cursor.second = another_line_was_empty ? this->cursor.first->first.begin() : ++old_pozition;
            return true;
        }
        return false;
    case (char)special_symbols::Home:
        if (this->cursor.second != this->cursor.first->first.begin())
        {
            this->cursor.second = this->cursor.first->first.begin();
            return true;
        }
        return false;
    case (char)special_symbols::End:
        if (this->cursor.second != this->cursor.first->first.end())
        {
            this->cursor.second = this->cursor.first->first.end();
        }
        return false;
    case (char)special_symbols::Line_Folding:
    {
        line new_line;
        while (this->cursor.second != this->cursor.first->first.end())
        {
            new_line.push_back(*(this->cursor.second));
            this->execute_command_without_of_saving_action((char)special_symbols::Delete);
        }
        this->cursor.first = this->text.insert(++this->cursor.first, make_pair(line(new_line), line_status::active));
        this->cursor.second = this->cursor.first->first.begin();
        return true;
    }
    case (char)special_symbols::Undo:
        return false;
    case (char)special_symbols::Redo:
        return false;
    default:
        this->cursor.first->first.insert(this->cursor.second, symbol);
        return true;
    }
}

bool text_editor::execute_command(const char & symbol)
{
    switch (symbol)
    {
    case (char)special_symbols::Left:
        return this->execute_command_without_of_saving_action(symbol);
    case (char)special_symbols::Right:
        return this->execute_command_without_of_saving_action(symbol);
    case (char)special_symbols::Down:
        return this->execute_command_without_of_saving_action(symbol);
    case (char)special_symbols::Up:
        return this->execute_command_without_of_saving_action(symbol);
    case (char)special_symbols::Delete:
    {
        char sym = this->cursor.second == this->cursor.first->first.end() ? (char)special_symbols::Line_Folding : *this->cursor.second;
        if (this->execute_command_without_of_saving_action(symbol))
        {
            Delete * del = new Delete(sym, this->cursor);
            this->memory_release_after(last_action_it);
            actions.push_back(del);
            last_action_it = actions.end();
            return true;
        }
        return false;
    }
    case (char)special_symbols::Backspace:
    {
        char sym = this->cursor.second == this->cursor.first->first.begin() ? (char)special_symbols::Line_Folding : *((--this->cursor.second)++);
        if (this->execute_command_without_of_saving_action(symbol))
        {
            Backspace * backspace = new Backspace(sym, this->cursor);
            this->memory_release_after(last_action_it);
            actions.push_back(backspace);
            last_action_it = actions.end();
            return true;
        }
        return false;
    }
    case (char)special_symbols::Home:
        return this->execute_command_without_of_saving_action(symbol);
    case (char)special_symbols::End:
        return this->execute_command_without_of_saving_action(symbol);
    case (char)special_symbols::Line_Folding:
        if (this->execute_command_without_of_saving_action(symbol))
        {
            Line_Folding * line_folding = new Line_Folding(this->cursor);
            this->memory_release_after(last_action_it);
            actions.push_back(line_folding);
            last_action_it = actions.end();
            return true;
        }
        return false;
    case (char)special_symbols::Undo:
        if (this->last_action_it != actions.begin())
        {
            (*(--this->last_action_it))->undo(*this);
            return true;
        }
        return false;
    case (char)special_symbols::Redo:
        if (this->last_action_it != actions.end())
        {
            (*(this->last_action_it++))->redo(*this);
            return true;
        }
        return false;
    default:
        if (this->execute_command_without_of_saving_action(symbol))
        {
            Insert * insert = new Insert(symbol, this->cursor);
            this->memory_release_after(last_action_it);
            actions.push_back(insert);
            last_action_it = actions.end();
            return true;
        }
        return false;
    }
}

using namespace std;

int main()
{
    text_editor text;
    while (cin.good())
    {
        string commands;
        getline(cin, commands);
        text.execute_command(commands);
        text.execute_command('+');
        //cout << "\t\t\t\tSTART\n" << text.get_text() << "\n\t\t\t\tEND\nCursor: " << text.get_cursor() << '\n';
    }
    cout << text.get_text();
    getchar();
    getchar();
    return 0;
}


