#!/usr/bin/env python3
"""Format HolyC with TempleOS naming and compact expression spacing.

clang-format supplies structural layout. The token pass changes only horizontal
whitespace and verifies that compacting it cannot merge language tokens.
"""
import argparse
from pathlib import Path
import re
import shutil
import subprocess

HERE = Path(__file__).resolve().parent.parent
INDENT_WIDTH = 2
MACRO_HEAD = re.compile(r'^[ \t]*#[ \t]*define[ \t]+[A-Za-z_$][A-Za-z_0-9$]*', re.M)
TOKEN = re.compile(
    r'(?P<comment>//[^\r\n]*|/\*[\s\S]*?\*/)'
    r'|(?P<string>"(?:\\[\s\S]|[^"\\])*"|\'(?:\\[\s\S]|[^\'\\])*\')'
    r'|(?P<word>[A-Za-z_$][A-Za-z_0-9$]*)'
    r'|(?P<number>0[xX][0-9a-fA-F]+|[0-9]+(?:\.[0-9]*)?(?:[eE][+-]?[0-9]+)?)'
    r'|(?P<operator>\.\.\.|>>=|<<=|->|\+\+|--|&&|\|\||==|!=|<=|>=|<<|>>|'
    r'[+*/%&|^=-]=|[^\s])'
)
COMPACT_OPERATORS = frozenset((
    '+', '-', '*', '/', '%', '&', '|', '^', '=', '!', '<', '>', '~',
    '&&', '||', '==', '!=', '<=', '>=', '<<', '>>', '+=', '-=', '*=', '/=',
    '%=', '&=', '|=', '^=', '<<=', '>>=', '->', ',',
))
# These legacy struct names predate word separators. Preserve their words when
# converting the shared C spelling instead of treating each as one long word.
CLASS_WORDS = {
    'CARSTATE': 'CarState',
    'GAMEINFO': 'GameInfo',
    'GAMESTATE': 'GameState',
    'MEMCHUNK': 'MemChunk',
    'TRACKOBJECT': 'TrackObject',
    'TRANSFORMEDSHAPE3D': 'TransformedShape3D',
    'TRKOBJINFO': 'TrkObjInfo',
    'VECTORLONG': 'VectorLong',
}


def snake_case(name):
    name = re.sub(r'([A-Z]+)([A-Z][a-z])', r'\1_\2', name)
    return re.sub(r'([a-z0-9])([A-Z])', r'\1_\2', name).lower()


def pascal_case(name):
    return ''.join(part[:1].upper() + part[1:] for part in snake_case(name).split('_'))


def class_name(name):
    if re.match(r'C[A-Z][a-zA-Z0-9]*$', name) and not name.isupper():
        return name
    return 'C' + ''.join(CLASS_WORDS.get(part, pascal_case(part)) for part in name.split('_'))


def function_name(name):
    if re.match(r'[A-Z][A-Za-z0-9]*$', name):
        return name
    if name.startswith('rs_'):
        return pascal_case(name)
    # Retain distinctions between private and public C compatibility routines.
    leading = len(name) - len(name.lstrip('_'))
    prefix = 'Rs' + ('Internal' + str(leading) if leading else '')
    return prefix + pascal_case(name)


def rename_identifiers(text, mapping):
    return TOKEN.sub(lambda match: mapping.get(match[0], match[0])
                     if match.lastgroup == 'word' else match[0], text)


def compact_spacing(text):
    result = []
    previous = None
    end = 0
    macro_names = {match.end() for match in MACRO_HEAD.finditer(text)}
    for token in TOKEN.finditer(text):
        gap = text[end:token.start()]
        # Parenthesized assignments can make clang-format align continuation
        # lines on an odd column even with bracket/operand alignment disabled.
        # Keep code indentation in two-column steps; literal/comment tokens
        # retain their own internal whitespace.
        if end == 0 or '\n' in gap:
            gap = re.sub(r'(^|\n)( +)$',
                         lambda match: match[1] + ' ' * (len(match[2]) // INDENT_WIDTH * INDENT_WIDTH),
                         gap)
        if previous and gap and not gap.strip() and '\n' not in gap and '\r' not in gap:
            left, right = previous[0], token[0]
            compact = left in COMPACT_OPERATORS or right in COMPACT_OPERATORS or left == ';'
            # Retain the separator before an object macro's replacement list,
            # including replacements that begin with a unary operator.
            if previous.end() in macro_names:
                compact = False
            # Keep the conventional space between a type and its pointer declarator.
            if right == '*' and (left.startswith('legacy_') or re.match(r'C[A-Z]', left)):
                compact = False
            if previous.lastgroup in ('comment', 'string') or token.lastgroup == 'comment':
                compact = False
            if (compact and left + right not in ('/*', '//') and
                    [part[0] for part in TOKEN.finditer(left + right)] == [left, right]):
                gap = ''
        result.extend((gap, token[0]))
        previous, end = token, token.end()
    result.append(text[end:])
    return ''.join(result)


def format_source(text, formatter=None, filename=None):
    formatter = formatter or shutil.which('clang-format')
    if not formatter:
        raise RuntimeError('clang-format 18.1.8 is required; install tools/REQUIREMENTS.TXT')
    # Always use the HolyC configuration, including for temporary test fixtures.
    result = subprocess.run([
        formatter, '--style=file:' + str(HERE / '.clang-format'),
        '--assume-filename=' + str(filename or HERE / 'SOURCE.HC.c'),
    ], input=text, text=True, capture_output=True, check=True).stdout
    compact = compact_spacing(result)
    original_tokens = [(token.lastgroup, token[0]) for token in TOKEN.finditer(result)]
    compact_tokens = [(token.lastgroup, token[0]) for token in TOKEN.finditer(compact)]
    if original_tokens != compact_tokens:
        raise ValueError('HolyC whitespace compaction changed language tokens')
    return compact.replace('\r\n', '\n').replace('\n', '\r\n')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--formatter', help='Path to clang-format 18.1.8')
    parser.add_argument('--check', action='store_true', help='Check without writing files')
    parser.add_argument('files', nargs='*', type=Path)
    args = parser.parse_args()
    paths = args.files or sorted(path for path in HERE.rglob('*') if path.suffix in ('.HC', '.HH'))
    mismatches = []
    for path in paths:
        original = path.read_bytes()
        formatted = format_source(original.decode(), args.formatter, path.with_suffix('.c')).encode()
        if original != formatted:
            if args.check:
                mismatches.append(str(path))
            else:
                path.write_bytes(formatted)
    if mismatches:
        raise SystemExit('HolyC formatting differs:\n' + '\n'.join(mismatches))


if __name__ == '__main__':
    main()
