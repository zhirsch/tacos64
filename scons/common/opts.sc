# -*- mode: python -*-
# Create the allowed options

opts = Variables(File('#custom.py').abspath)
opts.AddVariables(
    BoolVariable(
        'V',
        help='echo the build commands',
        default=False,
    ),
    ListVariable(
        'T',
        help='the build variant type',
        default='debug,release',
        names=['debug', 'release'],
    ),
    BoolVariable(
        'COLOR',
        help='show compiler output in color',
        default=True,
    ),
    PathVariable(
        'CC',
        help='the c compiler to use',
        default='x86_64-elf-gcc',
        validator=PathVariable.PathAccept,
    ),
)

Return('opts')
