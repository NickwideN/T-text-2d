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
    Insert          =   '*'
};

//template<class special_symbols = default_special_symbols>
class text_editor
{
private:
    typedef std::list<char> line;
    typedef std::list<line> text_t;
    typedef std::list<char>::iterator iterator_line;
    typedef std::list<line>::iterator iterator_vector;
    typedef std::pair<iterator_vector, iterator_line> cursor_t;
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
            text.execute_command(symbol);
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
            text.execute_command_without_of_saving_action(symbol);
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
            text.execute_command_without_of_saving_action((char)special_symbols::Line_Folding);
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
            for (auto ch : line)
            {
                text_string.push_back(ch);
            }
            text_string.push_back('\n');
        }
        return text_string;
    }

    std::string get_cursor() const
    {
        return (std::string)"line started with " + 
            (this->cursor.first->empty() ? '0' : *this->cursor.first->begin()) + "; cursor before " +
            (this->cursor.second == (this->cursor.first)->end() ? '0' : *(this->cursor.second));
    }
};

bool text_editor::execute_command_without_of_saving_action(const char & symbol)
{
    switch (symbol)
    {
    case (char)special_symbols::Left:
        if (this->cursor.second != this->cursor.first->begin())
        {
            --this->cursor.second;
            return true;
        }
        else if (this->cursor.first != this->text.begin())
        {
            --this->cursor.first;
            this->cursor.second = this->cursor.first->end();
            return true;
        }
        return false;
    case (char)special_symbols::Right:
        if (this->cursor.second != this->cursor.first->end())
        {
            ++this->cursor.second;
            return true;
        }
        else if (this->cursor.first != (--this->text.end())++)
        {
            ++this->cursor.first;
            this->cursor.second = this->cursor.first->begin();
            return true;
        }
        return false;
    case (char)special_symbols::Down:
        if (this->cursor.first != (--this->text.end())++)
        {
            auto prev_line = this->cursor.first;
            auto old_pozition = this->cursor.second;
            auto curr_line = ++this->cursor.first;
            this->cursor.second = cursor.first->begin();
            for (; old_pozition != prev_line->begin() && this->cursor.second != curr_line->end(); --old_pozition)
            {
                this->execute_command((char)special_symbols::Right);
            }
            return true;
        }
        return false;
    case (char)special_symbols::Up:
        if (this->cursor.first != this->text.begin())
        {
            auto prev_line = this->cursor.first;
            auto old_pozition = this->cursor.second;
            auto curr_line = --this->cursor.first;
            this->cursor.second = cursor.first->begin();
            for (; old_pozition != prev_line->begin() && this->cursor.second != curr_line->end(); --old_pozition)
            {
                this->execute_command((char)special_symbols::Right);
            }
            return true;
        }
        return false;
    case (char)special_symbols::Delete:
        if (this->cursor.second != this->cursor.first->end())
        {
            auto char_for_romove = this->cursor;
            this->execute_command((char)special_symbols::Right);
            char_for_romove.first->erase(char_for_romove.second);
            return true;
        }
        else if (this->cursor.first != (--this->text.end())++)
        {
            auto line_for_lifting = (++this->cursor.first)--;
            for (auto ch : *line_for_lifting)
            {
                this->cursor.first->push_back(ch);
            }
            this->text.erase(line_for_lifting);
            return true;
        }
        return false;
    case (char)special_symbols::Backspace:
        if (this->cursor.second != this->cursor.first->begin())
        {
            this->execute_command((char)special_symbols::Left);
            auto char_for_romove = this->cursor;
            this->execute_command((char)special_symbols::Right);
            char_for_romove.first->erase(char_for_romove.second);
            return true;
        }
        else if (this->cursor.first != this->text.begin())
        {
            auto line_for_lifting = this->cursor.first--;
            auto old_pozition = this->cursor.first->empty() ? this->cursor.first->begin() : --this->cursor.first->end();
            bool another_line_was_empty = this->cursor.first->empty();
            for (auto ch : *line_for_lifting)
            {
                this->cursor.first->push_back(ch);
            }
            this->text.erase(line_for_lifting);
            this->cursor.second = another_line_was_empty ? this->cursor.first->begin() : ++old_pozition;
            return true;
        }
        return false;
    case (char)special_symbols::Home:
        if (this->cursor.second != this->cursor.first->begin())
        {
            this->cursor.second = this->cursor.first->begin();
            return true;
        }
        return false;
    case (char)special_symbols::End:
        if (this->cursor.second != this->cursor.first->end())
        {
            this->cursor.second = this->cursor.first->end();
        }
        return false;
    case (char)special_symbols::Line_Folding:
    {
        line new_line;
        while (this->cursor.second != this->cursor.first->end())
        {
            new_line.push_back(*(this->cursor.second));
            this->execute_command_without_of_saving_action((char)special_symbols::Delete);
        }
        this->cursor.first = this->text.insert(++this->cursor.first, line(new_line));
        this->cursor.second = this->cursor.first->begin();
        return true;
    }
    case (char)special_symbols::Undo:
        return false;
    case (char)special_symbols::Redo:
        return false;
    default:
        this->cursor.first->insert(this->cursor.second, symbol);
        return true;
    }
}

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
    text.push_back(line());
    cursor = make_pair(text.begin(), text.begin()->begin());
}

text_editor::~text_editor()
{
    for (auto & action : actions)
    {
        delete action;
    }
    text.~list();
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
        char sym = this->cursor.second == this->cursor.first->end() ? (char)special_symbols::Line_Folding : *this->cursor.second;
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
        char sym = this->cursor.second == this->cursor.first->begin() ? (char)special_symbols::Line_Folding : *((--this->cursor.second)++);
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
        //cout << "\t\t\t\tSTART\n" << text.get_text() << "\t\t\t\tEND\nCursor: " << text.get_cursor() << '\n';
    }
    cout << text.get_text();
    getchar();
    getchar();
    return 0;
}

