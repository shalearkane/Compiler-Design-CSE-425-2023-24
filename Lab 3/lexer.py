import ply.lex as lex

tokens = [
  'COMMENT',
  'MAJOR_KEYWORD',
  'MINOR_KEYWORD',
  'ELLIPSIS',
  'PUNCTUATION',
  'FLOAT',
  'INTEGER',
  'LOWERCASE_IDENTIFIER',
  'UPPERCASE_IDENTIFIER',
  'ANONYMOUS_IDENTIFIER',
  'OPERATORS',
  'CHARACTER',
  'STRING',
]

def t_COMMENT(t):
  r'(%.*)|(/\*(.*(\n)*)*\*/)'
  pass

def t_newline(t):
  r'\n+'
  t.lexer.lineno += len(t.value)

def t_MAJOR_KEYWORD(t):
  r'\b(class|clauses|constants|constructors|delegate|domains|end|facts|goal|implement|inherits|interface|monitor|namespace|open|predicates|properties|resolve|supports|where)\b'
  return t

def t_MINOR_KEYWORD(t):
  r'\b(align|and|anyflow|as|bitsize|catch|determ|digits|div|do|else|elseif|erroneous|externally|failure|finally|foreach|from|guard|if|in|language|mod multi|nondeterm|otherwise|or|orelse|procedure|quot|rem|single|then|to|try)\b'
  return t

def t_ELLIPSIS(t):
  r'\.\.\.'
  return t

def t_FLOAT(t):
  r'[-+]?[0-9]+(\.([0-9]+)?([eE][-+]?[0-9]+)?|[eE][-+]?[0-9]+)' 
  t.value = float(t.value)
  return t

def t_PUNCTUATION(t):
  r':-|::|[;!,.#\[\]\|(){}:]'
  return t

def t_INTEGER(t):
  r'[-]?((0o[0-7]+)|(0x[0-9a-fA-F]+)|([0-9]+))'
  return t

def t_LOWERCASE_IDENTIFIER(t):
  r"\b[a-z][a-zA-Z0-9_]*\b"
  return t

def t_UPPERCASE_IDENTIFIER(t):
  r"\b[_A-Z][a-zA-Z0-9_]*\b"
  return t

def t_ANONYMOUS_IDENTIFIER(t):
  r'_'
  return t

def t_OPERATORS(t):
  r"~~|\+\+|\*\*|\^\^|>>|<<|<>|><|<=|>=|:=|==|\^|/|\*|\+|-|=|<|>"
  return t

def t_CHARACTER(t):
  r"'.'"
  t.value = t.value[1:2]
  return t

def t_STRING(t):
  r"'.+'|\".*\""
  t.value = t.value[1:-1]
  return t

def t_error(t):
  print(f"Illegal character '{t.value[0]}' at line {t.lexer.lineno}")
  t.lexer.skip(1)

t_ignore = ' \t'

lexer = lex.lex(optimize=1)

out = open("output.txt", "w")
f = open("input.txt")
text = f.read()

lexer.input(text)

# Tokenize
while True:
    tok = lexer.token()
    if not tok: 
        break      # No more input
    out.write(str(tok))
    out.write('\n')

f.close()
out.close()