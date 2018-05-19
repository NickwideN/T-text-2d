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