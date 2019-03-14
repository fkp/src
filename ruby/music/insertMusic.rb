#!/usr/bin/env ruby

require 'pathname'
require 'taglib'
require 'pg'

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

  def Init(tableName)

    begin
      #@@pgConnection = PG::Connection.new("localhost", 5432, '','',"scratch","fraser","fraser")
      @@pgConnection = PGconn.connect(:dbname => 'scratch')
      
      # Might fail
      begin 
        @@pgConnection.exec("DROP TABLE "+tableName)
      rescue
	puts '  Table drop failed'
      end

      @@pgConnection.exec("CREATE TABLE "+tableName+"(Artist TEXT, Album TEXT, Track INTEGER, Title TEXT, Filename TEXT)")

      # Setup the connection to insert rows
      @@pgConnection.prepare('insert1', 'INSERT INTO '+tableName+'(Artist, Album, Track, Title, Filename) VALUES ($1,$2,$3,$4,$5)')
    end
  end

  def InsertMusicRow(file)
      begin
	TagLib::FileRef.open(file.to_s()) do |fileref|
	  data = fileref.tag
          puts file.to_s()
	  @@pgConnection.exec_prepared('insert1', [data.artist,data.album,data.track,data.title,file.to_s()])
        rescue
          puts '  Failed for ' + file.to_s()
        end
      end
  end
  
  def Close()
    @@pgConnection.close if @@pgConnection
  end


end

inserter = MusicInserterPostgres.new
inserter.Init(ARGV[0])

files = FileFinder.new

files.FindFiles(Pathname.new(ARGV[1])) {|file| inserter.InsertMusicRow(file) }
inserter.Close()

puts "Done!"

