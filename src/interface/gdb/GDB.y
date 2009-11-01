%filenames GDBParser
%baseclass-preinclude preinclude.h
%class-name GDBParser
%namespace GDB_Parser

%stype Aesalon::Interface::GDB::ParseElementWrapper

%token  STRING
%token  C_STRING
%token  TOKEN
%token  GDB_PROMPT

%%
const: C_STRING { $$ = $1; }
;
variable: STRING { $$ = $1; }
;
value:      const { $$ = $1; }
    |       tuple { $$ = $1; }
    |       list { $$ = $1; }
;

result: variable '=' value { $$ = new Aesalon::Interface::GDB::ResultElement($1, $3); }
;

list_entry: value { $$ = $1; }
    |       value ',' list_entry
    |       result { $$ = $1; }
    |       result ',' list_entry
;
list:       '[' ']'
    |       '[' list_entry ']'
;

tuple_entry: result { $$ = $1; }
    |       result ',' tuple_entry { $$ = $1; $$->get_parent().to<Aesalon::Interface::GDB::TupleElement>()->add_tuple_item($3); }
;
tuple:      '{' '}' { $$ = new Aesalon::Interface::GDB::TupleElement(); }
    |       '{' tuple_entry '}' { $$ = new Aesalon::Interface::GDB::TupleElement(); $$.to<Aesalon::Interface::GDB::TupleElement>()->add_tuple_item($2); }
;

gdb_output:   oob_record_list GDB_PROMPT
    |       oob_record_list result_record GDB_PROMPT
;

line:       gdb_output
    |       '\n'
;