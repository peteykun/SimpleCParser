#!/usr/bin/env python

# -----------------------------------------------------------------------------
# parse.py
#
# A simple parser for C
#
# Based on calc.py by David McNab
# -----------------------------------------------------------------------------

import sys
sys.path.insert(0,"../..")

import readline
import ply.lex as lex
import ply.yacc as yacc
import os

#class Node:
#    def __init__(self, name, children):
#        self.name = name
#        self.children = children

class Parser:
    """
    Base class for a lexer/parser that has the rules defined as methods
    """
    tokens = ()
    precedence = ()

    def __init__(self, **kw):
        self.debug = kw.get('debug', 0)
        self.names = { }
        try:
            modname = os.path.split(os.path.splitext(__file__)[0])[1] + "_" + self.__class__.__name__
        except:
            modname = "parser"+"_"+self.__class__.__name__
        self.debugfile = modname + ".dbg"
        self.tabmodule = modname + "_" + "parsetab"
        #print self.debugfile, self.tabmodule

        # Build the lexer and parser
        lex.lex(module=self, debug=self.debug)
        yacc.yacc(module=self,
                  debug=self.debug,
                  debugfile=self.debugfile,
                  tabmodule=self.tabmodule)

    def run(self):
        while 1:
            try:
                s = raw_input('calc > ')
            except EOFError:
                break
            if not s: continue
            yacc.parse(s)


class Calc(Parser):
    tokens = (
        'PLUS','MINUS','TIMES','DIVIDE','ASSIGN',
        'LPAREN','RPAREN','SEMICOL','COLON','LCURLY',
        'RCURLY','COMMA','IF','ELSE','SWITCH','CASE',
        'DEFAULT','WHILE','DO','FOR','BREAK','CONT',
        'RETURN','GT','GTEQ','LT','LTEQ','EQ','NEQ',
        'VOID','CHAR','SHORT','INT','LONG','FLOAT',
        'DOUBLE','SIGNED','UNSIGNED','IDENT','ICONST',
        'INCR','OR','RSHIFT_ASSIGN','TIMES_ASSIGN',
        'BOR_ASSIGN','PLUS_ASSIGN','LSHIFT_ASSIGN',
        'BXOR_ASSIGN','BOR','LSHIFT','BXOR','ARROW',
        'BAND_ASSIGN','MINUS_ASSIGN','AND','MOD_ASSIGN',
        'RSHIFT','DIVIDE_ASSIGN','DOT','QUEST','DECR',
        'BAND','BNOT','MOD','ADDRESS','NOT','CONST',
        'VOLATILE','STRUCT','UNION','LBOX','RBOX',
        'FCONST','CCONST','SCONST','ELLIPSIS'
        )

    # Operators
    t_PLUS     = r'\+'
    t_MINUS    = r'-'
    t_TIMES    = r'\*'
    t_DIVIDE   = r'/'
    t_MOD      = r'%'
    t_LPAREN   = r'\('
    t_RPAREN   = r'\)'
    t_SEMICOL  = r';'
    t_COLON    = r':'
    t_COMMA    = r','
    t_LCURLY   = r'\{'
    t_RCURLY   = r'\}'
    t_LBOX     = r'\['
    t_RBOX     = r'\]'
    t_BAND     = r'&'
    t_BOR      = r'\|'
    t_BXOR     = r'\^'
    t_BNOT     = r'~'
    t_AND      = r'&&'
    t_OR       = r'\|\|'
    t_NOT      = r'!'
    t_LSHIFT   = r'<<'
    t_RSHIFT   = r'>>'
    t_INCR     = r'\+\+'
    t_DECR     = r'--'
    t_ADDRESS  = r'&'
    t_QUEST    = r'\?'
    t_DOT      = r'\.'
    t_ARROW    = r'->'
    t_ELLIPSIS = r'\.\.\.'

    # Assignments
    t_ASSIGN        = r'='
    t_PLUS_ASSIGN   = r'\+='
    t_MINUS_ASSIGN  = r'-='
    t_TIMES_ASSIGN  = r'\*='
    t_DIVIDE_ASSIGN = r'/='
    t_MOD_ASSIGN    = r'%='
    t_LSHIFT_ASSIGN = r'<<='
    t_RSHIFT_ASSIGN = r'>>='
    t_BAND_ASSIGN   = r'&='
    t_BOR_ASSIGN    = r'\|='
    t_BXOR_ASSIGN   = r'\^='

    # Types
    def t_VOID(self, t):
        r'void'
        return t
    def t_CHAR(self, t):
        r'char'
        return t
    def t_SHORT(self, t):
        r'short'
        return t
    def t_INT(self, t):
        r'int'
        return t
    def t_LONG(self, t):
        r'long'
        return t
    def t_FLOAT(self, t):
        r'float'
        return t
    def t_DOUBLE(self, t):
        r'double'
        return t
    def t_SIGNED(self, t):
        r'signed'
        return t
    def t_UNSIGNED(self, t):
        r'unsigned'
        return t
    def t_STRUCT(self, t):
        r'struct'
        return t
    def t_UNION(self, t):
        r'union'
        return t
    def t_CONST(self, t):
        r'const'
        return t
    def t_VOLATILE(self, t):
        r'volatile'
        return t

    # Conditionals
    def t_IF(self, t):
        r'if'
        return t
    def t_ELSE(self, t):
        r'else'
        return t
    def t_SWITCH(self, t):
        r'switch'
        return t
    def t_CASE(self, t):
        r'case'
        return t
    def t_DEFAULT(self, t):
        r'default'
        return t

    # Loops
    def t_WHILE(self, t):
        r'while'
        return t
    def t_DO(self, t):
        r'do'
        return t
    def t_FOR(self, t):
        r'for'
        return t
    def t_BREAK(self, t):
        r'break'
        return t
    def t_CONT(self, t):
        r'continue'
        return t
    def t_RETURN(self, t):
        r'return'
        return t

    # Comparison
    t_GT      = r'>'
    t_GTEQ    = r'>='
    t_LT      = r'<'
    t_LTEQ    = r'<='
    t_EQ      = r'=='
    t_NEQ     = r'!='

    # Names
    def t_IDENT(self, t):
        r'[a-zA-Z_][a-zA-Z0-9_]*'
        return t

    def t_CCONST(self, t):
        r'\'(\\.|[^\\\'])\''
        return t

    def t_SCONST(self, t):
        r'"(\\.|[^\\"])*"'
        return t

    def t_FCONST(self, t):
        r'[+-]?\d*[.]\d+f?'
        try:
            t.value = float(t.value)
        except ValueError:
            print "Float value could not be parsed", t.value
            t.value = 0

        return t

    def t_ICONST(self, t):
        r'\d+'
        try:
            t.value = int(t.value)
        except ValueError:
            print "Integer value too large", t.value
            t.value = 0

        return t

    t_ignore = " \t"

    def t_newline(self, t):
        r'\n+'
        t.lexer.lineno += t.value.count("\n")

    def t_error(self, t):
        print "Illegal character '%s'" % t.value[0]
        t.lexer.skip(1)

    # Parsing rules

    precedence = (
        ('left','COMMA'),
        ('right','ASSIGN','PLUS_ASSIGN','MINUS_ASSIGN','TIMES_ASSIGN','DIVIDE_ASSIGN','MOD_ASSIGN','LSHIFT_ASSIGN','RSHIFT_ASSIGN','BAND_ASSIGN','BOR_ASSIGN','BXOR_ASSIGN'),
        ('left','OR'),
        ('left','AND'),
        ('left','BOR'),
        ('left','BXOR'),
        ('left','BAND'),
        ('left','EQ','NEQ'),
        ('left','LT','LTEQ','GT','GTEQ'),
        ('left','LSHIFT','RSHIFT'),
        ('left','PLUS','MINUS'),
        ('left','TIMES','DIVIDE','MOD'),
        ('right','NOT','BNOT','UPLUS','UMINUS','INCR','DECR','ADDRESS','INDIREC'),
        )

    # def p_translation_unit(self, p):
    #     """
    #     translation-unit : external-declaration
    #                     | external-declaration translation-unit
    #     """
    #
    # def p_external_declaration(self, p):
    #     """
    #     external-declaration : function-definition
    #                         | declaration
    #     """

    def p_statement_function(self, p):
        'statement : function-definition'

    # Compound statements
    def p_statement_compound(self, p):
        'statement : compound-statement'

    def p_compound_statement(self, p):
        'compound-statement : LCURLY statement-sequence RCURLY'

    def p_statement_sequence_continue(self, p):
        'statement-sequence : statement-sequence statement'

    def p_statement_sequence(self, p):
        'statement-sequence : '

    # Types
    def p_type_specifier(self, p):
        """
        type-specifier : VOID
                      | CHAR
                      | SHORT
                      | INT
                      | LONG
                      | FLOAT
                      | DOUBLE
                      | SIGNED
                      | UNSIGNED
                      | struct-or-union-specifier
        """

    def p_type_qualifier(self, p):
        """
        type-qualifier : CONST
                      | VOLATILE
        """

    # Structs and unions
    def p_statement_struct_or_union(self, p): #todo
        'statement : struct-or-union-specifier'

    def p_struct_or_union_specifier(self, p):
        """
        struct-or-union-specifier : STRUCT IDENT LCURLY struct-declarations RCURLY
                                 | UNION IDENT LCURLY struct-declarations RCURLY
        """

    def p_struct_or_union_specifier_noident(self, p):
        """
        struct-or-union-specifier : STRUCT LCURLY struct-declarations RCURLY
                                 | UNION LCURLY struct-declarations RCURLY
        """

    def p_struct_or_union_specifier_onlyident(self, p):
        """
        struct-or-union-specifier : STRUCT IDENT
                                 | UNION IDENT
        """

    def p_struct_declarations(self, p):
        """
        struct-declarations : struct-declaration SEMICOL struct-declarations
                           | struct-declaration SEMICOL
        """

    def p_struct_declaration(self, p):
        """
        struct-declaration : specifier-qualifier struct-declarator-list
        """

    def p_specifier_qualifier(self, p):
        """
        specifier-qualifier : type-specifier specifier-qualifier
                           | type-qualifier specifier-qualifier
                           |
        """

    def p_struct_declarator_list(self, p):
        """
        struct-declarator-list : struct-declarator-list COMMA struct-declarator
                              | struct-declarator
        """

    def p_struct_declarator(self, p):
        """
        struct-declarator : declarator
                         | declarator COLON expression
                         | COLON expression
        """

    def p_declarator2(self, p):
        """
        declarator : direct-declarator
                  | pointer direct-declarator
        """

    def p_pointer(self, p):
        """
        pointer : TIMES type-qualifier-list pointer
               | TIMES type-qualifier-list
        """

    def p_type_qualifier_list(self, p):
        """
        type-qualifier-list : type-qualifier type-qualifier-list
                           |
        """

    def p_direct_declarator(self, p):
        """
        direct-declarator : IDENT
                         | LPAREN declarator RPAREN
                         | direct-declarator LBOX RBOX
                         | direct-declarator LBOX expression RBOX
                         | direct-declarator LPAREN parameter-type-list RPAREN
                         | direct-declarator LPAREN identifier-list RPAREN
                         | direct-declarator LPAREN RPAREN
        """
        #todo function declaration

    def p_parameter_type_list(self, p):
        """
        parameter-type-list : parameter-list
                           | parameter-list COMMA ELLIPSIS
        """

    def p_parameter_list(self, p):
        """
        parameter-list : parameter-declaration
                      | parameter-declaration COMMA parameter-list
        """
    def p_parameter_declaration(self, p):
        """
        parameter-declaration : declaration-specifiers declarator
                             | declaration-specifiers
        """

    def p_identifier_list(self, p):
        """
        identifier-list : IDENT
                       | IDENT COMMA identifier-list
        """

    # Functions
    def p_function_definition(self, p):
        'function-definition : declarator compound-statement'

    def p_declaration_specifiers(self, p):
        """
        declaration-specifiers : type-specifier declaration-specifiers
                              | type-qualifier declaration-specifiers
                              |
        """

    def p_declarator(self, p):
        'declarator : declaration-specifiers IDENT LPAREN argument-list RPAREN'

    def p_argument_list(self, p):
        'argument-list : type-specifier IDENT COMMA argument-list'

    def p_argument_list_one(self, p):
        'argument-list : type-specifier IDENT'

    def p_argument_list_none(self, p):
        'argument-list : '

    # Conditional statements
    def p_statement_if(self, p):
        'statement : IF LPAREN expression RPAREN statement'
        if p[3] == 1     : p[0] = p[5]

    def p_statement_ifelse(self, p):
        'statement : IF LPAREN expression RPAREN statement ELSE statement'
        if p[3] == 1     : p[0] = p[5]
        else             : p[0] = p[7]

    def p_statement_switch(self, p):
        'statement : SWITCH LPAREN expression RPAREN statement'

    # Iteration statements
    def p_statement_while(self, p):
        'statement : WHILE LPAREN expression RPAREN statement'
        if p[3] == 1     : p[0] = p[5]

    def p_statement_do_while(self, p):
        'statement : DO statement WHILE LPAREN expression RPAREN SEMICOL'
        if p[3] == 1     : p[0] = p[5]

    def p_statement_for(self, p):
        'statement : FOR LPAREN expression SEMICOL expression SEMICOL expression RPAREN statement'
        if p[3] == 1     : p[0] = p[9]

    # Labeled statments
    def p_statement_label(self, p):
        'statement : IDENT COLON statement SEMICOL'
        print p[1]

    def p_statement_case(self, p):
        'statement : CASE expression COLON statement SEMICOL'
        print p[1]

    def p_statement_default(self, p):
        'statement : DEFAULT COLON statement SEMICOL'
        print p[1]

    # Jump statements
    def p_statement_break(self, p):
        'statement : BREAK SEMICOL'

    def p_statement_continue(self, p):
        'statement : CONT SEMICOL'

    def p_statement_return(self, p):
        'statement : RETURN expression SEMICOL'
        print p[2]

    # Expression statement
    def p_statement_expr(self, p):
        'statement : expression SEMICOL'
        print p[1]

    # Conditional expression
    def p_expression_conditional(self, p):
        'expression : expression QUEST expression COLON expression'
        if p[1] == 1     : p[0] = p[3]
        else             : p[0] = p[5]

    # Reference
    def p_expression_reference(self, p):
        """
        expression : expression DOT IDENT
                  | expression ARROW IDENT
        """

    # Binary operations
    def p_expression_binop(self, p):
        """
        expression : expression PLUS expression
                  | expression MINUS expression
                  | expression TIMES expression
                  | expression DIVIDE expression
                  | expression MOD expression
                  | expression EQ expression
                  | expression NEQ expression
                  | expression GT expression
                  | expression GTEQ expression
                  | expression LT expression
                  | expression LTEQ expression
                  | expression BAND expression
                  | expression BOR expression
                  | expression BXOR expression
                  | expression AND expression
                  | expression OR expression
                  | expression LSHIFT expression
                  | expression RSHIFT expression
                  | expression COMMA expression
        """
        if p[2] == '+'   : p[0] = p[1] + p[3]
        elif p[2] == '-' : p[0] = p[1] - p[3]
        elif p[2] == '*' : p[0] = p[1] * p[3]
        elif p[2] == '/' : p[0] = p[1] / p[3]
        elif p[2] == '==': p[0] = int(p[1] == p[3])
        elif p[2] == '!=': p[0] = int(p[1] != p[3])
        elif p[2] == '>=': p[0] = int(p[1] >= p[3])
        elif p[2] == '>' : p[0] = int(p[1] > p[3])
        elif p[2] == '<=': p[0] = int(p[1] <= p[3])
        elif p[2] == '<' : p[0] = int(p[1] < p[3])

    # Assignment expression
    def p_statement_assign(self, p):
        """
        expression : IDENT ASSIGN expression
                  | IDENT PLUS_ASSIGN expression
                  | IDENT MINUS_ASSIGN expression
                  | IDENT TIMES_ASSIGN expression
                  | IDENT DIVIDE_ASSIGN expression
                  | IDENT MOD_ASSIGN expression
                  | IDENT LSHIFT_ASSIGN expression
                  | IDENT RSHIFT_ASSIGN expression
                  | IDENT BAND_ASSIGN expression
                  | IDENT BOR_ASSIGN expression
                  | IDENT BXOR_ASSIGN expression
        """
        p[0] = p[3]
        self.names[p[1]] = p[3]

    # Unary operations
    def p_expression_unary(self, p):
        """
        expression : MINUS expression %prec UMINUS
                  | PLUS expression %prec UPLUS
                  | INCR expression
                  | DECR expression
                  | TIMES expression %prec INDIREC
                  | BNOT expression
                  | NOT expression
                  | ADDRESS expression
        """
        p[0] = -p[2]

    # Parentheses
    def p_expression_group(self, p):
        'expression : LPAREN expression RPAREN'
        p[0] = p[2]

    # Empty expression
    def p_expression_empty(self, p):
        'expression : '

    # Literals
    def p_expression_literal(self, p):
        """
        expression : ICONST
                  | FCONST
                  | CCONST
                  | SCONST
        """
        p[0] = p[1]

    def p_expression_name(self, p):
        'expression : IDENT'
        try:
            p[0] = self.names[p[1]]
        except LookupError:
            print "Undefined name '%s'" % p[1]
            p[0] = 0

    def p_error(self, p):
        print "Syntax error at '%s'" % p.value

if __name__ == '__main__':
    calc = Calc()
    calc.run()
