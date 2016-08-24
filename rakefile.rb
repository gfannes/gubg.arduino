require(File.join(ENV['gubg'], 'shared'))
require('gubg/build/Library')
include GUBG

task :default => :help
task :help do
    puts("The following tasks can be specified:")
    puts("* declare: installs the arduino SDK")
    puts("* define: ")
end

task :clean do
    rm_rf '.cache'
    rm(FileList.new('lib*.a'))
end

task :declare do
    build_ok_fn = 'gubg.build.ok'

    Dir.chdir(shared_dir('extern')) do
        if false
            git_clone('https://github.com/arduino', 'Arduino') do
                if !File.exist?(build_ok_fn)
                    puts("Installing arduino")
                    # sh 'rm -rf build'
                    # sh "make -j 8 CMAKE_EXTRA_FLAGS=\"-DCMAKE_INSTALL_PREFIX:PATH=#{shared('install', 'neovim')}\""
                    # sh 'make install'
                    # Dir.chdir(shared('install', 'neovim', 'bin')) do
                    #     publish('nvim', dst: 'bin', mode: 0755)
                    # end
                    # sh "touch #{build_ok_fn}"
                end
            end
        else
            unless File.exist?('Arduino-master')
                zip_fn = 'master.zip'
                sh "wget https://github.com/arduino/Arduino/archive/#{zip_fn}" unless File.exist?(zip_fn)
                sh "unzip #{zip_fn}" 
            end
        end
    end
end

task :define => :declare do
    lib = Build::Library.new('arduino-core', arch: :uno)
    avr_dir = shared('extern/Arduino-master/hardware/arduino/avr')
    cpp_parts = %w[main]
    c_parts = %w[wiring hooks wiring_digital]
    lib.add_sources(cpp_parts.map{|part|File.join(avr_dir, "cores/arduino/#{part}.cpp")})
    lib.add_sources(c_parts.map{|part|File.join(avr_dir, "cores/arduino/#{part}.c")})
    lib.build
    publish(lib.lib_filename, dst: 'lib')
end

task :test do
end
