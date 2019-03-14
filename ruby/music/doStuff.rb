#!/usr/bin/env ruby

require 'pathname'
require 'taglib'
require 'pg'
require 'optparse'

class DoThingsWithRows
  #@@pgIP="localhost"
  #@@pgPort="5432"
  @@pgDbName="scratch"
  #@@pgUsername="fraser"
  
  @@pgConnection=nil

  def Init(tableName, noteValue)

    begin
      #@@pgConnection = PG::Connection.new("localhost", 5432, '','',"scratch","fraser","fraser")
      @@pgConnection = PGconn.connect(:dbname => 'scratch')
     
      res = @@pgConnection.exec('SELECT notes, filename FROM '+tableName+' WHERE notes = \'' + noteValue + '\'')
      res.each do |row|
        puts row['notes'] + ' | ' + row['filename']
      end	
    end
  end

end

doThings = DoThingsWithRows.new
doThings.Init(ARGV[0], ARGV[1])


