import re


class Lexer:
    def __init__(self):
        # Define the token types
        self.token_types = [
            ("MULTI_LINE_COMMENT", r"/\*.*(\n)*.*\*/"),
            ("SINGLE_LINE_COMMENT", r"%.*"),
            ("WHITESPACE", r"\s+"),
            ("NEW_LINE", r"\n"),
            (
                "MAJOR_KEYWORD",
                r"\b(class|clauses|constants|constructors|delegate|domains|end|facts|goal|implement|inherits|interface|monitor|namespace|open|predicates|properties|resolve|supports|where)\b",
            ),
            (
                "MINOR_KEYWORD",
                r"\b(align|and|anyflow|as|bitsize|catch|determ|digits|div|do|else|elseif|erroneous|externally|failure|finally|foreach|from|guard|if|in|language|mod multi|nondeterm|otherwise|or|orelse|procedure|quot|rem|single|then|to|try)\b",
            ),
            ("ELLIPSIS", r"\.\.\."),
            ("PUNCTUATION", r"\:\-|\:\:|[\;\!\,\.\#\[\]\|\(\)\{\}\:]"),
            ("FLOAT", r"-?([0-9]+\.[0-9]*|\.\d*)(e[0-9]+)?"),
            ("INTEGER", r"-?([0-9]+|0o[0-7]+|0x[0-9A-Fa-f]+)(?:\s)"),
            ("LOWERCASE_IDENTIFIER", r"\b[a-z][a-zA-Z0-9_]*\b"),
            ("UPPERCASE_IDENTIFIER", r"\b[A-Z][a-zA-Z0-9_]*\b"),
            ("UPPERCASE_IDENTIFIER", r"\b[_][a-zA-Z0-9_]+\b"),
            ("ANONYMOUS_IDENTIFIER", r"_"),
            ("OPERATORS", r"~~|\+\+|\*\*|\^\^|>>|<<|<>|><|<=|>=|:=|=="),
            ("OPERATORS", r"\^|/|\*|\+|-|=|<|>"),
            ("STRING", r'".*?"'),
            ("CHARACTER", r'\'.\'')
        ]

    def tokenize(self, input_string):
        tokens = []
        while input_string:
            match = None
            for token_type, pattern in self.token_types:
                match = re.match(pattern, input_string)
                if match:
                    value = match.group(0)
                    if token_type not in [
                        "WHITESPACE",
                        "COMMENT",
                        "NEW_LINE",
                        "SINGLE_LINE_COMMENT",
                        "MULTI_LINE_COMMENT",
                    ]:
                        tokens.append((value, token_type))
                    input_string = input_string[match.end() :]
                    break
            if not match:
                raise ValueError(f"Invalid character: {input_string[0]}")
        return tokens


def main():
    lexer = Lexer()

    out = open("output.txt", "w")
    f = open("input.txt")
    text = f.read()
    t = lexer.tokenize(text)

    out.write("Tokens: \n")
    for token in t:
        out.write(str(token))
        out.write("\n")

    f.close()
    out.close()


if __name__ == "__main__":
    main()
