#@+leo-ver=4-thin
#@+node:gcross.20090221223144.2:@thin SConstruct
#@@language Python
env = Environment()

#@+at
# Get our configuration options:
#@-at
#@@c
opts = Variables('unit--.conf') # Change wm to the name of your app
opts.Add(PathVariable('PREFIX', 'Directory to install under', '/usr/local', PathVariable.PathIsDir))
opts.Update(env)
opts.Save('unit--.conf', env) # Save, so user doesn't have to 
                                 # specify PREFIX every time
Help(opts.GenerateHelpText(env))

#@+at
# Build the program:
#@-at
#@@c
unitmm = env.Library(target = 'lib/unit--', source = ['src/unit--.cc'], CPPPATH='include')
Default(unitmm)

#@+at
# Perform installation:
#@-at
#@@c
idir_prefix = '$PREFIX'
idir_lib    = '$PREFIX/lib'
idir_bin    = '$PREFIX/bin'
idir_inc    = '$PREFIX/include'
idir_data   = '$PREFIX/share'
Export('env idir_prefix idir_lib idir_bin idir_inc idir_data')

env.Install(idir_lib, unitmm)
env.Install(idir_inc, ['include/unit--.h'])
env.Alias('install', idir_prefix)

#@+at
# Testing:
#@-at
#@@c
test = env.Program('testing/test',[
    'testing/fake1.cpp',
    'testing/fake2.cpp',
    'testing/main.cpp',
    'testing/template.cpp',
    'testing/unit--.selftest.cpp',
    'testing/utility.cpp',
    unitmm,
])
env.Alias('test',test)
#@-node:gcross.20090221223144.2:@thin SConstruct
#@-leo
