# Define the project name, version and compilation options
project('nanoRenderer', 'cpp',
    version : '1.0.0',
    default_options : [
        'warning_level=3',      # Enable all warnings
        'cpp_std=c++17', 
        'optimization=3',       # Maximum optimization
        'b_lto=true'            # Link-Time Optimizations
    ]
)

# Tell Meson where the headers live
include_dir = include_directories('include')

# Add dependencies
openmp_dep = dependency('openmp', required : false) # OpenMP 
if openmp_dep.found()
    message('OpenMP found! Parallel mode enabled.')
else
    message('OpenMP NOT found. Compiling in serial mode.')
endif


# Create a list to store our source files
sources = []

# Move into subdirectories to collect source files
subdir('src/tgaimage')
subdir('src/model')
subdir('src/wireframe')
subdir('src/line')
subdir('src/triangle')

# Finally, add the main entry point
sources += files('src/main.cpp')

# Build the executable
executable('nanoRenderer', 
    sources : sources,
    include_directories : include_dir,
    dependencies : openmp_dep,
    install : true
)
