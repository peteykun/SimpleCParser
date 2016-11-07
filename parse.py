#!/usr/bin/env python

# -----------------------------------------------------------------------------
# parse.py
#
# A simple parser for C
#
# Based on:
# - calc.py by David McNab
#   https://github.com/dabeaz/ply/blob/master/example/newclasscalc/calc.py
# - ANSI C grammar by Jutta Degener
#   https://www.lysator.liu.se/c/ANSI-C-grammar-y.html
#
# Pre-parsing instructions:
# http://eli.thegreenplace.net/2015/on-parsing-c-type-declarations-and-fake-headers
# -----------------------------------------------------------------------------

import readline
import ply.lex as lex
import ply.yacc as yacc
import os, sys, argparse

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

    def run(self, args=None):
        if args is not None and args.interactive:
            while True:
                s = raw_input('calc > ')
                yacc.parse(s)
        else:
            s = sys.stdin.read()
            yacc.parse(s)

class SimpleCParser(Parser):
    # lexer implementation based on pycparser, simplified greatly
    # assumes that type definitions are never masked
    _type_definitions = []

    keywords = (
        '_BOOL','_COMPLEX','AUTO','BREAK','CASE','CHAR','CONST',
        'CONTINUE','DEFAULT','DO','DOUBLE','ELSE','ENUM','EXTERN',
        'FLOAT','FOR','GOTO','IF','INT','LONG','REGISTER',
        'RETURN','SHORT','SIGNED','SIZEOF','STATIC','STRUCT',
        'SWITCH','TYPEDEF','UNION','UNSIGNED','VOID','VOLATILE',
        'WHILE',
    )

    keyword_map = {}
    for keyword in keywords:
        if keyword == '_BOOL':
            keyword_map['_Bool'] = keyword
        elif keyword =='_Complex':
            keyword_map['_Complex'] = keyword
        else:
            keyword_map[keyword.lower()] = keyword
    print keyword_map.keys()

    tokens = keywords + (
        # Identifiers
        'IDENT',

        # Type identifiers (typedefs)
        'TYPE_NAME',

        # Constants
        'ICONST_DEC','ICONST_OCT','ICONST_HEX','ICONST_BIN',
        'FCONST','FCONST_HEX',
        'CCONST',
        'SCONST',

        # Operators
        'PLUS','MINUS','TIMES','DIVIDE','MOD',
        'BAND','BOR','BNOT','BXOR','LSHIFT','RSHIFT',
        'AND','OR','NOT',
        'GT','GTEQ','LT','LTEQ','EQ','NEQ',

        # Assignment
        'ASSIGN','TIMES_ASSIGN','DIVIDE_ASSIGN','MOD_ASSIGN',
        'PLUS_ASSIGN','MINUS_ASSIGN',
        'LSHIFT_ASSIGN','RSHIFT_ASSIGN',
        'BAND_ASSIGN','BOR_ASSIGN','BXOR_ASSIGN',

        # Increment/decrement
        'INCR','DECR',

        # Structure dereference (->)
        'ARROW',

        # Conditional operator (?)
        'QUEST',

        # Delimiters
        'LPAREN','RPAREN',      # ( )
        'LBOX','RBOX',          # [ ]
        'LCURLY','RCURLY',      # { }
        'COMMA','DOT',          # , .
        'SEMICOL','COLON',      # ; :

        # Ellipsis (...)
        'ELLIPSIS',
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
    t_QUEST    = r'\?'
    t_DOT      = r'\.'
    t_ARROW    = r'->'
    t_ELLIPSIS = r'\.\.\.'
    t_LCURLY   = r'\{'
    t_RCURLY   = r'\}'

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

        if t.value in self.keyword_map.keys():
            t.type = self.keyword_map[t.value]
        elif t.value in self._type_definitions:
            t.type = 'TYPE_NAME'

        return t

    def t_CCONST(self, t):
        r'\'(\\.|\\\d+|[^\\\'])\''
        return t

    def t_SCONST(self, t):
        r'"(\\.|[^\\"])*"'
        return t

    def t_FCONST(self, t):
        r'((((([0-9]*\.[0-9]+)|([0-9]+\.))([eE][-+]?[0-9]+)?)|([0-9]+([eE][-+]?[0-9]+)))[FfLl]?)'
        return t

    def t_FCONST_HEX(self, t):
        r'(0[xX]([0-9a-fA-F]+|((([0-9a-fA-F]+)?\.[0-9a-fA-F]+)|([0-9a-fA-F]+\.)))([pP][+-]?[0-9]+)[FfLl]?)'
        return t

    def t_ICONST_HEX(self, t):
        r'0[xX][0-9a-fA-F]+(([uU]ll)|([uU]LL)|(ll[uU]?)|(LL[uU]?)|([uU][lL])|([lL][uU]?)|[uU])?'
        return t

    def t_ICONST_BIN(self, t):
        r'0[bB][01]+(([uU]ll)|([uU]LL)|(ll[uU]?)|(LL[uU]?)|([uU][lL])|([lL][uU]?)|[uU])?'
        return t

    def t_ICONST_OCT(self, t):
        r'0[0-7]+(([uU]ll)|([uU]LL)|(ll[uU]?)|(LL[uU]?)|([uU][lL])|([lL][uU]?)|[uU])?'
        return t

    def t_ICONST_DEC(self, t):
        r'(0|[1-9][0-9]*)(([uU]ll)|([uU]LL)|(ll[uU]?)|(LL[uU]?)|([uU][lL])|([lL][uU]?)|[uU])?'
        return t

    t_ignore = " \t"
    lineno   = 0

    def t_newline(self, t):
        r'\n+'
        t.lexer.lineno += t.value.count("\n")
        self.lineno = t.lexer.lineno

    def t_error(self, t):
        print "Illegal character '%s' on line %d" % (t.value[0], t.lexer.lineno)
        t.lexer.skip(1)

    def _compose(self, p):
        i = 1
        result = []

        try:
            while p[i] is not None:
                if isinstance(p[i], list):
                    result += p[i]
                else:
                    result += [p[i]]
                i += 1
        except IndexError:
            pass

        return result

    precedence = (
        ('left','COMMA'),
        ('right','ASSIGN','PLUS_ASSIGN','MINUS_ASSIGN','TIMES_ASSIGN','DIVIDE_ASSIGN','MOD_ASSIGN','LSHIFT_ASSIGN','RSHIFT_ASSIGN','BAND_ASSIGN','BOR_ASSIGN','BXOR_ASSIGN'),
        ('right','CONDITIONAL'),
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
        ('right','NOT','BNOT','INCR','DECR'), # 'UMINUS','UPLUS','ADDRESS','INDIREC','CAST'
        ('left','DOT','ARROW')
    )

    def p_translation_unit(self, p):
        """
        translation_unit : external_declaration
          | translation_unit external_declaration
        """
        p[0] = self._compose(p)

        #print p[0]

    def p_external_declaration(self, p):
        """
        external_declaration : function_definition
          | declaration
        """
        p[0] = self._compose(p)

    def p_function_definition(self, p):
        """
        function_definition : declaration_specifiers declarator declaration_list compound_statement
          | declaration_specifiers declarator compound_statement
          | declarator declaration_list compound_statement
          | declarator compound_statement
        """
        p[0] = self._compose(p)

    def p_primary_expression(self, p):
        """
        primary_expression : IDENT
          | ICONST_HEX
          | ICONST_OCT
          | ICONST_BIN
          | ICONST_DEC
          | FCONST_HEX
          | FCONST
          | CCONST
          | SCONST
          | LPAREN expression RPAREN
        """
        p[0] = self._compose(p)

    def p_postfix_expression(self, p):
        """
        postfix_expression : primary_expression
          | postfix_expression LBOX expression RBOX
          | postfix_expression LPAREN RPAREN
          | postfix_expression LPAREN argument_expression_list RPAREN
          | postfix_expression DOT IDENT
          | postfix_expression DOT TYPE_NAME
          | postfix_expression ARROW IDENT
          | postfix_expression ARROW TYPE_NAME
          | postfix_expression INCR
          | postfix_expression DECR
        """
        p[0] = self._compose(p)

    def p_argument_expression_list(self, p):
        """
        argument_expression_list : assignment_expression
          | argument_expression_list COMMA assignment_expression
        """
        p[0] = self._compose(p)

    def p_unary_expression(self, p):
        """
        unary_expression : postfix_expression
          | INCR unary_expression
          | DECR unary_expression
          | unary_operator multiplicative_expression
          | SIZEOF unary_expression
          | SIZEOF LPAREN type_name RPAREN
        """
        p[0] = self._compose(p)

    def p_unary_operator(self, p):
        """
        unary_operator : BAND
          | TIMES
          | PLUS
          | MINUS
          | BNOT
          | NOT
        """
        p[0] = self._compose(p)

    def p_cast_expression(self, p):
        """
        multiplicative_expression : unary_expression
          | LPAREN type_name RPAREN multiplicative_expression
        """
        p[0] = self._compose(p)

    def p_multiplicative_expression(self, p):
        """
        multiplicative_expression : multiplicative_expression TIMES multiplicative_expression
          | multiplicative_expression DIVIDE multiplicative_expression
          | multiplicative_expression MOD multiplicative_expression
          | multiplicative_expression PLUS multiplicative_expression
          | multiplicative_expression MINUS multiplicative_expression
          | multiplicative_expression LSHIFT multiplicative_expression
          | multiplicative_expression RSHIFT multiplicative_expression
          | multiplicative_expression LT multiplicative_expression
          | multiplicative_expression GT multiplicative_expression
          | multiplicative_expression LTEQ multiplicative_expression
          | multiplicative_expression GTEQ multiplicative_expression
          | multiplicative_expression EQ multiplicative_expression
          | multiplicative_expression NEQ multiplicative_expression
          | multiplicative_expression BAND multiplicative_expression
          | multiplicative_expression BXOR multiplicative_expression
          | multiplicative_expression BOR multiplicative_expression
          | multiplicative_expression AND multiplicative_expression
        """
        p[0] = self._compose(p)

    def p_logical_or_expression(self, p):
        """
        logical_or_expression : multiplicative_expression
          | logical_or_expression OR multiplicative_expression
        """
        p[0] = self._compose(p)

    def p_conditional_expression(self, p):
        """
        conditional_expression : logical_or_expression
          | logical_or_expression QUEST expression COLON conditional_expression %prec CONDITIONAL
        """
        p[0] = self._compose(p)

    def p_assignment_expression(self, p):
        """
        assignment_expression : conditional_expression
          | unary_expression assignment_operator assignment_expression
        """
        p[0] = self._compose(p)

    def p_assignment_operator(self, p):
        """
        assignment_operator : ASSIGN
          | TIMES_ASSIGN
          | DIVIDE_ASSIGN
          | MOD_ASSIGN
          | PLUS_ASSIGN
          | MINUS_ASSIGN
          | LSHIFT_ASSIGN
          | RSHIFT_ASSIGN
          | BAND_ASSIGN
          | BXOR_ASSIGN
          | BOR_ASSIGN
        """
        p[0] = self._compose(p)

    def p_expression(self, p):
        """
        expression : assignment_expression
          | expression COMMA assignment_expression
        """
        p[0] = self._compose(p)

    def p_constant_expression(self, p):
        """
        constant_expression : conditional_expression
        """
        p[0] = self._compose(p)

    def p_declaration(self, p):
        """
        declaration : declaration_specifiers SEMICOL
          | declaration_specifiers init_declarator_list SEMICOL
        """
        p[0] = self._compose(p)

        if 'typedef' in p[0]:
            type_name = None

            levels_in =  0
            for token in p[0][-2::-1]:
                if token == ')':
                    pass
                elif token == ']':
                    levels_in += 1
                elif token == '[':
                    levels_in -= 1
                elif levels_in == 0:
                    type_name = token
                    break

            assert type_name is not None
            print 'Adding %s...' % type_name
            self._type_definitions += [type_name]

    def p_declaration_specifiers(self, p):
        """
        declaration_specifiers : storage_class_specifier
          | storage_class_specifier declaration_specifiers
          | type_specifier
          | type_specifier declaration_specifiers
          | type_qualifier
          | type_qualifier declaration_specifiers
        """
        p[0] = self._compose(p)

    def p_init_declarator_list(self, p):
        """
        init_declarator_list : init_declarator
          | init_declarator_list COMMA init_declarator
        """
        p[0] = self._compose(p)

    def p_init_declarator(self, p):
        """
        init_declarator : declarator
          | declarator ASSIGN initializer
        """
        p[0] = self._compose(p)

    def p_storage_class_specifier(self, p):
        """
        storage_class_specifier : TYPEDEF
          | EXTERN
          | STATIC
          | AUTO
          | REGISTER
        """
        p[0] = self._compose(p)

    def p_type_specifier(self, p):
        """
        type_specifier : VOID
          | CHAR
          | SHORT
          | INT
          | LONG
          | FLOAT
          | DOUBLE
          | SIGNED
          | UNSIGNED
          | struct_or_union_specifier
          | enum_specifier
          | TYPE_NAME
          | _BOOL
          | _COMPLEX
        """
        p[0] = self._compose(p)

    def p_struct_or_union_specifier(self, p):
        """
        struct_or_union_specifier : struct_or_union IDENT LCURLY struct_declaration_list RCURLY
          | struct_or_union LCURLY struct_declaration_list RCURLY
          | struct_or_union IDENT
        """
        p[0] = self._compose(p)

    def p_struct_or_union(self, p):
        """
        struct_or_union : STRUCT
          | UNION
        """
        p[0] = self._compose(p)

    def p_struct_declaration_list(self, p):
        """
        struct_declaration_list : struct_declaration
          | struct_declaration_list struct_declaration
        """
        p[0] = self._compose(p)

    def p_struct_declaration(self, p):
        """
        struct_declaration : specifier_qualifier_list struct_declarator_list SEMICOL
        """
        p[0] = self._compose(p)

    def p_specifier_qualifier_list(self, p):
        """
        specifier_qualifier_list : type_specifier specifier_qualifier_list
          | type_specifier
          | type_qualifier specifier_qualifier_list
          | type_qualifier
        """
        p[0] = self._compose(p)

    def p_struct_declarator_list(self, p):
        """
        struct_declarator_list : struct_declarator
          | struct_declarator_list COMMA struct_declarator
        """
        p[0] = self._compose(p)

    def p_struct_declarator(self, p):
        """
        struct_declarator : declarator
          | COLON constant_expression
          | declarator COLON constant_expression
        """
        p[0] = self._compose(p)

    def p_enum_specifier(self, p):
        """
        enum_specifier : ENUM LCURLY enumerator_list RCURLY
          | ENUM IDENT LCURLY enumerator_list RCURLY
          | ENUM IDENT
        """
        p[0] = self._compose(p)

    def p_enumerator_list(self, p):
        """
        enumerator_list : enumerator
          | enumerator_list COMMA enumerator
        """
        p[0] = self._compose(p)

    def p_enumerator(self, p):
        """
        enumerator : IDENT
          | IDENT ASSIGN constant_expression
        """
        p[0] = self._compose(p)

    def p_type_qualifier(self, p):
        """
        type_qualifier : CONST
          | VOLATILE
        """
        p[0] = self._compose(p)

    def p_declarator(self, p):
        """
        declarator : pointer direct_declarator
          | direct_declarator
        """
        p[0] = self._compose(p)

    def p_direct_declarator(self, p):
        """
        direct_declarator : IDENT
          | LPAREN declarator RPAREN
          | direct_declarator LBOX constant_expression RBOX
          | direct_declarator LBOX RBOX
          | direct_declarator LPAREN parameter_type_list RPAREN
          | direct_declarator LPAREN identifier_list RPAREN
          | direct_declarator LPAREN RPAREN
        """
        p[0] = self._compose(p)

    def p_pointer(self, p):
        """
        pointer : TIMES
          | TIMES type_qualifier_list
          | TIMES pointer
          | TIMES type_qualifier_list pointer
        """
        p[0] = self._compose(p)

    def p_type_qualifier_list(self, p):
        """
        type_qualifier_list : type_qualifier
          | type_qualifier_list type_qualifier
        """
        p[0] = self._compose(p)

    def p_parameter_type_list(self, p):
        """
        parameter_type_list : parameter_list
          | parameter_list COMMA ELLIPSIS
        """
        p[0] = self._compose(p)

    def p_parameter_list(self, p):
        """
        parameter_list : parameter_declaration
          | parameter_list COMMA parameter_declaration
        """
        p[0] = self._compose(p)

    def p_parameter_declaration(self, p):
        """
        parameter_declaration : declaration_specifiers declarator
          | declaration_specifiers abstract_declarator
          | declaration_specifiers
        """
        p[0] = self._compose(p)

    def p_identifier_list(self, p):
        """
        identifier_list : IDENT
          | identifier_list COMMA IDENT
        """
        p[0] = self._compose(p)

    def p_type_name(self, p):
        """
        type_name : specifier_qualifier_list
          | specifier_qualifier_list abstract_declarator
        """
        p[0] = self._compose(p)

    def p_abstract_declarator(self, p):
        """
        abstract_declarator : pointer
          | direct_abstract_declarator
          | pointer direct_abstract_declarator
        """
        p[0] = self._compose(p)

    def p_direct_abstract_declarator(self, p):
        """
        direct_abstract_declarator : LPAREN abstract_declarator RPAREN
          | LBOX RBOX
          | LBOX constant_expression RBOX
          | direct_abstract_declarator LBOX RBOX
          | direct_abstract_declarator LBOX constant_expression RBOX
          | LPAREN RPAREN
          | LPAREN parameter_type_list RPAREN
          | direct_abstract_declarator LPAREN RPAREN
          | direct_abstract_declarator LPAREN parameter_type_list RPAREN
        """
        p[0] = self._compose(p)

    def p_initializer(self, p):
        """
        initializer : assignment_expression
          | LCURLY initializer_list RCURLY
          | LCURLY initializer_list COMMA RCURLY
        """
        p[0] = self._compose(p)

    def p_initializer_list(self, p):
        """
        initializer_list : initializer
          | initializer_list COMMA initializer
        """
        p[0] = self._compose(p)

    def p_statement(self, p):
        """
        statement : labeled_statement
          | compound_statement
          | expression_statement
          | selection_statement
          | iteration_statement
          | jump_statement
        """
        p[0] = self._compose(p)

    def p_labeled_statement(self, p):
        """
        labeled_statement : IDENT COLON statement
          | CASE constant_expression COLON statement
          | DEFAULT COLON statement
        """
        p[0] = self._compose(p)

    def p_compound_statement(self, p):
        """
        compound_statement : LCURLY RCURLY
          | LCURLY statement_list RCURLY
          | LCURLY declaration_list RCURLY
          | LCURLY declaration_list statement_list RCURLY
        """
        p[0] = self._compose(p)

    def p_declaration_list(self, p):
        """
        declaration_list : declaration
          | declaration_list declaration
        """
        p[0] = self._compose(p)

    def p_statement_list(self, p):
        """
        statement_list : statement
          | statement_list statement
        """
        p[0] = self._compose(p)

    def p_expression_statement(self, p):
        """
        expression_statement : SEMICOL
          | expression SEMICOL
        """
        p[0] = self._compose(p)

    def p_selection_statement(self, p):
        """
        selection_statement : IF LPAREN expression RPAREN statement
          | IF LPAREN expression RPAREN statement ELSE statement
          | SWITCH LPAREN expression RPAREN statement
        """
        p[0] = self._compose(p)

    def p_iteration_statement(self, p):
        """
        iteration_statement : WHILE LPAREN expression RPAREN statement
          | DO statement WHILE LPAREN expression RPAREN SEMICOL
          | FOR LPAREN expression_statement expression_statement RPAREN statement
          | FOR LPAREN expression_statement expression_statement expression RPAREN statement
        """
        p[0] = self._compose(p)

    def p_jump_statement(self, p):
        """
        jump_statement : GOTO IDENT SEMICOL
          | CONTINUE SEMICOL
          | BREAK SEMICOL
          | RETURN SEMICOL
          | RETURN expression SEMICOL
        """
        p[0] = self._compose(p)

    def p_error(self, p):
        print "Syntax error at '%s' on line %d" % (p.value, self.lineno)
        sys.exit(1)

if __name__ == '__main__':
    # Command line arguments
    argparser = argparse.ArgumentParser()
    argparser.add_argument('--interactive', action='store_true', help='Interactive mode')
    args = argparser.parse_args()

    # Build the parser
    calc = SimpleCParser()
    calc.run(args)
