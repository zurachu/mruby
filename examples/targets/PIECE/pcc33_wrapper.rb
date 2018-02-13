# pcc33 wrapper
# - add -o option for output file
# - enable to compile at sub directory

require "fileutils"
require "pathname"

in_file = nil
out_file = nil
pcc33_arg = []

while arg = ARGV.shift do
  if arg.start_with?("-")
    if arg.delete_prefix("-") == "o"
      out_file = ARGV.shift
    else
      if /^-I(?<path>.+)/ =~ arg
        arg = "-I#{File.absolute_path(path)}"
      end
      pcc33_arg << arg
    end
  else
    in_file = arg
  end
end

command = "pcc33"
unless in_file
  system(command)
  exit
end

in_file = Pathname(in_file)
command += " #{pcc33_arg.join(" ")} #{in_file.basename}"
result = nil
FileUtils.cd(in_file.dirname) { result = system(command) }

raise "Failed: #{command} [in #{File.absolute_path(in_file.dirname)}]" unless result

unless out_file.empty?
  obj_file = File.absolute_path(in_file.sub_ext(".o"))
  out_file = File.absolute_path(out_file)
  if obj_file != out_file
    FileUtils.mv(obj_file, out_file)
  end
end
