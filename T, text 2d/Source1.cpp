//enum class default_special_symbols
//{
//    Left            =   '<'   ,
//    Right           =   '>'   ,
//    Up              =   '^'   ,
//    Down            =   '|'   ,
//    Delete          =   '#'   ,
//    Backspace       =   '~'   ,
//    Home            =   '{'   ,
//    End             =   '}'   ,
//    Undo            =   '@'   ,
//    Redo            =   '$'   ,
//    Line_Folding    =   '+'
//};

//case (char)special_symbols::Down:
//    if (this->cursor.first != this->text.end() - 1)
//    {
//        int size_of_prev_line = this->cursor.first->size();
//        int size_of_curr_line = (++this->cursor.first)->size();
//        --this->cursor.first;
//        for (int i = 0; i < size_of_prev_line + 1; ++i)
//        {
//            this->execute_command((char)special_symbols::Right);
//        }
//        /*if (size_of_prev_line >= size_of_curr_line)
//        {
//        this->cursor.second = this->cursor.first->end();
//        }
//        else
//        {
//        --this->cursor.first;
//        for (int i = 0; i < size_of_prev_line + 1; ++i)
//        {
//        this->execute_command((char)special_symbols::Right);
//        }
//        }*/
//    }
//    break;
//case (char)special_symbols::Up:
//    if (this->cursor.first != this->text.begin())
//    {
//        int size_of_prev_line = this->cursor.first->size();
//        int size_of_curr_line = (--this->cursor.first)->size();
//        ++this->cursor.first;
//        for (int i = 0; i < size_of_curr_line + 1; ++i)
//        {
//            this->execute_command((char)special_symbols::Left);
//        }
//        /*if (size_of_prev_line >= size_of_curr_line)
//        {
//        this->cursor.second = this->cursor.first->end();
//        }
//        else
//        {
//        ++this->cursor.first;
//        for (int i = 0; i < size_of_curr_line + 1; ++i)
//        {
//        this->execute_command((char)special_symbols::Left);
//        }
//        }*/
//    }
//    break;

//
//void execute_command_without_of_saving_action(const char & symbol)
//{
//    switch (symbol)
//    {
//    case (char)special_symbols::Left:
//        if (this->cursor.second != this->cursor.first->begin())
//        {
//            --this->cursor.second;
//        }
//        else if (this->cursor.first != this->text.begin())
//        {
//            --this->cursor.first;
//            this->cursor.second = this->cursor.first->end();
//        }
//        break;
//    case (char)special_symbols::Right:
//        if (this->cursor.second != this->cursor.first->end())
//        {
//            ++this->cursor.second;
//        }
//        else if (this->cursor.first != this->text.end() - 1)
//        {
//            ++this->cursor.first;
//            this->cursor.second = this->cursor.first->begin();
//        }
//        break;
//    case (char)special_symbols::Down:
//        if (this->cursor.first != this->text.end() - 1)
//        {
//            auto prev_line = this->cursor.first;
//            auto old_pozition = this->cursor.second;
//            auto curr_line = ++this->cursor.first;
//            this->cursor.second = cursor.first->begin();
//            for (; old_pozition != prev_line->begin() && this->cursor.second != curr_line->end(); --old_pozition)
//            {
//                this->execute_command((char)special_symbols::Right);
//            }
//        }
//        break;
//    case (char)special_symbols::Up:
//        if (this->cursor.first != this->text.begin())
//        {
//            auto prev_line = this->cursor.first;
//            auto old_pozition = this->cursor.second;
//            auto curr_line = --this->cursor.first;
//            this->cursor.second = cursor.first->begin();
//            for (; old_pozition != prev_line->begin() && this->cursor.second != curr_line->end(); --old_pozition)
//            {
//                this->execute_command((char)special_symbols::Right);
//            }
//        }
//        break;
//    case (char)special_symbols::Delete:
//        if (this->cursor.second != this->cursor.first->end())
//        {
//            auto char_for_romove = this->cursor;
//            this->execute_command((char)special_symbols::Right);
//            char_for_romove.first->erase(char_for_romove.second);
//        }
//        else if (this->cursor.first != this->text.end() - 1)
//        {
//            auto line_for_lifting = ++this->cursor.first;
//            --this->cursor.first;
//            for (auto ch : *line_for_lifting)
//            {
//                this->cursor.first->push_back(ch);
//            }
//            this->text.erase(line_for_lifting);
//        }
//        break;
//    case (char)special_symbols::Backspace:
//        if (this->cursor.second != this->cursor.first->begin())
//        {
//            this->execute_command((char)special_symbols::Left);
//            auto char_for_romove = this->cursor;
//            this->execute_command((char)special_symbols::Right);
//            char_for_romove.first->erase(char_for_romove.second);
//        }
//        else if (this->cursor.first != this->text.begin())
//        {
//            auto line_for_lifting = this->cursor.first;
//            --this->cursor.first;
//            auto old_pozition = this->cursor.first->end();
//            for (auto ch : *line_for_lifting)
//            {
//                this->cursor.first->push_back(ch);
//            }
//            this->text.erase(line_for_lifting);
//            this->cursor.second = old_pozition;
//        }
//        break;
//    case (char)special_symbols::Home:
//        this->cursor.second = this->cursor.first->begin();
//        break;
//    case (char)special_symbols::End:
//        this->cursor.second = this->cursor.first->end();
//        break;
//    case (char)special_symbols::Line_Folding:
//        this->cursor.first = this->text.insert(++this->cursor.first, line());
//        this->cursor.second = this->cursor.first->begin();
//        break;
//    default:
//        this->cursor.first->insert(this->cursor.second, symbol);
//        break;
//    }
//}