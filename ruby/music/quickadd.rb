#!/usr/bin/env ruby

require 'pathname'
require 'taglib'
require 'pg'

def DelimitValue(value)
  return "'" + value.gsub("'","''") + "'"
end

pgConnection = PGconn.connect(:dbname => 'scratch')

begin
  file = Pathname.new(ARGV.first)
  data = TagLib::File.new(file)
  sql = "INSERT INTO Music(Artist, Album, Title, Filename) VALUES (" + DelimitValue(data.artist) + "," + DelimitValue(data.album) + "," + DelimitValue(data.title) + "," + DelimitValue(file.to_s()) + ")"
  pgConnection.exec(sql)
rescue Exception => e 
  puts "Exception occured"
  puts e
ensure
#  sqliteDatabase.close if sqliteDatabase
end

#puts "Done!"


