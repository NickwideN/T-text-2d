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

typedef std::list<char> line;
typedef std::vector<line> text_t;
typedef std::list<char>::iterator iterator_line;
typedef std::vector<line>::iterator iterator_vector;
typedef std::pair<iterator_vector, iterator_line> cursor_t;
typedef default_special_symbols special_symbols;


//template<class special_symbols = default_special_symbols>
class text_editor
{
private:
    class Action;
    class Insert;
    class Delete;
    class Backspace;
    class Line_Folding;

    text_t text;
    cursor_t cursor;
    std::list<Action*> actions;

    
public:
    friend Insert;
private:
    void execute_command_without_of_saving_action(const char & symbol)
    {
        switch (symbol)
        {
        case (char)special_symbols::Left:
            if (this->cursor.second != this->cursor.first->begin())
            {
                --this->cursor.second;
            }
            else if (this->cursor.first != this->text.begin())
            {
                --this->cursor.first;
                this->cursor.second = this->cursor.first->end();
            }
            break;
        case (char)special_symbols::Right:
            if (this->cursor.second != this->cursor.first->end())
            {
                ++this->cursor.second;
            }
            else if (this->cursor.first != this->text.end() - 1)
            {
                ++this->cursor.first;
                this->cursor.second = this->cursor.first->begin();
            }
            break;
        case (char)special_symbols::Down:
            if (this->cursor.first != this->text.end() - 1)
            {
                auto prev_line = this->cursor.first;
                auto old_pozition = this->cursor.second;
                auto curr_line = ++this->cursor.first;
                this->cursor.second = cursor.first->begin();
                for (; old_pozition != prev_line->begin() && this->cursor.second != curr_line->end(); --old_pozition)
                {
                    this->execute_command((char)special_symbols::Right);
                }
            }
            break;
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
            }
            break;
        case (char)special_symbols::Delete:
            if (this->cursor.second != this->cursor.first->end())
            {
                auto char_for_romove = this->cursor;
                this->execute_command((char)special_symbols::Right);
                char_for_romove.first->erase(char_for_romove.second);
            }
            else if (this->cursor.first != this->text.end() - 1)
            {
                auto line_for_lifting = ++this->cursor.first;
                --this->cursor.first;
                for (auto ch : *line_for_lifting)
                {
                    this->cursor.first->push_back(ch);
                }
                this->text.erase(line_for_lifting);
            }
            break;
        case (char)special_symbols::Backspace:
            if (this->cursor.second != this->cursor.first->begin())
            {
                this->execute_command((char)special_symbols::Left);
                auto char_for_romove = this->cursor;
                this->execute_command((char)special_symbols::Right);
                char_for_romove.first->erase(char_for_romove.second);
            }
            else if (this->cursor.first != this->text.begin())
            {
                auto line_for_lifting = this->cursor.first;
                --this->cursor.first;
                auto old_pozition = this->cursor.first->end();
                for (auto ch : *line_for_lifting)
                {
                    this->cursor.first->push_back(ch);
                }
                this->text.erase(line_for_lifting);
                this->cursor.second = old_pozition;
            }
            break;
        case (char)special_symbols::Home:
            this->cursor.second = this->cursor.first->begin();
            break;
        case (char)special_symbols::End:
            this->cursor.second = this->cursor.first->end();
            break;
        case (char)special_symbols::Line_Folding:
            this->cursor.first = this->text.insert(++this->cursor.first, line());
            this->cursor.second = this->cursor.first->begin();
            break;
        default:
            this->cursor.first->insert(this->cursor.second, symbol);
            break;
        }
    }


public:
    void execute_command(const char & symbol)
    {
        execute_command_without_of_saving_action(symbol);
        switch (symbol)
        {
        case (char)special_symbols::Left:

            break;
        case (char)special_symbols::Right:
            
            break;
        case (char)special_symbols::Down:
            
            break;
        case (char)special_symbols::Up:
            
            break;
        case (char)special_symbols::Delete:
            Delete del;
            break;
        case (char)special_symbols::Backspace:
            
            break;
        case (char)special_symbols::Home:
            
            break;
        case (char)special_symbols::End:

            break;
        case (char)special_symbols::Line_Folding:
            
            break;
        default:
            Insert insert(symbol, this->cursor);
            break;
        }
    }
    text_editor()
    {    
        text.push_back(line());
        cursor = make_pair(text.begin(), text.begin()->begin());
    }

    

    void execute_command(const std::string & text)
    {
        for (auto symbol_it = text.begin(); symbol_it != text.end(); ++symbol_it)
        {
            this->execute_command(*symbol_it);
        }
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
        return (std::string)"line = " + std::to_string(this->cursor.first - this->text.begin()) + "; cursor before " +
            (this->cursor.second == (this->cursor.first)->end() ? '0' : *(this->cursor.second));
    }
};

class text_editor::Action
{
protected:
    cursor_t cursor;
    Action(const cursor_t & cursor)
        : cursor(cursor)
    {
    }
    virtual void undo(text_editor & text) = 0;
    virtual void redo(text_editor & text) = 0;
};

class text_editor::Insert : public text_editor::Action
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
        text.execute_command(symbol);
        this->cursor = text.cursor;
    }
};

class text_editor::Delete : public text_editor::Action
{
    char symbol;
public:
    Delete(const char & symbol, const cursor_t & cursor)
        : symbol(symbol), Action(cursor)
    {
    }

    void undo(text_editor & text) override
    {
        text.cursor = this->cursor;
        text.execute_command(symbol);
        text.execute_command((char)special_symbols::Left);
        this->cursor = text.cursor;
    }

    void redo(text_editor & text) override
    {
        text.cursor = this->cursor;
        text.execute_command((char)special_symbols::Delete);
        this->cursor = text.cursor;
    }
};

class text_editor::Backspace : public text_editor::Action
{
    char symbol;
public:
    Backspace(const char & symbol, const cursor_t & cursor)
        : symbol(symbol), Action(cursor)
    {
    }

    void undo(text_editor & text) override
    {
        text.cursor = this->cursor;
        text.execute_command(symbol);
        this->cursor = text.cursor;
    }

    void redo(text_editor & text) override
    {
        text.cursor = this->cursor;
        text.execute_command((char)special_symbols::Backspace);
        this->cursor = text.cursor;
    }
};

class text_editor::Line_Folding : public text_editor::Action
{
public:
    Line_Folding(const cursor_t & cursor)
        : Action(cursor)
    {
    }

    void undo(text_editor & text) override
    {
        text.cursor = this->cursor;
        text.execute_command((char)special_symbols::Backspace);
        this->cursor = text.cursor;
    }

    void redo(text_editor & text) override
    {
        text.cursor = this->cursor;
        text.execute_command((char)special_symbols::Line_Folding);
        this->cursor = text.cursor;
    }
};

using namespace std;

int main()
{
    text_editor text;
    while (cin.good())
    {
        string commands;
        getline(cin, commands);
        text.execute_command(commands);
        //text.execute_command('\n');
        cout << text.get_text() << "END\nCursor: " << text.get_cursor() << '\n';
    }
    getchar();
    getchar();
    return 0;
}

