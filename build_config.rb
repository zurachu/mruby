MRuby::Build.new do |conf|

  # Gets set by the VS command prompts.
  if ENV['VisualStudioVersion'] || ENV['VSINSTALLDIR']
    toolchain :visualcpp
  else
    toolchain :gcc
  end

  enable_debug

  # include the default GEMs
  conf.gembox 'default'

end

# Cross Compiling configuration for P/ECE
# Expect setting path to development environment, ex) C:\usr\PIECE
MRuby::CrossBuild.new("piece") do |conf|
  conf.cc do |cc|
    cc.command = "ruby examples/targets/PIECE/pcc33_wrapper.rb"
    cc.flags = "-Wall -g -O2"
    cc.include_paths << ["examples/targets/PIECE/include"]
    cc.defines = %w(PIECE MRB_32BIT MRB_METHOD_TABLE_INLINE MRB_DISABLE_STDIO MRB_WITHOUT_FLOAT)

    #configuration for low memory environment
    cc.defines << %w(MRB_HEAP_PAGE_SIZE=64)
    cc.defines << %w(KHASH_DEFAULT_SIZE=8)
    cc.defines << %w(MRB_STR_BUF_MIN_SIZE=20)
    cc.defines << %w(MRB_GC_STRESS)
    
    cc.option_include_path = "-I%s"
    cc.option_define = "-D%s"
    cc.compile_options = "%{flags} -o %{outfile} -c %{infile}"
  end

  conf.archiver do |archiver|
    archiver.command = "lib33"
    archiver.archive_options = "%{outfile} %{objs}"
  end

  conf.exts do |exts|
    exts.library = ".lib"
  end

  #no executables
  conf.bins = []

  #do not build executable test
  conf.build_mrbtest_lib_only

  #disable C++ exception
  conf.disable_cxx_exception

end
