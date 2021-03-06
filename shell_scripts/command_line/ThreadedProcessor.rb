#!/usr/bin/env ruby

require 'optparse'
require 'thread'

class ThreadedProcessor
  def Run(args)

    options = {}
 
    optparse = OptionParser.new do|opts|
      # Set a banner, displayed at the top
      # of the help screen.
      opts.banner = "Usage: [options] file1 file2 ..."
 
      # Define the options, and what they do
      #nicer option as of Ruby 2.2.3...
      #options[:threads] = Etc.nprocessors
      options[:threads] = `cat /proc/cpuinfo | grep processor | wc -l`
      opts.on( '-t', '--threads n', 'Number of threads to use, otherwise use however many we can detect' ) do|threadsnum|
        options[:threads] = threadsnum
      end
 
    end

    begin
      # Parse the command-line. Remember there are two forms
      # of the parse method. The 'parse' method simply parses
      # ARGV, while the 'parse!' method parses ARGV and removes
      # any options found there, as well as any parameters for
      # the options. What's left is the list of files to resize.
      optparse.parse!
    rescue OptionParser::InvalidOption
      puts "Failed to parse command line"
      exit -1
    end
 
    puts "Using #{options[:threads]} threads" if options[:threads]

    # Put our stuff onto a queue 
    toProcess = Queue.new
    args.each do |value|
      toProcess.push(value)
    end

    threads = options[:threads].to_i
    puts "Starting " + threads.to_s + " threads for " + toProcess.length.to_s + " items"
    for threadNo in 1..threads
      puts "Starting thread" + threadNo.to_s
      Thread.new do
        while (!toProcess.empty?()) do
          thisVal = toProcess.pop
          DoThreadedAction(thisVal, toProcess.length)
        end
      end
    end

    while (!toProcess.empty?())
      sleep 0.5
    end

    puts "Done"
  end
end

class ThreadedFileProcessor < ThreadedProcessor

  def DoThreadedAction(argument, queued)
    regEx = "(.*)\\.([^.]*)"
    matches=argument.match regEx
    if matches == nil
      puts "skipping " + argument
    else
      puts ProgressString(queued) + ": " + argument
      DoThreadedFileAction(argument,matches[1],matches[2])
    end
  end

  def DoThreadedFileAction(filename, name, extension)
    puts ProgressString(queued) + "I have file: " + filename + " and will now sleep"
    sleep 0.5
    return
  end

  def ProgressString(queued)
    return "[q" + queued.to_s + "]"
  end
end

class ThreadedFileProcessorImageMagik < ThreadedFileProcessor

  def DoThreadedFileAction(filename, name, extension)
    `convert #{GetArgs()} \"#{filename}\" \"#{name}_#{GetSuffix()}.#{extension}\"`
    targetFile = name + "_" + GetSuffix() + "." + extension

    if (!File.exist?(targetFile))
      `convert #{GetArgs()} \"#{filename}\" \"#{targetFile}\" `
    else
      puts "Skipping file " + targetFile + " as already exists."
    end
  end

end
