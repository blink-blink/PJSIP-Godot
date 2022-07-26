#!python
import os, subprocess

opts = Variables([], ARGUMENTS)

# Gets the standard flags CC, CCX, etc.
env = DefaultEnvironment()

# Define our options
opts.Add(EnumVariable('target', "Compilation target", 'debug', ['d', 'debug', 'r', 'release']))
opts.Add(EnumVariable('platform', "Compilation platform", '', ['', 'windows', 'x11', 'linux', 'osx']))
opts.Add(EnumVariable('p', "Compilation target, alias for 'platform'", '', ['', 'windows', 'x11', 'linux', 'osx']))
opts.Add(BoolVariable('use_llvm', "Use the LLVM / Clang compiler", 'no'))
opts.Add(PathVariable('target_path', 'The path where the lib is installed.', 'PJSIP-Godot-test/bin/'))
opts.Add(PathVariable('target_name', 'The library name.', 'libgdexample', PathVariable.PathAccept))

# Local dependency paths, adapt them to your setup
godot_headers_path = "../gdnative_cpp_example/godot-cpp/godot-headers/"
cpp_bindings_path = "../gdnative_cpp_example/godot-cpp/"
cpp_library = "libgodot-cpp"

# only support 64 at this time..
bits = 64

# Updates the environment with the option variables.
opts.Update(env)

# Process some arguments
if env['use_llvm']:
    env['CC'] = 'clang'
    env['CXX'] = 'clang++'

if env['p'] != '':
    env['platform'] = env['p']

if env['platform'] == '':
    print("No valid target platform selected.")
    quit();

# Check our platform specifics
if env['platform'] == "osx":
    env['target_path'] += 'osx/'
    cpp_library += '.osx'
    if env['target'] in ('debug', 'd'):
        env.Append(CCFLAGS = ['-g','-O2', '-arch', 'x86_64', '-std=c++17'])
        env.Append(LINKFLAGS = ['-arch', 'x86_64'])
    else:
        env.Append(CCFLAGS = ['-g','-O3', '-arch', 'x86_64', '-std=c++17'])
        env.Append(LINKFLAGS = ['-arch', 'x86_64'])

elif env['platform'] in ('x11', 'linux'):
    env['target_path'] += 'x11/'
    cpp_library += '.linux'
    if env['target'] in ('debug', 'd'):
        env.Append(CCFLAGS = ['-fPIC', '-g3','-Og', '-std=c++17'])
    else:
        env.Append(CCFLAGS = ['-fPIC', '-g','-O3', '-std=c++17'])

elif env['platform'] == "windows":
    env['target_path'] += 'win64/'
    cpp_library += '.windows'
    # This makes sure to keep the session environment variables on windows,
    # that way you can run scons in a vs 2017 prompt and it will find all the required tools
    env.Append(ENV = os.environ)

    env.Append(CCFLAGS = ['-DWIN32', '-D_WIN32', '-D_WINDOWS', '-W3', '-GR', '-D_CRT_SECURE_NO_WARNINGS'])
    if env['target'] in ('debug', 'd'):
        env.Append(CCFLAGS = ['-EHsc', '-D_DEBUG', '-MDd'])
    else:
        env.Append(CCFLAGS = ['-O2', '-EHsc', '-DNDEBUG', '-MD'])

if env['target'] in ('debug', 'd'):
    cpp_library += '.debug'
else:
    cpp_library += '.release'

cpp_library += '.' + str(bits)

# make sure our binding library is properly includes
env.Append(CPPPATH=['.', godot_headers_path, cpp_bindings_path + 'include/', cpp_bindings_path + 'include/core/', cpp_bindings_path + 'include/gen/'])
env.Append(LIBPATH=[cpp_bindings_path + 'bin/'])
env.Append(LIBS=[cpp_library])


PJBASE = 'pjproject-2.12.1/'
TARGET_NAME = 'x86_64-unknown-linux-gnu'
#pjsua2 library
#env.Append(CPPPATH=[PJBASE + 'pjlib/include'])
#env.Append(CPPPATH=[PJBASE + 'pjlib-util/include'])
#env.Append(CPPPATH=[PJBASE + 'pjnath/include'])
#env.Append(CPPPATH=[PJBASE + 'pjmedia/include'])
#env.Append(CPPPATH=[PJBASE + 'pjsip/include'])
#env.Append(LIBPATH=[PJBASE+'/pjlib/lib/'])
#env.Append(LIBPATH=[PJBASE+'/pjlib-util/lib/'])
#env.Append(LIBPATH=[PJBASE+'/pjmedia/lib/'])
#env.Append(LIBPATH=[PJBASE+'/pjsip/lib/'])
if env['platform'] in ('x11', 'linux'):
    env.Append(LIBPATH=[PJBASE+'/usr/local/lib/'])
    env.Append(CPPPATH=['/usr/local/include'])
    env.Append(LIBS=['pjsua2'])
    env.Append(LIBS=['stdc++'])
    env.Append(LIBS=['pjsua'])
    env.Append(LIBS=['pjsip-ua'])
    env.Append(LIBS=['pjsip-simple'])
    env.Append(LIBS=['pjsip'])
    env.Append(LIBS=['pjmedia-codec'])
    env.Append(LIBS=['pjmedia'])
    env.Append(LIBS=['pjmedia-videodev'])
    env.Append(LIBS=['pjmedia-audiodev'])
    env.Append(LIBS=['pjnath'])
    env.Append(LIBS=['pjlib-util'])
    env.Append(LIBS=['pj'])
elif env['platform'] == "windows":
    env.Append(CPPPATH=['pjproject-2.12.1\pjlib\include'])
    env.Append(CPPPATH=['pjproject-2.12.1\pjlib-util\include'])
    env.Append(CPPPATH=['pjproject-2.12.1\pjnath\include'])
    env.Append(CPPPATH=['pjproject-2.12.1\pjmedia\include'])
    env.Append(CPPPATH=['pjproject-2.12.1\pjsip\include'])
    env.Append(LIBPATH=['.\PJSIP-GodotLib\lib/'])
    env.Append(LIBS=['libpjproject-x86_64-x64-vc14-Release'])
    env.Append(LIBS=['Iphlpapi'])
    env.Append(LIBS=['dsound'])	
    env.Append(LIBS=['dxguid'])
    env.Append(LIBS=['netapi32'])
    env.Append(LIBS=['mswsock'])
    env.Append(LIBS=['ws2_32'])
    env.Append(LIBS=['odbc32'])
    env.Append(LIBS=['odbccp32'])
    env.Append(LIBS=['ole32'])
    env.Append(LIBS=['user32'])
    env.Append(LIBS=['gdi32'])
    env.Append(LIBS=['advapi32'])

# tweak this if you want to use different folders, or more folders, to store your source code in.
env.Append(CPPPATH=['PJSIP-GodotLib/'])
sources = Glob('PJSIP-GodotLib/*.cpp')

library = env.SharedLibrary(target=env['target_path'] + env['target_name'] , source=sources)

Default(library)

# Generates help for the -h scons option.
Help(opts.GenerateHelpText(env))
