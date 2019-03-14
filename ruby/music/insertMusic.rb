#!/usr/bin/env ruby

require 'pathname'
require 'taglib'
require 'pg'
require 'optparse'

class FileFinder
  def FindFiles(path, &block)
    if path.file?
      yield path
    else
      path.children.collect do |child|
        if child.directory?
          FindFiles(child, &block)
        elsif child.file?
          yield child
	end
      end
    end
  end
end

class MusicInserterPostgres
  #@@pgIP="localhost"
  #@@pgPort="5432"
  @@pgDbName="scratch"
  #@@pgUsername="fraser"
  
  @@pgConnection=nil
  @@quiet

  def Init(tableName, quiet)

    @@quiet = quiet

    begin
      #@@pgConnection = PG::Connection.new("localhost", 5432, '','',"scratch","fraser","fraser")
      @@pgConnection = PGconn.connect(:dbname => 'scratch')
      
      # Might fail
      begin 
        @@pgConnection.exec("DROP TABLE "+tableName)
      rescue
	puts '  Table drop failed'
      end

      @@pgConnection.exec("CREATE TABLE "+tableName+"(artist TEXT, album TEXT, track INTEGER, title TEXT, filename TEXT, notes TEXT)")

      # Setup the connection to insert rows
      @@pgConnection.prepare('insert1', 'INSERT INTO '+tableName+'(artist, album, track, title, filename, notes) VALUES ($1,$2,$3,$4,$5,$6)')
    end
  end

  def InsertMusicRow(file)
      begin
	TagLib::FileRef.open(file.to_s()) do |fileref|
	  data = fileref.tag
	  if !@@quiet
            puts file.to_s()
	  end
	  @@pgConnection.exec_prepared('insert1', [data.artist,data.album,data.track,data.title,file.to_s(),''])
        rescue
          puts '  Failed for ' + file.to_s()
        end
      end
  end
  
  def Close()
    @@pgConnection.close if @@pgConnection
  end


end

quiet=false
options = {}
optparse = OptionParser.new do |opts|
  opts.banner = "Useage: insertMusic.rb [options] <tableName to insert> <file/directory to search from>"
  opts.on('-q', '--quiet', 'Don\'t show files as they are processed') do |q|
    quiet=true;
  end
end
optparse.parse!

# Check the required arguments
if ARGV.length != 2
  puts optparse
  exit(-1)
end

inserter = MusicInserterPostgres.new
inserter.Init(ARGV[0], quiet)

files = FileFinder.new

files.FindFiles(Pathname.new(ARGV[1])) {|file| inserter.InsertMusicRow(file) }
inserter.Close()

puts "Done!"

