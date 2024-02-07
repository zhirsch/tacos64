# -*- mode: python -*-
# Common definitions for all SConstruct files

import os
import SCons

Import('opts')

env = Environment(options=opts)

if env["COLOR"]:
    env.Append(CFLAGS=["-fdiagnostics-color"])

# Create a builder for creating an ISO.
env.Append(
    BUILDERS = {'ISO': Builder(action=Action('$ISOCOM', '$ISOCOMSTR'))},
    ISOCOM = 'grub2-mkrescue -o $TARGET $ROOT > /dev/null 2>&1',
    ISOCOMSTR = '$ISOCOM',
)

# Create a builder for sed
env.Append(
    BUILDERS = {'Sed': Builder(action=Action('$SEDCOM', '$SEDCOMSTR'))},
    SEDCOM = 'sed -e"$PATTERN" < $SOURCE > $TARGET',
    SEDCOMSTR = '$SEDCOM',
)

if not env['V']:
    env['CCCOMSTR']    = '  CC                $TARGET'
    env['LINKCOMSTR']  = '  LD                $TARGET'
    env['ASPPCOMSTR']  = '  ASPP              $TARGET'
    env['ISOCOMSTR']   = '  ISO               $TARGET'
    env['SEDCOMSTR']   = '  SED               $TARGET'
    env['COPYSTR']     = '  COPY              $TARGET'
    SCons.Script.Copy.strfunc = lambda d, s: env.subst('$COPYSTR', 1, target=d)

Return('env')
