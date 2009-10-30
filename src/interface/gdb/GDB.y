%filenames GDBParser
%baseclass-preinclude preinclude.h
%class-name GDBParser
%namespace GDB_Parser

%token  STRING
%token  C_STRING
%token  TOKEN
%token  GDB_PROMPT

%%
const: C_STRING;
variable: STRING;
value:      const
    |       tuple
    |       list
;
result: variable '=' value;

list_entry: value
    |       value ',' list_entry
    |       result
    |       result ',' list_entry
;
list:       '[' ']'
    |       '[' list_entry ']'
;

tuple_entry: result
    |       result ',' tuple_entry
;
tuple:      '{' '}'
    |       '{' tuple_entry '}'
;

gdb_line:   oob_record_list GDB_PROMPT
    |       oob_record_list result_record GDB_PROMPT
;

line:       gdb_line
    |       '\n'
;