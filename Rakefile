require 'rake'
require 'rake/clean'

NAME    = 'skype'
RELEASE = '0.0.1'
PLUGIN  = "#{NAME}-#{RELEASE}.so"

CC      = (`clang -v &> /dev/null` rescue nil) ? 'clang' : 'gcc'
CFLAGS  = "-Os -Wall -Iinclude -I/usr/include/bitlbee #{`pkg-config 'glib-2.0' --cflags`.strip}"
LDFLAGS = "#{`pkg-config 'glib-2.0' --libs`.strip}"

SOURCES = FileList['sources/**.c']
OBJECTS = SOURCES.ext('o')

CLEAN.include(OBJECTS)
CLOBBER.include(PLUGIN)

task :default => PLUGIN

rule '.o' => '.c' do |t|
    sh "#{CC} -fPIC #{CFLAGS} -o #{t.name} -c #{t.source}"
end

file PLUGIN => OBJECTS do
    sh "#{CC} #{CFLAGS} -shared -o #{PLUGIN} #{OBJECTS} #{LDFLAGS}"
end
