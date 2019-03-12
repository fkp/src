#!/usr/bin/env ruby

require 'pathname'
require 'taglib'
#require 'sqlite3'
require 'pg'

class FileFinder

  def FindFiles(path, &block)
    path.children.collect do |child|
      if child.directory?
        FindFiles(child, &block)
      elsif child.file?
        yield child
      end
    end
  end

end

class MusicInserter
  @@rowsInserted=0
  COMMIT_AFTER=1000

  def DelimitValue(value)
    return "'" + value.gsub("'","''") + "'"
  end

  def InsertRow(file)
    begin
      TagLib::FileRef.open(file.to_s()) do |fileref|
	data = fileref.tag
	sql = "INSERT INTO Music(Artist, Album, Track, Title, Filename) VALUES (" + DelimitValue(data.artist) + "," + DelimitValue(data.album) + "," + data.track.to_s() + "," + DelimitValue(data.title) + "," + DelimitValue(file.to_s()) + ")"
        #puts sql
        #print "\r#{@@rowsInserted.to_s()}"
        STDOUT.flush
        puts file.to_s()
        ExecuteSql(sql)

        @@rowsInserted+=1
      end
      
        #if(@@rowsInserted % COMMIT_AFTER == 0)
        #  puts "Committing after " + @@rowsInserted.to_s() + " rows"
  	  #  ExecuteSql("COMMIT")
  	  #end
    rescue Exception => e 
      puts "Exception occured"
      puts e
    ensure
    #  sqliteDatabase.close if sqliteDatabase
    end
  end

end

class MusicInserterSqlite < MusicInserter
  CREATE_TABLE_SQL="CREATE TABLE Music(Id INTEGER PRIMARY KEY, Artist TEXT, Album TEXT, Track INT, Title TEXT, Filename TEXT)"
  @@sqliteFile="data.sqlite"
  @@sqliteDatabase=nil

  def Init()

    begin
      if (File.exist?(@@sqliteFile))
        File.delete(@@sqliteFile)
      end

      @@sqliteDatabase = SQLite3::Database.new(@@sqliteFile)
      @@sqliteDatabase.execute(CREATE_TABLE_SQL)

    rescue SQLite3::Exception => e 
      puts "Exception occured"
      puts e
    ensure
    #  sqliteDatabase.close if sqliteDatabase
    end
  end

  def ExecuteSql(sql)
    @@sqliteDatabase.execute(sql)
  end
  
  def Close()
    @@sqliteDatabase.close if @@sqliteDatabase
  end

end

class MusicInserterPostgres < MusicInserter
  #@@pgIP="localhost"
  #@@pgPort="5432"
  @@pgDbName="scratch"
  #@@pgUsername="fraser"
  CREATE_TABLE_SQL="CREATE TABLE Music(Artist TEXT, Album TEXT, Track INTEGER, Title TEXT, Filename TEXT)"
  DROP_TABLE_SQL="DROP TABLE Music"
  
  @@pgConnection=nil

  def Init()

    begin
      #@@pgConnection = PG::Connection.new("localhost", 5432, '','',"scratch","fraser","fraser")
      @@pgConnection = PGconn.connect(:dbname => 'scratch')
      @@pgConnection.exec(DROP_TABLE_SQL)
      @@pgConnection.exec(CREATE_TABLE_SQL)
    end
  end

  def ExecuteSql(sql)
    @@pgConnection.exec(sql)
  end
  
  def Close()
    @@pgConnection.close if @@pgConnection
  end


end

inserter = MusicInserterPostgres.new
inserter.Init()

files = FileFinder.new

files.FindFiles(Pathname.new(ARGV.first)) {|file| inserter.InsertRow(file) }
inserter.Close()

puts "Done!"


