#!/usr/bin/env ruby

require 'optparse'

Version = "0.2.0"

class Configure
    def initialize()
        @qmake = `which qmake`.chomp
        @qmake_default = @qmake.dup
        @install_root = '/usr/local'
        @config = []
        parseOpt()
        checkPro()
        checkQMake()
    end

    def parseOpt
        opt = OptionParser.new
        opt.on("--prefix=#{@install_root}", "install root" ) { |val| @install_root = val }
        opt.on("--qmake=#{@qmake}", 'path to qmake to use') { |val| @qmake = val }
        opt.on("--sdk", "build and install qimsys sdk") { @config.push( 'sdk' ) }
#        opt.on("--examples", "build and install examples") { @config.push( 'examples' ) }
#        opt.on("--tests", "build tests") { @config.push( 'tests' ) }
        opt.parse!(ARGV.dup)
    end

    def checkPro()
        @pro = $0.sub(/configure.rb$/, "silk.pro")
        raise ArgumentError "silk.pro not found" if @pro.empty?
    end

    def checkQMake()
        raise ArgumentError "qmake not found" if @qmake.empty?
    end

    def exec()
        cmd = []
        cmd.push @qmake
        cmd.push @pro
        cmd.push '-r'
        cmd.push "PREFIX=#{@install_root}"

        print "Qt(qmake)        : #{@qmake}\n"
        print "Prefix           : #{@install_root}\n"
#        print "SDK              : #{@config.include?('sdk') ? 'Yes' : 'No'}\n"
#        print "Examples         : #{@config.include?('examples') ? 'Yes' : 'No'}\n"
#        print "Tests            : #{@config.include?('tests') ? 'Yes' : 'No'}\n"
        print( "#{cmd.join(' ')}\n" )
        `#{cmd.join(' ')}`
#        print( "configure finished successfully!\n" )
    end
end

begin
    configure = Configure.new
    configure.exec()
rescue
    print "#{$!}\n"
end

